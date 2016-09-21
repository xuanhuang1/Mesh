//
//  edge.cpp
//  
//
//  Created by Xuan Huang on 6/23/16.
//
//
#include "edge.h"

edge::edge(int a, int b){
    node1 = a;
    node2 = b;
    edgeRep = 0;
}

int edge::get1(){
    return node1;
}

int edge::get2(){
    return node2;
}

void edge::addRep(){
    ++edgeRep;
}

int edge::getRep(){
    return edgeRep;
}
