/* Author: lev */

#include <iostream>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <cstring>
//#include <linux/if_link.h>
#include <math.h>
#include <time.h>
#include <fcntl.h>
#include <sys/poll.h>

#define BUFLEN 512	//Max length of buffer
#define PORT 888	//The port on which to send data

using namespace std;

void SetSocket(sockaddr_in &sok, int port, char *ip){
    cout<<"sockIP="<<ip<<endl;
    memset((char*) &sok, 0, sizeof(sok));
    sok.sin_family = AF_INET;
    sok.sin_port = htons(port);
    sok.sin_addr.s_addr  = inet_addr(ip);
}

void mastDie(const char *s, int numErr)
{
    perror(s);
    exit(numErr);
}

int getLastByteIP(string subNet){
    //---- Определение ИП данного конкретного устройства для заданной подсети ---
    struct ifaddrs *ifaddr, *ifa;//создаём указатели на структуры отисания сетевых интерфейсов
    int family, s, n;
    char host[NI_MAXHOST];
    if (getifaddrs(&ifaddr) == -1) mastDie("getifaddrs", 1);
    
    /* обходим связный список, сохраняя начальный указатель, чтобы
       освободить список позже */
    int lastAddr = 1;//atoi(host+pos);
    for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
        if (ifa->ifa_addr == NULL)
            continue;
        family = ifa->ifa_addr->sa_family;
        /* для адресов интерфейса AF_INET* показываем адрес */
        if (family == AF_INET){// || family == AF_INET6) {
            s = getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if (s != 0) {
                printf("ошибка getnameinfo(): %s\n", gai_strerror(s));
                exit(2);
            }
            string h = host;
            int pos = subNet.length();
                        if(!h.find(subNet)){
                lastAddr = atoi(host+pos);
                if(lastAddr < 1 && lastAddr > 254){
                    cout<<"Указанная подсеть - "<<subNet<<" отсутствует в интерфейсах устройства"<<endl;
                    exit(3);
                }
            } 
            printf("pos=%d, [%d]\n",pos, lastAddr);
            //cout<<"h="<<host<<", find substr = "<<h.find(subNet)<<endl;
        } 
    }
    freeifaddrs(ifaddr);
    return lastAddr;
}

double getTime(){
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    double time = spec.tv_sec + spec.tv_nsec/1000000000.0;
    return time;
}

int main(int argc, char *argv[])
{   //------- запомнили момент запуска программы -----------
    double time = getTime();
    //-------------------------------------------------------
    struct sockaddr_in sockSt[2];//_other, si_me;
    int sockID[2];
//---- сокет для приёма сообщений ----------------
    if ( (sockID[0] = socket(AF_INET, SOCK_DGRAM, 0)) < 0) mastDie("socket", 1);
    SetSocket(sockSt[0], 8880, INADDR_ANY);
    if( bind(sockID[0] , (struct sockaddr*)&sockSt[0], sizeof(sockSt[0])) < 0) mastDie("bind", 2);
    //--------------------------------------------------------------------
    //----- Определяем свой адрес в заданной подсети -------------
    //cout<<"argc="<<argc<<", argv[1]="<<argv[1]<<endl;
    string subNet = "127.0.0."; //задаём подсеть по умолчанию
    if(argc>1) subNet = argv[1];//если в аргументах командной строки что-о есть - принимаем это за заданную подсеть
    int lastByteIP = getLastByteIP(subNet);
    //-------------------------------------------------------------
    //---- сокет для отправки broadcast сообщений ----------------
    if ( (sockID[1] = socket(AF_INET, SOCK_DGRAM, 0)) < 0) mastDie("socket", 3);//IPPROTO_UDP
    socklen_t slen=sizeof(sockSt[1]);
    int n = 1;
    setsockopt(sockID[1],SOL_SOCKET,SO_BROADCAST,&n,sizeof(n));
    string broadCast = subNet + "255";
    SetSocket(sockSt[1], 8888, (char*) broadCast.c_str());
    //------------------------------------------------------------

    struct pollfd fds[2];
    memset(fds, 0 , sizeof(fds));
    fds[0].fd = sockID[0];
    fds[0].events = POLLIN;
    fds[1].fd = sockID[1];
    fds[1].events = POLLIN;
    //----- Если lastByteIP == 1, запускаем чистого мастера ------
    
    char buf[BUFLEN];
    string message;//[BUFLEN];
    
    while(1){
        double newTime = getTime();
        message = to_string(newTime)+"("+to_string(newTime-time)+")";
        cout<<message<<endl;
        time = newTime;
        //send the message
        if (sendto(sockID[1], message.c_str(), message.length() , 0 , (struct sockaddr *) &sockSt[1], slen)<=0)	mastDie("sendto",4);//()");
        for(int i=0;i<2;i++){
            int rc=1;
            while(rc){
                rc = poll(fds, 2, 1000);
                if (rc < 0){
                  perror("poll");
                  break;
                }
                if(rc > 0){
                    for(int i = 0; i < 2; i++){
                        if(fds[i].revents == POLLIN){
                            //try to receive some data, this is a blocking call
                            int n = recvfrom(sockID[i], buf, BUFLEN, 0, (struct sockaddr *) &(sockSt[i]), &slen);
                            if (n <=0 ) mastDie("recvfrom()", 5);
                            buf[n] = 0;
                            cout<<"On socket "<<i<<" received packet from address "<<inet_ntoa(sockSt[i].sin_addr)<<" port "<<ntohs(sockSt[i].sin_port)<<endl;
                            cout<<"Recive "<<n<<" byte. Buf = "<<buf<<endl;
                        }
                    }
                }
            }
        }
    }

//    clock_gettime(CLOCK_REALTIME, &spec);
//    double dtime = spec.tv_sec + spec.tv_nsec/1000000000.0 - time;
//    cout<<"dtime="<<dtime<<endl;

    return 0;
}