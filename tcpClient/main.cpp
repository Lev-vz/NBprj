#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>

#include "defines.h"
#include "tools.h"

//#define BUFFER_SIZE 1024

using namespace std;

string ip;

int getSocketId(int port){//sockaddr_in serverAddr){
    int socketId = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;
    bzero((char*)&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family       = AF_INET;
    serverAddr.sin_port         = htons(port);
    serverAddr.sin_addr.s_addr  = inet_addr(ip.c_str());
    
    socklen_t addrSize = sizeof(serverAddr);
    if(connect(socketId, (struct sockaddr*)&serverAddr, addrSize)) return -1;
    return socketId;
}

int socketForFile;

void * sendFile(void * attr){
    socketForFile = getSocketId(3081);
    if(socketForFile < 0){
        cout<<"Connect with fileServer failed\n";
        return attr;
    }else cout<<"Connect with fileServer success!\n";
    char buffer[BUFFER_SIZE];
    FILE *f = (FILE*)attr;
    while(!feof(f)){ //пока не конец файла, передаем частями файл (сколько помещается в буфере)
        int qSimb=fread(buffer,1,10,f);
        cout<<"buffer:"<<buffer<<"\n";
        if(qSimb!=0){
            send(socketForFile, buffer, qSimb*sizeof(char), 0);
            recv(socketForFile, buffer, BUFFER_SIZE - 1, 0);
            //cout<<buffer<<"\n";                
        }
    }
    send(socketForFile,"exitFload",9,0);
    cout<<"exitFload"<<"\n";
    read(socketForFile, buffer, BUFFER_SIZE - 1);
    shutdown(socketForFile, SHUT_WR);
    close(socketForFile);
    socketForFile = -1;
}

FILE *getFileDescr(string pathFile, int socketId, char *buffer){
    FILE *f;
    f = fopen(pathFile.c_str(),"rb");
    if(f>0){
        int x = pathFile.find_last_of("/");
        string fn = pathFile.substr(x+1);
       
        send(socketId,"fload",5,0);
        recv(socketId, buffer, BUFFER_SIZE - 1,0);
        send(socketId,fn.c_str(),fn.length(),0);
        recv(socketId, buffer, BUFFER_SIZE - 1,0);
    } else return NULL;
    
    return f;
}

int main(int argc, char* argv[]){
    string mess = "Hi!";
    if (argc < 2) {
        cout<<"Input ip\n";
        cin>>ip;
    }else ip = argv[1];
    
    int socketId = getSocketId(3080);//serverAddr);

    if(socketId < 0){
        cout<<"Connect with server failed\n";
        return 0;
    } else cout<<"Connect with server!\n";
    
    send(socketId, mess.c_str(), mess.length(),0);
    
    char buffer[BUFFER_SIZE];
    readServ(socketId, buffer);
    
    //timeval to = setTimeOut(5,0);
    //globInit();
    //-----------------------------------------------------------------
    pthread_t tid; /* идентификатор потока */
    while(1){
        cout<<"Enter message: ";
        if(isKbInput(mess)){
            if(mess=="fload"){
                string pathFile;
                cout<<"Path and name file: ";
                cin>>pathFile;
                FILE *f = getFileDescr(pathFile, socketId, buffer);
                if(f!=NULL){
                    void* attr = (void*)f;
                    pthread_create(&tid, NULL, sendFile, attr);
                }
            }else{
                if(send(socketId, mess.c_str(), mess.length(),0)<=0){
                    perror("send");
                    //exit(2);
                }
            }
            
            if(mess=="exit" || mess=="exitServer") break;
        }
        readServ(socketId, buffer);
    }
    //------------------------------------------------------------------
    readServ(socketId, buffer);
    close(socketId);
}