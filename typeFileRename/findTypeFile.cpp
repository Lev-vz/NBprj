#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <fstream>
#include <string>

#include "findTypeName.h"

// <editor-fold defaultstate="collapsed" desc="comment">
int findTypeFile(){
    DIR *dir = opendir(".");
    if(dir){
        struct dirent *ent;
        while((ent = readdir(dir)) != NULL){
            std::string fileName = ent->d_name;
            printf("%s\n", fileName.c_str()); // Контрольный вывод -------------------------------
            int x = fileName.rfind(".type");
            if(!(fileName.length()-x-5)){
                std::ifstream file(ent->d_name);
                if(file){
                    std::string line;
                    std::string typeName;
                    while (getline(file, line)){
                        typeName = findTypeName(line);
                        if(typeName != ""){
                            typeName += ".type";
                            printf("%s -> %s\n", fileName.c_str(), typeName.c_str());
                          break;
                        }
                    }
                    file.close();     // закрываем файл
                    if(typeName != ""){
                        rename(ent->d_name, typeName.c_str());
                    }
                }
            }
        }
    }else{
        fprintf(stderr, "Error opening directory\n");
    }
    return 0;
}

