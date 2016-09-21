//
//  vertex.cpp
//
//
//  Created by Xuan Huang on 6/23/16.
//
//

#include "vertex.h"

vertex::vertex(double a, double b, double c){
    x = a;
    y = b;
    z = c;
    bound = 0;// 0 not onbound, 1 onbound
}

double vertex::getX(){return x;}
double vertex::getY(){return y;}
double vertex::getZ(){return z;}
int vertex::getNum(){return num;}
int vertex::ifBound(){return bound;}

void vertex::setX(double xi){x = xi;}
void vertex::setY(double yi){y = yi;}
void vertex::setZ(double zi){z = zi;}
void vertex::setNum(int numi){num = numi;}
void vertex::onBound(){bound = 1;}