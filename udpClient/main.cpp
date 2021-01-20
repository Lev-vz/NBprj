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

int main(void)
{
    struct sockaddr_in si_other, si_me;
    int s, s2;
    socklen_t slen=sizeof(si_other);
    char buf[BUFLEN];
    string message;//[BUFLEN];
    struct pollfd fds[2];
    
    //---- сокет для приёма сообщений ----------------
    if ( (s2=socket(AF_INET, SOCK_DGRAM, 0)) < 0) die("socket", 2);//IPPROTO_UDP
    
    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(8880);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to port
    if( bind(s2 , (struct sockaddr*)&si_me, sizeof(si_me) ) < 0) die("bind", 3);
    //--------------------------------------------------------------------
    if ( (s=socket(AF_INET, SOCK_DGRAM, 0)) < 0) die("socket", 1);//IPPROTO_UDP

    int n = 1;
    setsockopt(s,SOL_SOCKET,SO_BROADCAST,&n,sizeof(n));
    
    memset((char*) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(8888);
    si_other.sin_addr.s_addr  = inet_addr(SERVER);
    memset(fds, 0 , sizeof(fds));
    fds[0].fd = s;
    fds[0].events = POLLIN;
    fds[1].fd = s2;
    fds[1].events = POLLIN;
    int nfds = 1;
    
    while(1)
    {
        printf("Enter message : ");
        cin>>message;

        //send the message TrueFalse1
        if (sendto(s, message.c_str(), message.length() , 0 , (struct sockaddr *) &si_other, slen)<=0)	die("sendto", 4);//()");
        for(int i=0;i<2;i++){
            int rc = poll(fds, nfds, 3000);
            if (rc < 0){
              perror("  poll() failed");
              break;
            }
            if(rc > 0){
                for(int i = 0; i < 2; i++){
                    if(fds[i].revents == POLLIN){
                        //try to receive some data, this is a blocking call
                        int n = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);
                        if (n <=0 ) die("recvfrom()", 5);
                        buf[n] = 0;
                        cout<<"On socket "<<i<<" received packet from address "<<inet_ntoa(si_other.sin_addr)<<" port "<<ntohs(si_other.sin_port)<<endl;
                        cout<<"Recive "<<n<<" byte. Buf = "<<buf<<endl;
                    }
                }
            }
        }
    }
    close(s);
    return 0;
}