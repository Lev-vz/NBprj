#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <vector>

#include "defines.h"
#include "tools.h"

using namespace std;

char addr[16] = "127.0.0.1";


  bool login(int socketId, char *buf) {
    cout << "Введите имя: "; char name[64] = "testuser";// cin >> name;
    sprintf(buf,"USER %s\r\n",name);
    send(socketId,buf,strlen(buf),0);
    readServ(socketId,buf);
    cout << "Введите пароль: "; char pass[64] = "1";// cin >> pass;
    sprintf(buf,"PASS %s\r\n",pass);
    send(socketId,buf,strlen(buf),0);
    readServ(socketId, buf);
    
    string tmp = buf;
    if(tmp.substr(0,3)=="230") return true;
    return false;
}
 

int main(void)
{
    char buf[2][BUFFER_SIZE];
    int ctrlSoket = getSocketId(addr, 21);
    readServ(ctrlSoket, buf[0]);

    while(!login(ctrlSoket, buf[0]));
    //int dataSocket = getSocketForData(ctrlSoket);
    //cout<<"ctrlSoket="<<ctrlSoket<<", dataSocket="<<dataSocket<<endl;
    string kbInput = "";
    
    
//    list<int> sockArr = {ctrlSoket, dataSocket};
//    list<string> msgArr = {"", ""};
    int sockArr[2] = {ctrlSoket, -1};
    //char msgArr[2][BUFFER_SIZE];// = {"", ""};

    do{
        if(isKbInput(kbInput)){
            if(sockArr[1] == -1) sockArr[1] = getSocketForData(ctrlSoket);
            cout<<"kbInput.substr(0,4) = "<<kbInput.substr(0,4)<<"]"<<endl;
            if(kbInput.substr(0,4) == "STOR"){
                cout<<"kbInput.substr(4) = ["<<kbInput.substr(4)<<"]\n";
                sendFile(kbInput.substr(4), sockArr[0], sockArr[1]);
                close(sockArr[1]);
                sockArr[1] = -1;
            }else{
                kbInput += "\n";
                int rep = send(ctrlSoket, kbInput.c_str(), kbInput.size(), 0);
                cout << "rep = "<<rep<<"\n";
            }
        }
        read2Sockets(sockArr, (char**)buf);
    }while(kbInput!="exit");
    
    close(ctrlSoket);  ///закрытие соединения
    close(sockArr[1]);  ///закрытие соединения
    cout << "Client exit"<<endl;
    return 0;
}
