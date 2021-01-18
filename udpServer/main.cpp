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

void die(const char *s)
{
	perror(s);
	exit(1);
}

int main(void)
{
    struct sockaddr_in si_me;
    struct sockaddr_in si_other;

    int s, recv_len;
    socklen_t slen = sizeof(si_other);
    char buf[BUFLEN];

    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, 0)) < 0) die("socket");
    
    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) < 0) die("bind");

    //keep listening for data
    while(1)
    {
        //printf(
        cout<<"Waiting for data...";
        fflush(stdout);

        //try to receive some data, this is a blocking call
        recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, (socklen_t*) &slen);
        if (recv_len <= 0) die("recvfrom");
        buf[recv_len] = 0;
        //print details of the client/peer and the data received inet_ntoa(*(struct in_addr*)&ipaddr)
        cout<<"Received packet from address "<<inet_ntoa(si_other.sin_addr)<<" port "<<ntohs(si_other.sin_port)<<endl;
        //printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), si_other.sin_port);
        printf("Data: %s\n" , buf);

        //now reply the client with the same data
        if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) <= 0) die("sendto()");
    }

    close(s);
    return 0;
}