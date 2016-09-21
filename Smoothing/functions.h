//
//  functions.hpp
//  
//
//  Created by Xuan Huang on 6/28/16.
//
//

#ifndef functions_h
#define functions_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include "vertex.h"
#include "edge.h"
#include "face.h"

using namespace std;

int ifduplicate(int tobetest, vector<int> list);
double dif(vector<vertex> &v1, vector<vertex> &v2);
void smooth(vector<vertex> &, vector<face> &, double ar);
void smoothQ(vector<vertex> &, vector<face> &, double ar);
void smoothLapAng(vector<vertex> &, vector<face> &);
void smoothN(vector<vertex> &, vector<face> &, double ar);
void smoothNQ(vector<vertex> &, vector<face> &, double ar);

int insideF(face, vector<vertex>);

//bool sortByAr(vector<vertex>, face, face);
double sortByDet(face &f1, face &f2);
void maxminAng(vector<vertex> , vector<face> , double &, double &);
double aspectR(vector<vertex> , vector<face> &, double &);
double GetCircumCenterX(double Ax, double Ay, double Bx, double By, double Cx, double Cy);
double GetCircumCenterY(double Ax, double Ay, double Bx, double By, double Cx, double Cy);
double movePX(double Ax, double Ay, double cenX, double cenY, double ang);
double movePY(double Ax, double Ay, double cenX, double cenY, double ang);


struct Vertex{
    double x;
    double y;
    double z;
};

struct CREAnalysis{
    double AR;
    double skew;
    double taperX;
    double taperY;
    double jacobianDet;
    double modifiedDet;
};

CREAnalysis analyze(Vertex vertices[4]);



#endif /* functions_h */
