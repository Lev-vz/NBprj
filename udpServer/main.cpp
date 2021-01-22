/* 
 * File:   main.cpp
 * Author: lev
 *
 * Created on 13 января 2021 г., 12:11
 */

using namespace std;

#include<stdio.h>	//printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<iostream>
#include <iostream>
#include <stdio.h>	//printf
#include <string.h> //memset
#include <stdlib.h> //exit(0);
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>

#define BUFLEN 512	//Max length of buffer
#define PORT 8888	//The port on which to listen for incoming data
#define SERVER "172.16.35.97"

void die(const char *s)
{
	perror(s);
	exit(1);
}

int main(int argc, char *argv[])
{
    long ta = 500000;
    if(argc>1) ta = atoi(argv[1])*1000;//если в аргументах командной строки что-о есть - принимаем это за заданную подсеть

    struct sockaddr_in si_me, si_other, si_second;
    //struct sockaddr_in ;

    int s, s2, recv_len;
    socklen_t slen = sizeof(si_other);
    //socklen_t slen2 = sizeof(si_second);
    char buf[BUFLEN];
    //---------------------------------------------------------------------------
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) die("socket");
    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(8888);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) < 0) die("bind");
    //---------------------------------------------------------------------------
    //---- Сокет на второй порт для инициативной передачи ----
    if ((s2 = socket(AF_INET, SOCK_DGRAM, 0)) < 0) die("socket");
    memset((char *) &si_second, 0, sizeof(si_me));

    si_second.sin_family = AF_INET;
    si_second.sin_port = htons(8880);
    si_second.sin_addr.s_addr  = inet_addr("172.16.35.97");
    //-------------------------------------------------------------------------

    //keep listening for data
    while(1)
    {
        cout<<"Waiting for data...";
        fflush(stdout);

        if (recv_len <= 0) die("recvfrom");
        buf[recv_len] = 0;
        cout<<"Received packet from address "<<inet_ntoa(si_other.sin_addr)<<" port "<<ntohs(si_other.sin_port)<<endl;
        //printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), si_other.sin_port);
        printf("Data: %s\n" , buf);
        usleep(ta);
        //if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) <= 0) die("sendto()");
        if (sendto(s2, buf, recv_len, 0, (struct sockaddr*) &si_second, slen) <= 0) die("sendto");
    }

    close(s);
    return 0;
}