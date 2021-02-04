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

//#define BUFFER_SIZE 1024

using namespace std;

//char addr[16] = "127.0.0.1"; //по умолчанию - локальный сервер

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
    if(!line.size()) return -1;
    size_t prev, next, delta = delim.length();	//
    prev = 0;                                   //
    while ((next = line.find(delim, prev)) != string::npos){            //
        //cout<<"prev="<<prev<<", next="<<next<<", delta="<<delta<<", ["<<line.substr(prev, next - prev)<<"]\n";
        if(prev != next) tmp.push_back(line.substr(prev, next - prev)); //
        prev = next + delta;                                            //
    }
    if(prev != line.size()) tmp.push_back(line.substr(prev));
    return 0;
}

extern "C" int vegaFtp(char *commLine)
{
    vector<string> tmp;                 //массив для разбора строки
    parsLine(commLine, " ", tmp);       //
    int tmpSize = tmp.size();
    if(tmpSize < 5 || tmpSize > 6){
        cout<<"\""<<commLine<<"\" - неправильный формат запроса."<<endl<<
                "Должно быть через побел: <IP-адрес> <порт> <логин> <пароль> <путь и имя файла-источника> [целевой путь]"<<endl<<
                "Пример: 172.16.35.97 21 testuser 1 c:/projects/kln/ttt.cfg cfg"<<endl;
        return 2;
    }
    //for(int i=0; i< tmp.size(); i++) cout<<"["<<tmp[i]<<"]"<<endl;
    char buf[BUFFER_SIZE];                               //создаём массив строк для ответов через сокеты
    
    int ctrlSoket = getSocketId((char*)tmp[0].c_str(), atoi(tmp[1].c_str()));                  //создаём сокет управляющего канала
    if(ctrlSoket < 0){
        cout<<"Неправильный адрес "<<tmp[0]<<" или неправильный порт "<<tmp[1]<<". Или FTP-сервер не запущен"<<endl;
        return 3;        
    }
    readServ(ctrlSoket, buf);                            //вычитываем реакцию сервера на установление связи
    
    //!!! тут должна быть проверка реакции сервера

    int ret = login(ctrlSoket, buf, (char*)tmp[2].c_str(), (char*)tmp[3].c_str());    //логинимся с сервером
    if(ret) return ret;
    
    if(tmpSize == 6){
        sprintf(buf,"CWD %s\r\n",tmp[5].c_str());
        //cout<<"Set bin mode: "<<buf<<endl;
        send(ctrlSoket,buf,strlen(buf),0);
        readServ(ctrlSoket, buf);                       //читаем ответ сервера
        string str = buf;
        if(str.substr(0,3) != "250"){
            cout<<"Неправильно указан целевой путь "<<tmp[5]<<endl;
            return 3;        
        }
    }
    
    sprintf(buf,"TYPE I\r\n");
    send(ctrlSoket,buf,strlen(buf),0);
    readServ(ctrlSoket, buf);                       //читаем ответ сервера
    string str = buf;
    if(str.substr(0,3) != "200"){
        cout<<"Не удалось установить бинарный тип передачи файла"<<endl;
        return 4;        
    }

    int dataSocket = getSocketForData(ctrlSoket);
    ret = sendFile(tmp[4], ctrlSoket, dataSocket);
    close(dataSocket);
    readServ(ctrlSoket, buf);                       //читаем ответ сервера
    close(ctrlSoket);  ///закрытие соединения
    str = buf;
    if(str.substr(0,3) != "150") return 5;        
    return 0;
}

