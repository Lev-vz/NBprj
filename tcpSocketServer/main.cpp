#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>
//#include <fcntl.h>
#include <algorithm>
#include <set>
#include <iostream>
#include "defines.h"

using namespace std;

int readSockets(set<int> &clients, fd_set &readset);

int main()
{
    char buf[BUFFER_SIZE];
    int bytes_read;

    int socketID = socket(AF_INET, SOCK_STREAM, 0);
    if(socketID < 0)
    {
        perror("socket");
        exit(1);
    }
    cout<<"Open base socket "<<socketID<<endl;
    //fcntl(socketID, F_SETFL, O_NONBLOCK);
    
    struct sockaddr_in addr;
    bzero((char*)&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3080);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    if(bind(socketID, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }
    cout<<"Bind socket with port 3080"<<endl;

    listen(socketID, 2);
    
    set<int> clients;
    clients.clear();
    
    fd_set readset;

    while(1)
    {
        // Заполняем множество сокетов
        FD_ZERO(&readset);
        FD_SET(socketID, &readset);

        for(set<int>::iterator it = clients.begin(); it != clients.end(); it++) FD_SET(*it, &readset);

        // Задаём таймаут
        timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        // Ждём события в одном из сокетов
        int mx = max(socketID, *max_element(clients.begin(), clients.end()));
        
        if(select(mx+1, &readset, nullptr, nullptr, &timeout) < 0)
        {
            perror("select");
            exit(3);
        }
        //cout<<"5 sec go out but nothing!"<<endl;
        
        // Определяем тип события и выполняем соответствующие действия
        if(FD_ISSET(socketID, &readset))
        {
            // Поступил новый запрос на соединение, используем accept
            int sock = accept(socketID, NULL, NULL);
            if(sock < 0)
            {
                perror("accept");
                exit(4);
            }
            clients.insert(sock);
        }
        readSockets(clients, readset);
    }
    
    return 0;
}

int readSockets(set<int> &clients, fd_set &readset){
    char buf[BUFFER_SIZE];
    for(set<int>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        if(FD_ISSET(*it, &readset)){// Поступили данные от клиента, читаем их
            int bytes_read = recv(*it, buf, BUFFER_SIZE-1, 0);
            buf[bytes_read]='\0';
            cout<<"buf="<<buf<<"|"<<endl;

            if(bytes_read <= 0){// Соединение разорвано, удаляем сокет из множества
                cout<<"Socket send "<<bytes_read<<" byts and connect abort."<<endl;
                close(*it);
                clients.erase(*it);
            }else if(bytes_read > 3)
                send(*it, buf, bytes_read, 0);// Отправляем данные обратно клиенту
        }
    }
    return 0;
}