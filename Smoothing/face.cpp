//
//  face.cpp
//  
//
//  Created by Xuan Huang on 6/23/16.
//
//

#include "face.h"

face::face(std::vector<int> list){
    for(int i=0; i<list.size();i++){
        listOfV.push_back(list[i]);
    }
}

std::vector<int> face::getList(){
    return listOfV;
}

int face::hasV(int v){
    for (int i=0; i<listOfV.size();i++) {
        if(listOfV[i]== v){
            return i;
        }
    }
    return -1;
}

std::string face::listToS(){
    std::string str;
    for (int i=0; i<listOfV.size();i++) {
        str += std::to_string(listOfV[i]) + ", ";
    }
    return str;
}

void face::setAR(double ar){
    aspectR = ar;
}

double face::getAR(){
    return aspectR;
}

void face::setDet(double d){
    det = d;
}

double face::getDet(){
    return det;
}