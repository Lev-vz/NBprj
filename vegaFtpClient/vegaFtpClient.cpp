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

extern "C" int ftp(char *commLine)
{
    vector<string> tmp;                 //массив для разбора строки
    parsLine(commLine, " ", tmp);       //
    int tmpSize = tmp.size();
    if(tmpSize < 5 || tmpSize > 6){
        cout<<"\""<<commLine<<"\" - неправильный формат запроса."<<endl<<
                "Должно быть через побел: <IP-адрес> <порт> <логин> <пароль> <путь и имя файла-источника> [целевой путь]"<<endl<<
                "Пример: 172.16.35.97 21 testuser 1 c:/projects/kln/ttt.cfg cfg"<<endl;
        exit(2);
    }
    //for(int i=0; i< tmp.size(); i++) cout<<"["<<tmp[i]<<"]"<<endl;
    char buf[2][BUFFER_SIZE];                               //создаём массив строк для ответов через сокеты
    
    int ctrlSoket = getSocketId((char*)tmp[0].c_str(), atoi(tmp[1].c_str()));                  //создаём сокет управляющего канала
    if(ctrlSoket < 0){
        cout<<"Неправильный адрес "<<tmp[0]<<" или неправильный порт "<<tmp[1]<<". Или FTP-сервер не запущен"<<endl;
        exit(3);        
    }
    readServ(ctrlSoket, buf[0]);                            //вычитываем реакцию сервера на установление связи
    
    //!!! тут должна быть проверка реакции сервера

    int ret = login(ctrlSoket, buf[0], (char*)tmp[2].c_str(), (char*)tmp[3].c_str());    //логинимся с сервером
    if(!ret) return ret;
    
    if(tmpSize == 6){
        sprintf(buf[0],"CWD %s\r\n",tmp[5].c_str());
        send(ctrlSoket,buf[0],strlen(buf[0]),0);
        readServ(ctrlSoket, buf[0]);                       //читаем ответ сервера
        string tmp = buf[0];
        if(tmp.substr(0,3) != "250"){
            cout<<"Неправильно указан целевой путь "<<tmp[5]<<endl;
            exit(3);        
        }
    }
    
    sprintf(buf[0],"TYPE I\r\n");
    send(ctrlSoket,buf[0],strlen(buf[0]),0);
    readServ(ctrlSoket, buf[0]);                       //читаем ответ сервера
    string str = buf[0];
    if(str.substr(0,3) != "200"){
        cout<<"Не удалось установить бинарный тип передачи файла"<<endl;
        exit(3);        
    }

    int sockArr[2] = {ctrlSoket, -1};                       //создаём массив из двух сокетов
    sockArr[1] = getSocketForData(ctrlSoket);
    ret = sendFile(tmp[4], sockArr[0], sockArr[1]);
    close(sockArr[1]);
    readServ(ctrlSoket, buf[0]);                       //читаем ответ сервера
    //read2Sockets(sockArr, (char**)buf, 1);
    cout << "buf[0]:"<<buf[0]<<endl<< "buf[1]:"<<buf[1]<<endl;
    close(ctrlSoket);  ///закрытие соединения
    cout << "Client exit"<<endl;
    return 0;
}

