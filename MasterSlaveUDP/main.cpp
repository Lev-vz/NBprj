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
#include <math.h>
#include <time.h>
#include <fcntl.h>
#include <sys/poll.h>

#define BUFLEN 512	//Max length of buffer
#define Q_SOCK 3	//количество сокетов, опрашиваемое функцией poll

using namespace std;
//Заполнение структуры сокета
void SetSocket(sockaddr_in &sok, int port, in_addr_t ip){
    memset((char*) &sok, 0, sizeof(sok));
    sok.sin_family = AF_INET;
    sok.sin_port = htons(port);
    sok.sin_addr.s_addr  = ip;
}

//сообщение об ошибке и завешение программы с заданным кодом
void mastDie(const char *s, int numErr){
    perror(s);
    exit(numErr);
}

//Определение IP данного конкретного устройства для заданной подсети
//аргумент - адрес подсети в формате 192.168.0. без последнего байта
//возвращает - последний байт IP-адреса в формате int
int getLastByteIP(string subNet){
    struct ifaddrs *ifaddr, *ifa;//создаём указатели на структуры отисания сетевых интерфейсов
    int family, s, n;
    char host[NI_MAXHOST];
    if (getifaddrs(&ifaddr) == -1) mastDie("getifaddrs", 1);
    
    //обходим связный список, сохраняя начальный указатель, чтобы освободить список позже
    int lastAddr = 1;
    for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
        if (ifa->ifa_addr == NULL)
            continue;
        family = ifa->ifa_addr->sa_family;
        // для адресов интерфейса AF_INET* показываем адрес
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
        } 
    }
    freeifaddrs(ifaddr);
    cout<<"В указанной подсети адрес устройства - "<<subNet<<lastAddr<<endl;
    return lastAddr;
}

//Возвращает текущее время в формате double секунд с 0 часов 1 января 1970 года с точностью до микросекунд
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
    struct sockaddr_in sockSt[Q_SOCK];//_other, si_me;
    int sockID[Q_SOCK];
    //---- сокет для приёма сообщений ----------------
    if ( (sockID[0] = socket(AF_INET, SOCK_DGRAM, 0)) < 0) mastDie("socket", 1);
    SetSocket(sockSt[0], 8880, htonl(INADDR_ANY));
    if( bind(sockID[0] , (struct sockaddr*)&sockSt[0], sizeof(sockSt[0])) < 0) mastDie("bind", 2);
    //--------------------------------------------------------------------
    //---- сокет для приёма сообщений ----------------
    if ( (sockID[2] = socket(AF_INET, SOCK_DGRAM, 0)) < 0) mastDie("socket", 9);
    SetSocket(sockSt[2], 8888, htonl(INADDR_ANY));
    if( bind(sockID[2] , (struct sockaddr*)&sockSt[2], sizeof(sockSt[2])) < 0) mastDie("bind", 8);
    //--------------------------------------------------------------------
    //----- Определяем свой адрес в заданной подсети -------------
    //cout<<"argc="<<argc<<", argv[1]="<<argv[1]<<endl;
    string subNet = "127.0.0."; //задаём подсеть по умолчанию
    if(argc>1) subNet = argv[1];//если в аргументах командной строки что-о есть - принимаем это за заданную подсеть
    int lastByteIP = getLastByteIP(subNet);
    //-------------------------------------------------------------
    //---- сокет для отправки broadcast сообщений ----------------
    if ( (sockID[1] = socket(AF_INET, SOCK_DGRAM, 0)) < 0) mastDie("socket", 3);
    socklen_t slen=sizeof(sockSt[1]);
    int n = 1;
    setsockopt(sockID[1],SOL_SOCKET,SO_BROADCAST,&n,sizeof(n));
    string ip = subNet + "255";
    SetSocket(sockSt[1], 8888, inet_addr(ip.c_str()));
    //------------------------------------------------------------
    //---- Сокет на второй порт для инициативной передачи ----
    struct sockaddr_in si_second;
    int s2;
    if ((s2 = socket(AF_INET, SOCK_DGRAM, 0)) < 0) mastDie("socket",6);
    memset((char *) &si_second, 0, sizeof(si_second));

    si_second.sin_family = AF_INET;
    si_second.sin_port = htons(8880);
    si_second.sin_addr.s_addr  = inet_addr("0.0.0.0");
    //-------------------------------------------------------------------------
    //----- Подготовка данных для поллинга-----
    struct pollfd fds[Q_SOCK];
    memset(fds, 0 , sizeof(fds));
    fds[0].fd = sockID[0];
    fds[0].events = POLLIN;
    fds[1].fd = sockID[1];
    fds[1].events = POLLIN;
    fds[2].fd = sockID[2];
    fds[2].events = POLLIN;
    
    //----- Основной цикл ------
    char buf[BUFLEN];                   //буфер для приёма приходящих данных
    string message;                     //буфер для исходящих данных
    bool imMaster = lastByteIP == 1;    //Если lastByteIP == 1, запускаем чистого мастера ------
    while(1){
        /*
         здесь должен быть вызов функции обработки данных от датчиков и подготовка данныях для пересылки мастеру
         */
        double newTime = getTime();         //определяем текущее время
                                            //если с запуска программы или последнего запроса на любой сокет прошло более 10 секунд
                                            //или выставлен флаг "я мастер и прошло 5 сек...
        if(newTime-time > (10.0 + lastByteIP*0.1) || imMaster && newTime-time > 5.0){
            imMaster = true;                    //выставляем флаг "я мастер"
            cout<<"Прошло "<<newTime-time<<" секунд"<<endl; //------------ контрольный вывод ----------------
            message = to_string(newTime)+"("+to_string(newTime-time)+")";//------ формируем сообщение -----------------
                                                            //отправляем broadcast-запрос
            if (sendto(sockID[1], message.c_str(), message.length() , 0 , (struct sockaddr *) &sockSt[1], slen)<=0) mastDie("sendto",4);//()");
            time = newTime;                                 // и засекаем время
        }
        int rc=1;                           //заводим ненулевой флаг ответа функции poll
        while(rc){                          //пока он остаётся ненулевым, производим опрос сокетов
            rc = poll(fds, Q_SOCK, 100);        //проверяем в течении 100мс не пришло ли чего-нибудь на какой-нибудь сокет
            if (rc < 0){                        //если ответ меньше 0
              perror("poll");                       //выводим сообщение об ошибке и выходим из цикла опроса сокетов
              break;
            }
            if(rc > 0){                         //если ответ больше 0
                time = newTime;                     //засекаем время
                for(int i = 0; i < Q_SOCK; i++){    //проходим по всем опрашиваемым сокетам
                    if(fds[i].revents == POLLIN){       //и там, где стоит признак пришедших данных...
                                                            //вычитываем данные из сокета
                        int n = recvfrom(sockID[i], buf, BUFLEN, 0, (struct sockaddr *) &(sockSt[i]), &slen);
                        if (n <=0 ) mastDie("recvfrom()", 5);//проверяем длину посылки - если она меньше или = 0 - завершаем программу с ошибкой
                        buf[n] = 0;                         //если всё нормально - ставим 0 в конец полученных данных
                        string inpIp = inet_ntoa(sockSt[i].sin_addr); //определяем IP отправителя полученной посялки
                        //если этот пакет получен не от самого себя - начинаем обработку полученной информации
                        if(inpIp != (subNet+to_string(lastByteIP)) && inpIp!= "127.0.0.1" && inpIp!= "0.0.0.0"){
                            //----------- контрольный вывод --------------------------
                            cout<<"On socket "<<i<<" received packet from address "<<inet_ntoa(sockSt[i].sin_addr)<<" port "<<ntohs(sockSt[i].sin_port)<<endl;
                            cout<<"Recive "<<n<<" byte. Buf = "<<buf<<endl;
                            //--------------------------------------------------------
                            if(i==2){                       //если посылка пришла на сокет 2 - значит это broadcast запрос
                                /*
                                 Здесь должен быть вызов функции обработки данных из broadcast запроса
                                 */
                                usleep(lastByteIP *10000);      //ждём время равное lastByteIP * 10 мс
                                si_second.sin_addr  = sockSt[i].sin_addr;   //выставляем адрес получателя равным адресу запрашивателя
                                message = to_string(newTime)+"("+to_string(newTime-time)+")";   //формируем сообщение
                                //шлём данные запрашивающему
                                if (sendto(s2, message.c_str(), message.length(), 0, (struct sockaddr*) &si_second, slen) <= 0) mastDie("sendto",7);
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}