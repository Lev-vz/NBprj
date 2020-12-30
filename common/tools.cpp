#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <sys/time.h>
#include <set>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <list>

#include "defines.h"
#include "tools.h"

using namespace std;
  

bool isKbInput(string &s){
    timeval tv0;
    tv0.tv_sec = 0;
    tv0.tv_usec = 0;
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    s = "";
    if(select(1, &fds, nullptr, nullptr, &tv0)){
        while(1){
            char ch = getchar();
            if(ch == 10) break;
            s += ch;
        }
        return true;
    }
    return false;
}

int rcv(int id, char *buf, int port){
    int len = recv(id, buf, BUFFER_SIZE - 1, 0);
    buf[len] = '\0';
    cout<<port<<" port:"<<buf<<"|\n";                
    return len;
}

timeval setTimeOut(int s, int us){
    timeval timeout;
    timeout.tv_sec = s;   ///зададим  структуру времени со значением 1 сек
    timeout.tv_usec = 0;  
    return timeout;
}

int readServ(int socketID, char *buf, int ta) {
    timeval timeout = setTimeOut(ta,0);  //зададим  структуру времени со значением 5 сек
    fd_set readset;                     //создаём структуру для анализа функцией select
    FD_ZERO(&readset);                  //чистим её
    FD_SET(socketID, &readset);         //записываем в неё дескриптер сокета
    int selRep = select(socketID+1, &readset, nullptr, nullptr, &timeout);
    if(selRep < 0)
    {
        perror("select");
        exit(3);
    }
    cout<<"selRep = "<<selRep<<endl;
    selRep = FD_ISSET(socketID, &readset);
    //cout<<"issetRep = "<<selRep<<endl;
    if(selRep){// Поступили данные от клиента, читаем их
        //cout<<"Что-то пришло!"<<endl;
        int bytes_read = recv(socketID, buf, BUFFER_SIZE-1, 0);
        buf[bytes_read]='\0';
        cout<<"buf="<<buf<<", bytes_read="<<bytes_read<<endl;
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

int readSockets(list<int> &sockArr, list<string> buf, int ta){
    if(sockArr.empty()){
        cout<<"sockArr empty\n";
        return 0; 
    }

    auto maxSockID = max_element(sockArr.begin(), sockArr.end());
    cout<<"maxSockID = "<<*maxSockID<<", ta = "<<ta<<endl;
    timeval timeout = setTimeOut(ta,0);  //зададим  структуру времени со значением 1 сек
    fd_set readset;                     //создаём структуру для анализа функцией select
    FD_ZERO(&readset);                  //чистим её
    for(int sock: sockArr){
        //cout<<"socket="<<sock<<endl;
        FD_SET(sock, &readset);         //записываем в неё дескриптер сокета
    } 
    int selRep = select(*maxSockID + 1, &readset, nullptr, nullptr, &timeout);
    if(selRep < 0){
        perror("select");
        exit(3);
    }
    char bufTmp[BUFFER_SIZE];
    if(selRep > 0){
        auto itBuf = buf.begin();
        for(auto it = sockArr.begin(); it != sockArr.end(); it++){
            if(FD_ISSET(*it, &readset)){// Поступили данные от клиента, читаем их
                int bytes_read = recv(*it, bufTmp, BUFFER_SIZE-1, 0);
                if(bytes_read <= 0){// Соединение разорвано, удаляем сокет из множества
                    cout<<"Socket "<<*it<<" send "<<bytes_read<<" byts and connect abort."<<endl;
                    sockArr.remove(*it);
                    close(*it);
                    return -(*it);
                }else{
                    bufTmp[bytes_read]='\0';
                    *itBuf = bufTmp;
                    cout<<"socket "<<*it<<" recive "<<bytes_read<<" byte:\n"<<*itBuf<<"|"<<endl;
                }
            }else *itBuf = "";
            itBuf++;
        }
    }
    return 0;
}

int getSocketId(char *ipAddr, int port){//sockaddr_in serverAddr){
    int socketId = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;
    bzero((char*)&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family       = AF_INET;
    serverAddr.sin_port         = htons(port);
    serverAddr.sin_addr.s_addr  = inet_addr(ipAddr);
    
    socklen_t addrSize = sizeof(serverAddr);
    if(connect(socketId, (struct sockaddr*)&serverAddr, addrSize)) return -1;
    return socketId;
}

int read2Sockets(int *sockArr, char **buf, int ta){
    int maxSockID = max(sockArr[0], sockArr[1]);
    //cout<<"maxSockID = "<<maxSockID<<", ta = "<<ta<<endl;
    timeval timeout = setTimeOut(ta,0);  //зададим  структуру времени со значением 1 сек
    fd_set readset;                     //создаём структуру для анализа функцией select
    FD_ZERO(&readset);                  //чистим её
    for(int i=0; i < 2; i++){
        //cout<<"socket["<<i<<"]="<<sockArr[i]<<endl;
        if(sockArr[i] > 0) FD_SET(sockArr[i], &readset);         //записываем в неё дескриптер сокета
    } 
    int selRep = select(maxSockID+1, &readset, nullptr, nullptr, &timeout);
    //cout<<"selPep = "<<selRep<<endl;
    if(selRep < 0){
        perror("select");
        exit(3);
    }
    char bufTmp[BUFFER_SIZE];
    if(selRep > 0){
        for(int i=0; i < 2 && sockArr[i] > 0; i++){
            int isset = FD_ISSET(sockArr[i], &readset);
            //cout<<"isset["<<i<<"] = "<<isset<<endl;
            if(isset){// Поступили данные от клиента, читаем их
                int bytes_read = recv(sockArr[i], bufTmp, BUFFER_SIZE-1, 0);
                //cout<<"Socket "<<sockArr[i]<<" send "<<bytes_read<<endl;
                if(bytes_read <= 0){// Соединение разорвано, удаляем сокет из множества
                    cout<<"Socket "<<sockArr[i]<<" send "<<bytes_read<<" byts and connect abort."<<endl;
                    close(sockArr[i]);
                    if(i==1) sockArr[1] = -1;
                    else exit(0);
                    //return -sockArr[i];
                }else{
                    bufTmp[bytes_read]='\0';
                    cout<<"socket"<<sockArr[i]<<":\n"<<bufTmp;//<<"|"<<endl;
                }
            }
        }
    }
    return 0;
}

//----- Создаение сокета по полученным у сервера порта для получения данных ----------
int getSocketForData(int socketID) {
    send(socketID,"PASV\r\n",strlen("PASV\r\n"),0);
    char buff[BUFFER_SIZE];
    recv(socketID,buff,BUFFER_SIZE,0);
    cout<<"Server ans:" << buff<<endl; //выводим на экран полученную от сервера строку
    //----- Вытягиваем из этой строки адрес и порт для получения данных
    char *tmp_char;
    tmp_char = strtok(buff,"(");
    tmp_char = strtok(NULL,"(");
    tmp_char = strtok(tmp_char, ")");
    int a,b,c,d,e,f;
    sscanf(tmp_char, "%d,%d,%d,%d,%d,%d",&c,&d,&e,&f,&a,&b);
    //------- открытие сокета по полученным адресу и порту ---------
    int port = a*256 + b;
    char address[16];
    sprintf(address,"%d.%d.%d.%d",c,d,e,f);
    cout<<"address="<<address<<endl;
    return getSocketId(address, port);
}

void sendFile(string pathFile, int socketID, int socketForFile){
    cout<<"pathFile = ["<<pathFile.erase(0, pathFile.find_first_not_of(" \n\r\t"))<<"]\n";
    char buffer[BUFFER_SIZE];
    FILE *f = fopen(pathFile.c_str(), "rb");
    if(f<=0) return;
    int x = pathFile.find_last_of("/");
    string fn = pathFile.substr(x+1);
    sprintf(buffer,"STOR %s\r\n",fn.c_str());
    int qByteSend = send(socketID,buffer,strlen(buffer),0);
    cout<<"buffer:"<<buffer<<" qByteSend="<<qByteSend<<"\n";
    while(!feof(f)){ //пока не конец файла, передаем частями файл (сколько помещается в буфере)
        int qSimb=fread(buffer,1,BUFFER_SIZE,f);
        cout<<"buffer:"<<buffer;//"<<"\n";
        if(qSimb!=0){
            int qByte = send(socketForFile, buffer, qSimb, 0);
            //recv(socketForFile, buffer, BUFFER_SIZE - 1, 0);
            cout<<" qByte="<<qByte<<"\n";                
        }
    }
    //close(socketForFile);
}