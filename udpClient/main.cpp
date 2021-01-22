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
//#include <fcntl.h>
#include <sys/poll.h>

#define SERVER "172.16.35.255"
//#define SERVER "0.0.0.0"
#define BUFLEN 512	//Max length of buffer
//#define PORT 8888	//The port on which to send data
#define TRUE 1
#define FALSE 0
using namespace std;

void die(const char *s, int e)
{
    perror(s);
    exit(e);
}

void SetSock(sockaddr_in &sock){
    memset((char*) &sock, 0, sizeof(sock));
    sock.sin_family = AF_INET;
    sock.sin_port = htons(8888);
    sock.sin_addr.s_addr  = inet_addr(SERVER);
}

int main(void)
{
    struct sockaddr_in si[2];//_other, si_me;
    int s[2];
    socklen_t slen[2];
    slen[0]=sizeof(si[0]);
    slen[1]=sizeof(si[1]);
    char buf[BUFLEN];
    string message;//[BUFLEN];
    struct pollfd fds[2];
    
    //---- сокет для приёма сообщений ----------------
    if ( (s[1]=socket(AF_INET, SOCK_DGRAM, 0)) < 0) die("socket", 2);//IPPROTO_UDP
    
    //SetSock(si[1]);
    memset((char *) &si[1], 0, sizeof(si[1]));
    si[1].sin_family = AF_INET;
    si[1].sin_port = htons(8880);
    si[1].sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to port
    if( bind(s[1] , (struct sockaddr*)&(si[1]), sizeof(si[1]) ) < 0) die("bind", 3);
    //--------------------------------------------------------------------
    if ( (s[0]=socket(AF_INET, SOCK_DGRAM, 0)) < 0) die("socket", 1);//IPPROTO_UDP

    int n = 1;
    setsockopt(s[0],SOL_SOCKET,SO_BROADCAST,&n,sizeof(n));
    //SetSock(si[0]);
    memset((char*) &si[0], 0, sizeof(si[0]));
    si[0].sin_family = AF_INET;
    si[0].sin_port = htons(8888);
    si[0].sin_addr.s_addr  = inet_addr(SERVER);
    //------------------------------------------------------------------------
    memset(fds, 0 , sizeof(fds));
    fds[0].fd = s[0];
    fds[0].events = POLLIN;
    fds[1].fd = s[1];
    fds[1].events = POLLIN;
    int nfds = 2;
    
    while(1)
    {
        printf("Enter message : ");
        cin>>message;

        //send the message TrueFalse1
        if (sendto(s[0], message.c_str(), message.length() , 0 , (struct sockaddr *) &si[0], slen[0])<=0)	die("sendto", 4);//()");
        for(int i=0;i<2;i++){
            int rc=1;
            while(rc){
                rc = poll(fds, nfds, 3000);
                if (rc < 0){
                  perror("  poll() failed");
                  break;
                }
                if(rc > 0){
                    for(int i = 0; i < 2; i++){
                        if(fds[i].revents == POLLIN){
                            //try to receive some data, this is a blocking call
                            int n = recvfrom(s[i], buf, BUFLEN, 0, (struct sockaddr *) &si[i], &slen[i]);
                            if (n <=0 ) die("recvfrom()", 5);
                            buf[n] = 0;
                            cout<<"On socket "<<i<<" received packet from address "<<inet_ntoa(si[i].sin_addr)<<" port "<<ntohs(si[i].sin_port)<<endl;
                            cout<<"Recive "<<n<<" byte. Buf = "<<buf<<endl;
                        }
                    }
                }
            }
        }
    }
    close(s[0]);
    close(s[1]);
    return 0;
}