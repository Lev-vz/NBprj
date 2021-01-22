/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: lev
 *
 * Created on 15 января 2021 г., 10:43
 */

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
#include <linux/if_link.h>
#include <math.h>
#include <time.h>
#include <fcntl.h>

#define BUFLEN 512	//Max length of buffer
#define PORT 888	//The port on which to send data

using namespace std;

void SetSocket(int &sok){
    memset((char*) &sok, 0, sizeof(sok));
    si[0].sin_family = AF_INET;
    si[0].sin_port = htons(8888);
    si[0].sin_addr.s_addr  = inet_addr(SERVER);
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

int main(int argc, char *argv[])
{
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    double time = spec.tv_sec + spec.tv_nsec/1000000000.0;

    //cout<<"argc="<<argc<<", argv[1]="<<argv[1]<<endl;
    string subNet = "127.0.0."; //задаём подсеть по умолчанию
    if(argc>1) subNet = argv[1];//если в аргументах командной строки что-о есть - принимаем это за заданную подсеть
    
    int lastByteIP = getLastByteIP(subNet);
    
    struct sockaddr_in si_other;//, si_me;
    int sockID;
    socklen_t slen=sizeof(si_other);
    char buf[BUFLEN];
    string message;//[BUFLEN];

    if ( (sockID=socket(AF_INET, SOCK_DGRAM, 0)) < 0) mastDie("socket", 3);//IPPROTO_UDP
    fcntl(sockID, F_SETFL, O_NONBLOCK);
    
    int n = 1;
    setsockopt(sockID,SOL_SOCKET,SO_BROADCAST,&n,sizeof(n));
    
    string broadCast = subNet + "255";
    
    memset((char*) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    si_other.sin_addr.s_addr  = inet_addr(broadCast.c_str());

    while(1){
        printf("Enter message : ");
        cin>>message;

        //send the message
        if (sendto(sockID, message.c_str(), message.length() , 0 , (struct sockaddr *) &si_other, slen)<=0)	mastDie("sendto",4);//()");
        for(int i=0; i <5;i++){
            //try to receive some data, this is a blocking call
            if (n= recvfrom(sockID, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) <=0 ) mastDie("recvfrom",5);
            buf[n] = 0;
            cout<<"Recive "<<n<<"byte. Buf="<<buf<<endl;
            usleep(10000);
        }
        puts(buf);
    }

    clock_gettime(CLOCK_REALTIME, &spec);
    double dtime = spec.tv_sec + spec.tv_nsec/1000000000.0 - time;
    cout<<"dtime="<<dtime<<endl;

    return 0;
}