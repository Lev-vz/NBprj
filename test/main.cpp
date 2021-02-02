#include <iostream>
#include <string.h>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <future>
#include <vector>
#include <list>
#include <algorithm>
#include <dlfcn.h>
//#include "tools.h"

using namespace std;
bool ex = true;
/*
static string getAnswer()
{    
    string answer;
    cin >> answer;
    return answer;
}

bool inputAvailable()  
{
  timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(0, &fds);
  return select(1, &fds, nullptr, nullptr, &tv);;
}

void setArr(int *&arr, int N){
    arr = new int[N];
    arr[0] = 111;
    arr[1] = 333;
}

void * thrd(void *y){
    char *s;
    FILE *f = fopen("2.txt","wt");
    s = (char*)(y);
    void * ret = NULL;
    for(int i=0; i <10; i++){
        //s += to_string(i) + "\n";
        cout<<s<<i<<"\n";
        fputs(s,f);
        sleep(1);
        if(i==5) return ret;
    }
    fclose(f);
}
*/


int main(int argc, char** argv) {
    //void *handle = dlopen("libhwDinLib.so",RTLD_LAZY);
    //int(*fun)(void) = dlsym(handle,"hello");
    int x = 0;//(*fun)();
    //dlclose(handle);
    cout<<"Return code: "<<x<<endl;    
//    list<int> v = {11,20,40,30};
//    v.push_back(33);
//    if(v.empty()){
//        cout<<"vector empty\n";
//        return 0; 
//    }
//    auto ti = v.begin();
//    ti++;
//    v.remove(*ti);
//    for(auto it = v.begin(); it != v.end(); it++)//list<int>::iterator
//        cout<<*it<<endl;
//    auto biggest = max_element(v.begin(), v.end());
//    cout << "Max element is " << *biggest;// << " at position " << distance(begin(v), biggest) << endl;
//    
//
//    auto smallest = min_element(v.begin(), v.end());
//    cout << "min element is " << *smallest<< " at position " << distance(begin(v), smallest) << endl;    //pthread_t tid; /* идентификатор потока */
    //char x[16] = "ku-ku";
    //void* attr = (void*)(x);/* aтрибуты потока */
    
    //future<string> ftr = async(getAnswer);

    //pthread_create(&tid, NULL, thrd, attr);
    //cout<<"tid="<<tid<<"\n";
//    int i=0;
//    string s = "";
//    while(1){
//        cout<<"i="<<(i++)<<"\n";
//        sleep(3);
//        isKbInput(s);
//        //if(isKbInput()) getline(cin, s);
//        cout<<"s="<<s<<endl;
//    }
    //pthread_join(tid,NULL);
    //cout<<"attr="<<((char*)attr)<<"\n";
//    string pf;
//    cin>>pf;
//    int x = pf.find_last_of("/");
//    string fn = pf.substr(x+1);
//    cout<<"pf="<<pf<<", x="<<x<<", fn="<<fn<<"\n";
//    int *arr;
//    setArr(arr,2);
//    cout<<"x="<<arr[0]<<", y="<<arr[1]<<"\n";
    return 0;
}

