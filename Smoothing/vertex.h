//
//  vertex.h
//  
//
//  Created by Xuan Huang on 6/23/16.
//
//

#ifndef vertex_h
#define vertex_h


class vertex{
    double x,y,z;
    int num;
    int bound;
public:
    vertex(double, double,double);
    double getX();
    double getY();
    double getZ();
    int getNum();
    int ifBound();
    void setX(double);
    void setY(double);
    void setZ(double);
    void setNum(int);
    void onBound();
};

#endif /* vertex_h */
