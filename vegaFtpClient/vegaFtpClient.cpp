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

#include "../common/defines.h"
#include "../common/tools.h"

#define BUFFER_SIZE 1024

using namespace std;

char addr[16] = "127.0.0.1"; //по умолчанию - локальный сервер

//идентификация. Для тестирования имена задаются прямо в ней. Потом заменить на аргументы
int login(int socketId, char *buf, char *name, char *pass) {
    sprintf(buf,"USER %s\r\n",name);                        //формируем логин для сервера
    send(socketId,buf,strlen(buf),0);                       //отправляем логин
    readServ(socketId,buf);                                 //читаем ответ сервера. Пока ника не используем
    sprintf(buf,"PASS %s\r\n",pass);                        //формируем пароль для сервера
    send(socketId,buf,strlen(buf),0);                       //отправляем пароль
    readServ(socketId, buf);                                //читаем ответ сервера
    
    string tmp = buf;
    if(tmp.substr(0,3)=="230") return 0;                 //если логин/пероль прошли - возвращаем 0
    return 1;//atoi(tmp.substr(0,3));                        //если н прошли -0 возвращаем код ошибки
}
 
int parsLine(string line, string delim, vector<string> &tmp){
    size_t prev, next, delta = delim.length();	//
    prev = 0;                                   //
    while ((next = line.find(delim, prev)) != string::npos){            //
        cout<<"prev="<<prev<<", next="<<next<<", delta="<<delta<<", ["<<line.substr(prev, next - prev)<<"]\n";
        if(prev != next) tmp.push_back(line.substr(prev, next - prev)); //
        prev = next + delta;                                            //
    }
    if(prev != line.size()) tmp.push_back(line.substr(prev));
    return 0;
}

extern "C" int ftp(char *commLine)
{
    vector<string> tmp;
    parsLine(commLine, " ", tmp);
    for(int i=0; i< tmp.size(); i++) cout<<"["<<tmp[i]<<"]"<<endl;
    /*
    char buf[2][BUFFER_SIZE];                               //создаём массив строк для ответов через сокеты
    int ctrlSoket = getSocketId(addr, 21);                  //создаём сокет управляющего канала
    readServ(ctrlSoket, buf[0]);                            //вычитываем реакцию сервера на установление связи
    
    //!!! тут должна быть проверка реакции сервера

    int ret = login(ctrlSoket, buf[0], name, pass);                     //логинимся с сервером, пока не залогинимся - функция login должна вернуть true
    if(!ret) return ret;

    int sockArr[2] = {ctrlSoket, -1};                       //создаём массив из двух сокетов
    do{
        if(isKbInput(kbInput)){
            if(sockArr[1] == -1) sockArr[1] = getSocketForData(ctrlSoket);
            //cout<<"kbInput.substr(0,4) = "<<kbInput.substr(0,4)<<"]"<<endl;
            if(kbInput.substr(0,4) == "STOR"){
                //cout<<"kbInput.substr(4) = ["<<kbInput.substr(5)<<"]\n";
                sendFile(kbInput.substr(5), sockArr[0], sockArr[1]);
                close(sockArr[1]);
                sockArr[1] = -1;
            }else{
                kbInput += "\n";
                int rep = send(ctrlSoket, kbInput.c_str(), kbInput.size(), 0);
                cout << "rep = "<<rep<<"\n";
            }
        }
        read2Sockets(sockArr, (char**)buf, 1);
    }while(kbInput!="exit");
    
    close(ctrlSoket);  ///закрытие соединения
    close(sockArr[1]);  ///закрытие соединения
    cout << "Client exit"<<endl;
    */
    return 0;
}

