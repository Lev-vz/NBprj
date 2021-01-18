/* 
 * File:   main.cpp
 * Author: lev
 *
 * Created on 13 января 2021 г., 13:38
 */

/*
	Simple udp client
*/
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
#include </usr/include/ifaddrs.h>
#include <fcntl.h>

//#define SERVER "172.16.35.255"
#define SERVER "0.0.0.0"
#define BUFLEN 512	//Max length of buffer
#define PORT 8888	//The port on which to send data

using namespace std;

void die(const char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    //struct sockaddr *IF_addr;
    //cout<<"addr="<<IFap->ifa_addr->sa_data<<", fam="<<IFap->ifa_addr.sa_family<<endl;
    struct sockaddr_in si_other;//, si_me;
    int s;
    socklen_t slen=sizeof(si_other);
    char buf[BUFLEN];
    string message;//[BUFLEN];

    if ( (s=socket(AF_INET, SOCK_DGRAM, 0)) < 0) die("socket");//IPPROTO_UDP
//    fcntl(s, F_SETFL, O_NONBLOCK);
    
    int n = 1;
    setsockopt(s,SOL_SOCKET,SO_BROADCAST,&n,sizeof(n));
    
    memset((char*) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    si_other.sin_addr.s_addr  = inet_addr(SERVER);
//    if (!inet_aton(SERVER , &si_other.sin_addr)) {
//            fprintf(stderr, "inet_aton() failed\n");
//            exit(1);
//    }
//    //---- сокет для приёма сообщений ----------------
//    memset((char *) &si_me, 0, sizeof(si_me));
//
//    si_me.sin_family = AF_INET;
//    si_me.sin_port = htons(PORT);
//    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
//
//    //bind socket to port
//    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) < 0) die("bind");

    while(1)
    {
        printf("Enter message : ");
        cin>>message;

        //send the message
        if (sendto(s, message.c_str(), message.length() , 0 , (struct sockaddr *) &si_other, slen)<=0)	die("sendto");//()");

        //try to receive some data, this is a blocking call
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) <=0 ) die("recvfrom()");

        puts(buf);
    }
    close(s);
    return 0;
}