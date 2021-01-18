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
#define PORT 8888	//The port on which to send data
#define TRUE 1
#define FALSE 0
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
    struct pollfd fds[2];
    //int desc_ready, end_server = FALSE;
    
    if ( (s=socket(AF_INET, SOCK_DGRAM, 0)) < 0) die("socket");//IPPROTO_UDP
    //fcntl(s, F_SETFL, O_NONBLOCK);
    
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
    memset(fds, 0 , sizeof(fds));
    fds[0].fd = s;
    fds[0].events = POLLIN;
    int nfds = 1;
    
    while(1)
    {
        printf("Enter message : ");
        cin>>message;

        //send the message
        if (sendto(s, message.c_str(), message.length() , 0 , (struct sockaddr *) &si_other, slen)<=0)	die("sendto");//()");
        for(int i=0;i<2;i++){
            int rc = poll(fds, nfds, 3000);
            if (rc < 0){
              perror("  poll() failed");
              break;
            }
            if(rc > 0){
                //try to receive some data, this is a blocking call
                int n = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);
                if (n <=0 ) die("recvfrom()");
                buf[n] = 0;
                cout<<"Received packet from address "<<inet_ntoa(si_other.sin_addr)<<" port "<<ntohs(si_other.sin_port)<<endl;
                cout<<"Recive "<<n<<" byte. Buf = "<<buf<<endl;
            }
        }
    }
    close(s);
    return 0;
}