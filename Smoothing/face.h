//
//  face.h
//  
//
//  Created by Xuan Huang on 6/23/16.
//
//

#ifndef face_h
#define face_h

#include <vector>
#include <string>

class face{
    int numOfV;
    double aspectR, det;
    std::vector<int> listOfV;
public:
    face(std::vector<int>);
    std::vector<int> getList();
    int hasV(int);
    std::string listToS();
    void setAR(double);
    double getAR();
    void setDet(double);
    double getDet();
};



#endif /* face_h */
