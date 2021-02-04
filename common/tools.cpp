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
#include "../common/tools.h"

using namespace std;
  
//асинхронный ввод с клавиатуры. Принимает ссылку на строку, в которую будет помещены введённые символы до символа 10.
//Возвращает true, если что-то было введено, false - если ввода не было с последнего вычитывания потока ввода с клавиатуры
bool isKbInput(string &s){
    timeval tv0;                                    //создаём структуру для таймаута функции select
    tv0.tv_sec = 0;                                 //заполняем её 0, т.к. тут просто проверяем,
    tv0.tv_usec = 0;                                //не приходило ли в поток ввода хоть что-нибудь
    
    fd_set fds;                                     //создаём пустую структуру для данных (возможно она не нужна - проверить потом) 
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    
    s = "";                                         //обнуляем строку
    if(select(1, &fds, nullptr, nullptr, &tv0)){    //проверяем, было ли что-то введено с клавиатуры. Дискриптор потока с клавиатуры - всегда 0
        while(1){                   //если что-то было введено
            char ch = getchar();        //вычитывам все символы, 
            if(ch == 10) break;         //пока не встретится символ перевода строки
            s += ch;                    //считанные символы записываем в строку
        }
        return true;
    }
    return false;
}
//функция чтения сокета с эхом
int rcv(int id, char *buf, int port){
    int len = recv(id, buf, BUFFER_SIZE - 1, 0);//вычитываем сокет в буфер
    buf[len] = '\0';                            //замыкаем строку
    cout<<port<<" port:"<<buf<<"|\n";           //выводим эхо
    return len;                                 //возвращаем длину считанной строки
}
//функция создания и заполнения структуры для таймаута
timeval setTimeOut(int s, int us){
    timeval timeout;
    timeout.tv_sec = s;   //задаём секунды
    timeout.tv_usec = 0;  //задаём микросекунд
    return timeout;
}

//функция чтения из сокета
int readServ(int socketID, char *buf, int ta) {
    timeval timeout = setTimeOut(ta,0);  //зададим  структуру времени со значением 5 сек
    fd_set readset;                     //создаём структуру для анализа функцией select
    FD_ZERO(&readset);                  //чистим её
    FD_SET(socketID, &readset);         //записываем в неё дескриптер сокета
    int selRep = select(socketID+1, &readset, nullptr, nullptr, &timeout); //поверяем, пришло ли что-то через сокет
    if(selRep < 0)                      //если ответ от сокета отрицательный
    {
        perror("select");                   //выводим сообщение об ошибке функции select
        exit(3);                            //выходим с кодом 3
    }
    //cout<<"selRep = "<<selRep<<endl;        //контрольный вывод
    selRep = FD_ISSET(socketID, &readset);  //если ошибки не было - проверяем, в нужный ли сокет пришла информация 
    //cout<<"issetRep = "<<selRep<<endl;
    if(selRep){                             // Поступили данные от клиента, читаем их
        //cout<<"Что-то пришло!"<<endl;       //контрольный вывод
        int bytes_read = recv(socketID, buf, BUFFER_SIZE-1, 0);
        buf[bytes_read]='\0';
        cout<<"buf="<<buf<<" bytes_read="<<bytes_read<<endl; //
    }
    return 0;
}
//Чтение списка сокетов 
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
//создание сокета
//Аргументы IP-адрес в виде строки, номер порта
//Возвращает идентификатор сокета
int getSocketId(char *ipAddr, int port){
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


//функция для FTP клиента - чтение двух сокетов: 0 - управление, 1 - данные
//Аргументы: массив идентификаторов сокетов, массив строк для записи сообщений от разных сокетов, таймаут в секундах
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
    char bufTmp[BUFFER_SIZE];           //создём временный буфер данных
    if(selRep > 0){                     //если в какой-то сокет что-то получено
        for(int i=0; i < 2 && sockArr[i] > 0; i++){                 //проверяем все сокеты
            int isset = FD_ISSET(sockArr[i], &readset);                 //смотрим,что пришло в i-тый сокет
            //cout<<"isset["<<i<<"] = "<<isset<<endl;
            if(isset){                                                  // Поступили данные от клиента, читаем их
                int bytes_read = recv(sockArr[i], bufTmp, BUFFER_SIZE-1, 0);
                //cout<<"Socket "<<sockArr[i]<<" send "<<bytes_read<<endl;
                if(bytes_read <= 0){                                           // Если ответ recv < 0 - соединение разорвано, удаляем сокет из множества
                    cout<<"Socket "<<sockArr[i]<<" send "<<bytes_read<<" byts and connect abort."<<endl;
                    close(sockArr[i]);
                    if(i==1) sockArr[1] = -1;                                  //если это был сокет 1 - значит был завершён поток данных от сервера
                    else exit(0);                                              //если это был сокет 0 - значит сервер завершил общение. Завершаем клиента
                    //return -sockArr[i];
                }else{                                                      //если ответ recv > 0 
                    bufTmp[bytes_read]='\0';                                    //принимаем данные
                    cout<<"socket"<<sockArr[i]<<":\n"<<bufTmp;//<<"|"<<endl;
                    //strcpy(buf[i],bufTmp);                                      //в соответствующую строку
                }
            }
            strncpy(buf[i],bufTmp,BUFFER_SIZE-1);
        }
    }
    return 0;
}

//----- Создание сокета по полученным у сервера порта для получения данных ----------
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
//пересылка файла с клиета на сервер
//Аргументы: путь к пересылаемому файлу + имя файла, идентификатор сокета управления, идентификатор сокета передачи данных
int sendFile(string pathFile, int socketID, int socketForFile){
    char buffer[BUFFER_SIZE];
    FILE *f = fopen(pathFile.c_str(), "rb");                    //открываем файл
    if(f<=0){
        cout<<"Не удалось открыть файл "<<pathFile.c_str()<<endl;
        return 1;                                            //если открытие не удалось - завершаем функцию
    } 
    
    int x = pathFile.find_last_of("/");                         //отделяем имя файла от пути
    string fn = pathFile.substr(x+1);
    
    sprintf(buffer,"STOR %s\r\n",fn.c_str());                   //формируем сообщение серверу
    send(socketID,buffer,strlen(buffer),0);                     //посылаем сообщение
    readServ(socketID, buffer, 1);
    cout<<"server answer: "<<buffer<<endl;
    //!!!! Тут дожен быть анализ ответа сервера на предмет подтверждения готовности сервера принять данные
    
    while(!feof(f)){                                            //пока не конец файла, 
        int qSimb=fread(buffer,1,BUFFER_SIZE-1,f);                    //передаем частями файл (сколько помещается в буфере)
        if(qSimb!=0){
          send(socketForFile, buffer, qSimb, 0);         //если считано ненулевое количество байт - передаём их через сокет данных  
          cout<<"send "<<qSimb<<" byte"<<endl;
          readServ(socketForFile, buffer, 1);
          readServ(socketID, buffer, 1);
        } 
    }
    return 0;
}
