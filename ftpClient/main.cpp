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

char addr[16] = "127.0.0.1"; //по умолчанию - локальный сервер

//идентификация. Для тестирования имена задаются прямо в ней. Потом заменить на аргументы
  bool login(int socketId, char *buf) {
    cout << "Введите имя: "; char name[64] = "testuser";// cin >> name;
    sprintf(buf,"USER %s\r\n",name);                        //формируем логин для сервера
    send(socketId,buf,strlen(buf),0);                       //отправляем логин
    readServ(socketId,buf);                                 //читаем ответ сервера. Пока ника не используем
    cout << "Введите пароль: "; char pass[64] = "1";// cin >> pass;
    sprintf(buf,"PASS %s\r\n",pass);                        //формируем пароль для сервера
    send(socketId,buf,strlen(buf),0);                       //отправляем пароль
    readServ(socketId, buf);                                //читаем ответ сервера
    
    string tmp = buf;
    if(tmp.substr(0,3)=="230") return true;                 //если логин/пероль прошли - возвращаем true
    return false;                                           //если н прошли -0 возвращаем false
}
 

int main(void)
{
    char buf[2][BUFFER_SIZE];                               //создаём массив строк для ответов через сокеты
    int ctrlSoket = getSocketId(addr, 21);                  //создаём сокет управляющего канала
    readServ(ctrlSoket, buf[0]);                            //вычитываем реакцию сервера на установление связи
    
    //!!! тут должна быть проверка реакции сервера

    while(!login(ctrlSoket, buf[0]));                       //логинимся с сервером, пока не залогинимся - функция login должна вернуть true

    string kbInput = "";                                    //заводим строку для клавиатурного ввода
    int sockArr[2] = {ctrlSoket, -1};                       //создаём массив из двух сокетов
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
