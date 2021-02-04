

#include <cstdlib>
#include <iostream>
#include <dlfcn.h>

using namespace std;

int main(int argc, char** argv) {
    if(argc < 7 || argc>8){
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
    if(argc == 8) sprintf(str,"%s %s %s %s %s %s",argv[2],argv[3],argv[4],argv[5],argv[6],argv[7]);
    else          sprintf(str,"%s %s %s %s %s",   argv[2],argv[3],argv[4],argv[5],argv[6]);
    cout<<"str="<<str<<endl;
    int x = (*fun)(str);
    dlclose(handle);
    cout<<"x="<<x<<endl;
    return 0;
}

/**/
