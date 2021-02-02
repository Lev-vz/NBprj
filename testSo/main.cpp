

#include <cstdlib>
#include <iostream>
#include <dlfcn.h>

using namespace std;

int main(int argc, char** argv) {
    void *handle = dlopen("/home/lev/NBprj/vegaFtpClient/dist/Debug/GNU-Linux/libvegaFtpClient.so",RTLD_LAZY);
    if (!handle) {
        exit(1);
    }
    int (*fun)(char*);
    fun = (int(*)(char*))dlsym(handle,"ftp");//
    char str[256] = " Ku    rf re ku!   ";
    cout<<"str="<<str<<endl;
    int x = (*fun)(str);
    dlclose(handle);
    cout<<"x="<<x<<endl;
    return 0;
}

/**/
