#include <iostream>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "defines.h"
#include "tools.h"
#include <sys/select.h>

//#define BUFFER_SIZE      1024

using namespace std;

int getSoketForServer(int port, int n){
    int socketId = socket(AF_INET, SOCK_STREAM, 0); //Создаёт конечную точку соединения и возвращает файловый дескриптор
    
    struct sockaddr_in serverAddr;                  //структура данных для открытия исходящего сокета
    bzero((char*)&serverAddr, sizeof(serverAddr));  //обнуление всех полей структуры
    serverAddr.sin_family       = AF_INET;          //семейство протоколов - интернет
    serverAddr.sin_port         = htons(port);      //назначаем порт 3081
    serverAddr.sin_addr.s_addr  = INADDR_ANY;       //INADDR_ANY - все адреса локального хоста (0.0.0.0); INADDR_LOOPBACK - (127.0.0.1); 
    bind(socketId, (struct sockaddr *) &serverAddr, sizeof(serverAddr));//Связывает сокет с конкретным адресом
    listen(socketId, n);                            //Подготавливает привязываемый сокет к принятию входящих соединений
    cout<<"Server open port "<<port<<"\n";
    return socketId;
}

int getAccept(int socketId){
    struct  sockaddr_storage serverStorage;
    socklen_t addr_size   = sizeof(serverStorage);
    int newSocket = accept(socketId, (struct sockaddr*)&serverStorage, &addr_size); //приём запроса на установление соединения от удаленного хоста
    cout<<"newSocket"<<"-"<<newSocket<<endl;//----------------------------------Check Out -----------------------------------------
    return newSocket;
}
void * fileLoadThread(void *attr){//int ctrlSocket){
    int socketId = getSoketForServer(3081, 1);//socket(AF_INET, SOCK_STREAM, 0); //Создаёт конечную точку соединения и возвращает файловый дескриптор
    
    int newSocket = getAccept(socketId);//accept(socketId, (struct sockaddr*)&serverStorage, &addr_size); //приём запроса на установление соединения от удаленного хоста
    //cout<<newSocket<<"-";//----------------------------------Check Out -----------------------------------------
    string resp;
    char buffer[BUFFER_SIZE];
    FILE *f = (FILE*)attr;
    while(1){
        bzero(buffer,BUFFER_SIZE);
        int len  = rcv(newSocket, buffer, 81);//read(newSocket, buffer, BUFFER_SIZE - 1);
        if(!strcmp(buffer,"exitFload")){
            break;
        }else{
            fputs(buffer,f);
        }
        resp = to_string(len).c_str();//+" ";
        cout<<buffer<<"\n";
        send(newSocket, resp.c_str(), resp.length(),0);
    }
    resp = "fileLoadComplit";
    send(newSocket, resp.c_str(), resp.length(),0);
    cout<<resp<<"\n";
    fclose(f);
    close(newSocket);
    close(socketId);
}

pthread_t fileLoad(int newSocket){
    char buffer[BUFFER_SIZE];
    int len  = rcv(newSocket, buffer, 80); 
    string resp = to_string(len).c_str();//+" ";
    send(newSocket, resp.c_str(), resp.length(),0);
    
    string fileName=buffer;
    FILE *f = fopen(fileName.c_str(),"wb");
    if(f<=0) return -1;
    pthread_t tid;
    void* attr = (void*)f;
    pthread_create(&tid, NULL, fileLoadThread, attr);
    return tid;
}

int main(int argc, char** argv) {
    pthread_t tid; /* идентификатор потока */
    void* attr=NULL; /* aтрибуты потока */
    
    int socketId = getSoketForServer(3080, 5);//socket(AF_INET, SOCK_STREAM, 0); //Создаёт конечную точку соединения и возвращает файловый дескриптор
    
    while(1)
    {
        int newSocket = getAccept(socketId);//accept(socketId, (struct sockaddr*)&serverStorage, &addr_size); //приём запроса на установление соединения от удаленного хоста
        //cout<<newSocket<<"-";//----------------------------------Check Out -----------------------------------------
        string resp;
        char buffer[BUFFER_SIZE];
        while(1){
            bzero(buffer,BUFFER_SIZE);
            int get  = read(newSocket, buffer, BUFFER_SIZE - 1);
            buffer[get] = '\0';
            resp = to_string(get).c_str();//+" ";
            cout<<"80:"<<buffer<<"\n";
            send(newSocket, resp.c_str(), resp.length(),0);
            if(!strcmp(buffer,"fload")){
                fileLoad(newSocket);//pthread_create(&tid, NULL, fileLoad, attr);
            }else if(!strcmp(buffer,"exit") || !strcmp(buffer,"exitServer") || !strcmp(buffer,"")) break;
        }
        close(newSocket);
        if(!strcmp(buffer,"exitServer")) break;
    }
    close(socketId);
    cout<<"Normal server exit\n";
    return 0;
}