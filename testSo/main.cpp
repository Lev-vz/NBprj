

#include <cstdlib>
#include <iostream>
#include <dlfcn.h>

using namespace std;

int main(int argc, char** argv) {
    if(argc < 8 || argc>9){
            cout<<"Hеправильный формат запроса."<<endl<<
            "Должно быть через побел: <IP-адрес> <порт> <логин> <пароль> <путь и имя файла-источника> [целевой путь]"<<endl<<
            "Пример: 172.16.35.97 21 testuser 1 c:/projects/kln/ttt.cfg cfg"<<endl;
        return 2;
    }
    void *handle = dlopen(argv[1],RTLD_LAZY);
    if (!handle) {
        cout<<"Не найден so-файл"<<endl;
        return 3;
    }
    int (*fun)(char*);
    fun = (int(*)(char*))dlsym(handle,"vegaFtp");//
    char str[1024];// = "172.16.35.97 21 testuser 1 /home/lev/tmp/555/3.txt 1";
    if(argc == 9) sprintf(str,"%s %s %s %s %s %s %s",argv[2],argv[3],argv[4],argv[5],argv[6],argv[7],argv[8]);
    else          sprintf(str,"%s %s %s %s %s %s",   argv[2],argv[3],argv[4],argv[5],argv[6],argv[7]);
    cout<<"Параметры вызова ftp-загрузчика: "<<str<<endl;
    int x = (*fun)(str);
    dlclose(handle);
    cout<<"ftp-загрузчик завершился с кодом "<<x<<endl;
    return 0;
}

/**/
