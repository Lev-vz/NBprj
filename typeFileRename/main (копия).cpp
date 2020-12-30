/* 
 * File:   main.cpp
 * Author: lev
 *
 * Created on 18 февраля 2020 г., 11:57
 */

#include <iostream>
#include <fstream>
#include <string>

#include "findTypeName.h"
#include "findTypeFile.h"

using namespace std;

int main()
{
    findTypeFile();
    
    ifstream file; // создаем объект класса ifstream
    file.open("/home/lev/NetBeansProjects/typeFileRename/dist/Debug/GNU-Linux/Type_XTY273TAYAJUBLUA5ROYRWX5WU.type"); // открываем файл
    if (!file){
        cout << "Файл не открыт\n\n"; 
        return -1;
    }

    std::string line;
    std::string line2;
    line2 = "Papa u vasi silen v matematike";
    
    while (getline(file, line))
    {
        //std::cout << line << std::endl;
        line2 = findTypeName(line);
        if(line2 != ""){
          std::cout << line2 << std::endl;  
        }
    }
    
    file.close();     // закрываем файл
     
    //std::cout << findTypeName(line2) << std::endl;
}


