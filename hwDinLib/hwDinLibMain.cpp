/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <iostream>
using namespace std;

//#include <dynamic.h>

extern "C" int hello(char *str)
{
    cout<<"DLL user say: "<<str<<endl;
    return 0;
}