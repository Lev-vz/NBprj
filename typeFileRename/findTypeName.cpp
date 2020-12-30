/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <string>

std::string   findTypeName(std::string s1){
    std::string line;
    int x = s1.find("Type Name");
    if(x>0){
        int y1 = s1.find("\"",x);
        int y2 = s1.find("\"",y1+1);
        if(y1+1 < y2) return s1.substr(y1+1,y2-y1-1);
        return "";
    }
    return "";
}