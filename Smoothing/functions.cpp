//
//  functions.cpp
//
//
//  Created by Xuan Huang on 6/28/16.
//
//

#include "functions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


int ifduplicate(int tobetest, vector<int> list){
    for (int q=0; q<list.size(); q++) {
        if (list[q] == tobetest) {
            return 1;
        }
    }
    return 0;
}

double dif(vector<vertex> &v1, vector<vertex> &v2){
    double val = 0;
    for (int i=0; i<v1.size(); ++i) {
        val += abs(v1[i].getX()+v1[i].getY() - v2[i].getX() - v2[i].getY());
    }
    return val;
}

int insideF(face fa, vector<vertex> v){
    for (int i =0; i<fa.getList().size(); i++){
        if (v[fa.getList()[i]].ifBound() == 1)
            return 1;
    }
    return 0;
}


void smooth(vector<vertex> &v, vector<face> &f, double ar){
    //cout << "smoothing ";
    for(int i=0; i<f.size();i++){
        double min, max, vecx, vecy;
        double thisEdge, tempAR;
        int self, next, last, ver1, ver2;
        for (int j=0; j<f[i].getList().size(); ++j) { // for each vertex in this face, measure the edge it connects with the next vertex
            //cout << i<<"\n";
            if (j == 0) {
                self = f[i].getList()[j];
                next = f[i].getList()[j+1];
            }else if(j==f[i].getList().size()-1){
                self = f[i].getList()[j];
                next = f[i].getList()[0];
            }else{
                self = f[i].getList()[j];
                next = f[i].getList()[j+1];
            }
            
            vecx = v[next].getX() - v[self].getX();
            vecy = v[next].getY() - v[self].getY();
            
            thisEdge = sqrt(pow(vecx,2)+pow(vecy,2));
            
            if(j==0){
                max = thisEdge;
                min = thisEdge;
                ver1 = self;
                ver2 = next;
                last = f[i].getList()[f[i].getList().size()-1];
            }else{
                if (thisEdge>max) {
                    max = thisEdge;
                }
                if (thisEdge<min) {
                    min = thisEdge;
                    ver1 = self;
                    ver2 = next;
                    if(j==0){
                        last = f[i].getList()[f[i].getList().size()-1];
                    }else{
                        last =f[i].getList()[j-1];
                    }
                }
            }
        }// end of all edges in the face
        tempAR = max/min;
        
        if(tempAR > ar){
            /*cout << ver1<<" "<< ver2 << " "<< last<< " lk"<< "\n";
             cout << v[ver1].getX()<<" "<< v[ver2].getX() << " "<< v[last].getX()<< " lk"<< "\n";
             cout << v[ver1].getY()<<" "<< v[ver2].getY() << " "<< v[last].getY()<< " lk"<< "\n";*/
            
            if(v[ver1].ifBound() == 0 || v[ver2].ifBound() == 0){
                //cout << ver1<<" "<< ver2 << " "<< last<< " tempAR > ar"<< "\n";
                double pdist = sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2));
                //cout<< "pdist "<<pdist<<"\n";
                double angle = -0.03;
                double centerX = GetCircumCenterX(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[last].getX(), v[last].getY());
                double centerY = GetCircumCenterY(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[last].getX(), v[last].getY());
                
                if(v[ver1].ifBound() == 0 && v[ver2].ifBound() == 0){
                    
                    //cout << "pdist " << pdist << " centerX " << centerX<< " centerY " << centerY<<"\n";
                    while(sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2)) < (100*pdist)/99){
                        v[ver1].setX(movePX(v[ver1].getX(), v[ver1].getY(), centerX, centerY, angle));
                        v[ver1].setY(movePY(v[ver1].getX(), v[ver1].getY(), centerX, centerY, angle));
                        v[ver2].setX(movePX(v[ver2].getX(), v[ver2].getY(), centerX, centerY, 0-angle));
                        v[ver2].setY(movePY(v[ver2].getX(), v[ver2].getY(), centerX, centerY, 0-angle));
                        //cout << "&& " <<endl;
                    }
                }else if(v[ver1].ifBound() == 0){
                    while(sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2)) < (100*pdist)/99){
                        v[ver1].setX(movePX(v[ver1].getX(), v[ver1].getY(), centerX, centerY, angle));
                        v[ver1].setY(movePY(v[ver1].getX(), v[ver1].getY(), centerX, centerY, angle));
                        //cout << "ver1 " <<endl;
                    }
                }else if(v[ver2].ifBound() == 0){
                    while(sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2)) < (100*pdist)/99){
                        v[ver2].setX(movePX(v[ver2].getX(), v[ver2].getY(), centerX, centerY, 0-angle));
                        v[ver2].setY(movePY(v[ver2].getX(), v[ver2].getY(), centerX, centerY, 0-angle));
                        //cout << "ver2 " <<endl;
                    }
                }
            }
        }
    }
    
    
}
void smoothQ(vector<vertex> &v, vector<face> &f, double ar){// move shortest edge
    for(int i=0; i<f.size();i++){
        double min, max, vecx, vecy;
        double thisEdge, tempAR;
        int self, next, last, next2, ver1, ver2;
        int vlist[4];
        for (int j=0; j<f[i].getList().size(); ++j) { // for each vertex in this face, measure the edge it connects with the next vertex
            //cout << i<<"\n";
            vlist[j] = f[i].getList()[j];
            
            if (j == 0) {
                self = f[i].getList()[j];
                next = f[i].getList()[j+1];
            }else if(j==f[i].getList().size()-1){
                self = f[i].getList()[j];
                next = f[i].getList()[0];
            }else{
                self = f[i].getList()[j];
                next = f[i].getList()[j+1];
            }
            
            vecx = v[next].getX() - v[self].getX();
            vecy = v[next].getY() - v[self].getY();
            
            thisEdge = sqrt(pow(vecx,2)+pow(vecy,2));
            
            if(j==0){
                max = thisEdge;
                min = thisEdge;
                ver1 = self;
                ver2 = next;
                last = f[i].getList()[f[i].getList().size()-1];
                next2 = f[i].getList()[1];
            }else{
                if (thisEdge>max) {
                    max = thisEdge;
                }
                if (thisEdge<min) {
                    min = thisEdge;
                    ver1 = self;
                    ver2 = next;
                    if(j==0){
                        last = f[i].getList()[f[i].getList().size()-1];
                    }else{
                        last =f[i].getList()[j-1];
                    }
                    next2 = f[i].getList()[(j+1)%(f[i].getList().size())];
                }
            }
        }// end of all edges in the face
        tempAR = max/min;
        
        if(tempAR > ar){
            /*cout << ver1<<" "<< ver2 << " "<< last<< " lk"<< "\n";
             cout << v[ver1].getX()<<" "<< v[ver2].getX() << " "<< v[last].getX()<< " lk"<< "\n";
             cout << v[ver1].getY()<<" "<< v[ver2].getY() << " "<< v[last].getY()<< " lk"<< "\n";*/
            
            if(v[ver1].ifBound() == 0 || v[ver2].ifBound() == 0){
                //cout << ver1<<" "<< ver2 << " "<< last<< " tempAR > ar"<< "\n";
                double pdist = sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2));
                //cout<< "pdist "<<pdist<<"\n";
                double angle = -0.03;
                double centerX = GetCircumCenterX(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[last].getX(), v[last].getY());
                double centerY = GetCircumCenterY(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[last].getX(), v[last].getY());
                double centerX2 = GetCircumCenterX(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[next2].getX(), v[next2].getY());
                double centerY2 = GetCircumCenterY(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[next2].getX(), v[next2].getY());
                //cout << "next2: " <<next2 <<endl;
                
                if(v[ver1].ifBound() == 0 && v[ver2].ifBound() == 0){
                    
                    //cout << "pdist " << pdist << " centerX " << centerX<< " centerY " << centerY<<"\n";
                    while(sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2)) < (100*pdist)/99){
                        v[ver1].setX(movePX(v[ver1].getX(), v[ver1].getY(), centerX, centerY, angle));
                        v[ver1].setY(movePY(v[ver1].getX(), v[ver1].getY(), centerX, centerY, angle));
                        v[ver2].setX(movePX(v[ver2].getX(), v[ver2].getY(), centerX, centerY, 0-angle));
                        v[ver2].setY(movePY(v[ver2].getX(), v[ver2].getY(), centerX, centerY, 0-angle));
                        v[ver1].setX(movePX(v[ver1].getX(), v[ver1].getY(), centerX2, centerY2, angle));
                        v[ver1].setY(movePY(v[ver1].getX(), v[ver1].getY(), centerX2, centerY2, angle));
                        v[ver2].setX(movePX(v[ver2].getX(), v[ver2].getY(), centerX2, centerY2, 0-angle));
                        v[ver2].setY(movePY(v[ver2].getX(), v[ver2].getY(), centerX2, centerY2, 0-angle));
                        //cout << "&& " <<endl;
                        //cout <<" " <<sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2)) <<" "<< (100*pdist)/99<<endl;
                    }
                }else if(v[ver1].ifBound() == 0){
                    while(sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2)) < (100*pdist)/99){
                        v[ver1].setX(movePX(v[ver1].getX(), v[ver1].getY(), centerX, centerY, angle));
                        v[ver1].setY(movePY(v[ver1].getX(), v[ver1].getY(), centerX, centerY, angle));
                        v[ver1].setX(movePX(v[ver1].getX(), v[ver1].getY(), centerX2, centerY2, angle));
                        v[ver1].setY(movePY(v[ver1].getX(), v[ver1].getY(), centerX2, centerY2, angle));
                        //cout << "ver1 " <<endl;
                    }
                }else if(v[ver2].ifBound() == 0){
                    while(sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2)) < (100*pdist)/99){
                        v[ver2].setX(movePX(v[ver2].getX(), v[ver2].getY(), centerX, centerY, 0-angle));
                        v[ver2].setY(movePY(v[ver2].getX(), v[ver2].getY(), centerX, centerY, 0-angle));
                        v[ver2].setX(movePX(v[ver2].getX(), v[ver2].getY(), centerX2, centerY2, 0-angle));
                        v[ver2].setY(movePY(v[ver2].getX(), v[ver2].getY(), centerX2, centerY2, 0-angle));
                        //cout << "ver2 " <<endl;
                    }
                }
            }
        }
    }
}

void smoothNQ(vector<vertex> &v, vector<face> &f, double ar){
    for(int i=0; i<f.size();i++){
        double min, max, vecx, vecy;
        double thisEdge, tempAR;
        int self, next, lastS, nextS, lastL, nextL, ver1, ver2, ver3, ver4;
        for (int j=0; j<f[i].getList().size(); ++j) { // for each vertex in this face, measure the edge it connects with the next vertex
            //cout << i<<"\n";
            if (j == 0) {
                self = f[i].getList()[j];
                next = f[i].getList()[j+1];
            }else if(j==f[i].getList().size()-1){
                self = f[i].getList()[j];
                next = f[i].getList()[0];
            }else{
                self = f[i].getList()[j];
                next = f[i].getList()[j+1];
            }
            
            vecx = v[next].getX() - v[self].getX();
            vecy = v[next].getY() - v[self].getY();
            
            thisEdge = sqrt(pow(vecx,2)+pow(vecy,2));
            
            if(j==0){
                max = thisEdge;
                min = thisEdge;
                ver1 = self; // shortest
                ver2 = next;
                ver3 = self; // longest
                ver4 = next;
                lastS = f[i].getList()[f[i].getList().size()-1];
                nextS = f[i].getList()[1];
                lastL = f[i].getList()[f[i].getList().size()-1];
                nextL = f[i].getList()[1];
            }else{
                if (thisEdge>max) {
                    max = thisEdge;
                    ver3 = self;
                    ver4 = next;
                    lastL = f[i].getList()[(j-1)%f[i].getList().size()];
                    nextL = f[i].getList()[(j+1)%f[i].getList().size()];
                }
                if (thisEdge<min) {
                    min = thisEdge;
                    ver1 = self;
                    ver2 = next;
                    if(j==0){
                        lastS = f[i].getList()[f[i].getList().size()-1];
                    }else{
                        lastS =f[i].getList()[j-1];
                    }
                    nextS = f[i].getList()[(j+1)%f[i].getList().size()];
                }
            }
        }// end of all edges in the face
        tempAR = max/min;
        
        
        if(tempAR > ar){
            /*cout << ver1<<" "<< ver2 << " "<< last<< " lk"<< "\n";
             cout << v[ver1].getX()<<" "<< v[ver2].getX() << " "<< v[last].getX()<< " lk"<< "\n";
             cout << v[ver1].getY()<<" "<< v[ver2].getY() << " "<< v[last].getY()<< " lk"<< "\n";*/
            int verToMove;
            
            if(ver1 == ver4)//both ccw
            {
                verToMove = ver1;// Lo 3 4=1 2 Sh
                if(v[verToMove].ifBound() == 0){
                    //cout << ver1<<" "<< ver2 << " "<< last<< " tempAR > ar"<< "\n";
                    double pdist = sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2));
                    //cout<< "pdist "<<pdist<<"\n";
                    double angle = -0.03;
                    double centerX = GetCircumCenterX(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[ver3].getX(), v[ver3].getY());
                    double centerY = GetCircumCenterY(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[ver3].getX(), v[ver3].getY());
                    //cout << "pdist " << pdist << " centerX " << centerX<< " centerY " << centerY<<"\n";
                    while(sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2)) < (100*pdist)/99){
                        v[verToMove].setX(movePX(v[verToMove].getX(), v[verToMove].getY(), centerX, centerY, angle));
                        v[verToMove].setY(movePY(v[verToMove].getX(), v[verToMove].getY(), centerX, centerY, angle));
                        //v[verToMove].setX(movePX(v[verToMove].getX(), v[verToMove].getY(), centerX2, centerY2, angle));
                        //v[verToMove].setY(movePY(v[verToMove].getX(), v[verToMove].getY(), centerX2, centerY2, angle));
                        
             
                    }
                }else if(v[ver2].ifBound() == 0){
                    verToMove = ver2;
                    //cout << ver1<<" "<< ver2 << " "<< last<< " tempAR > ar"<< "\n";
                    double pdist = sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2));
                    //cout<< "pdist "<<pdist<<"\n";
                    double angle = 0.03;
                    double centerX = GetCircumCenterX(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[ver3].getX(), v[ver3].getY());
                    double centerY = GetCircumCenterY(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[ver3].getX(), v[ver3].getY());
                    //cout << "pdist " << pdist << " centerX " << centerX<< " centerY " << centerY<<"\n";
                    while(sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2)) < (100*pdist)/99){
                        v[verToMove].setX(movePX(v[verToMove].getX(), v[verToMove].getY(), centerX, centerY, angle));
                        v[verToMove].setY(movePY(v[verToMove].getX(), v[verToMove].getY(), centerX, centerY, angle));
                        //v[verToMove].setX(movePX(v[verToMove].getX(), v[verToMove].getY(), centerX2, centerY2, angle));
                        //v[verToMove].setY(movePY(v[verToMove].getX(), v[verToMove].getY(), centerX2, centerY2, angle));
                        //cout << ".ver1=4, 1"<<endl;
                    }
                }else if(v[ver3].ifBound() == 0){
                    verToMove = ver3;
                    //cout << ver1<<" "<< ver2 << " "<< last<< " tempAR > ar"<< "\n";
                    double pdist = sqrt(pow(v[ver3].getX()-v[ver4].getX(), 2) + pow(v[ver3].getY()-v[ver4].getY(), 2));
                    //cout<< "pdist "<<pdist<<"\n";
                    double angle = 0.03;
                    double centerX = GetCircumCenterX(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[ver3].getX(), v[ver3].getY());
                    double centerY = GetCircumCenterY(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[ver3].getX(), v[ver3].getY());
                    //cout << "pdist " << pdist << " centerX " << centerX<< " centerY " << centerY<<"\n";
                    while(sqrt(pow(v[ver3].getX()-v[ver4].getX(), 2) + pow(v[ver3].getY()-v[ver4].getY(), 2)) > (99*pdist)/100){
                        v[verToMove].setX(movePX(v[verToMove].getX(), v[verToMove].getY(), centerX, centerY, angle));
                        v[verToMove].setY(movePY(v[verToMove].getX(), v[verToMove].getY(), centerX, centerY, angle));
                        //v[verToMove].setX(movePX(v[verToMove].getX(), v[verToMove].getY(), centerX2, centerY2, angle));
                        //v[verToMove].setY(movePY(v[verToMove].getX(), v[verToMove].getY(), centerX2, centerY2, angle));
                        //cout << ".ver1=4, 2 "<<sqrt(pow(v[ver3].getX()-v[ver4].getX(), 2) + pow(v[ver3].getY()-v[ver4].getY(), 2))<<" "<< (99*pdist)/100<<endl;
                    }
                }
                //cout << "1 "<<endl;
            }
            
            
            else if(ver2 == ver3)
            {
                //cout << "2 "<<endl;
                verToMove = ver3;// Sh 1 2=3 4 Lo
                if(v[verToMove].ifBound() == 0){
                    //cout << ver1<<" "<< ver2 << " "<< last<< " tempAR > ar"<< "\n";
                    double pdist = sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2));
                    //cout<< "pdist "<<pdist<<"\n";
                    double angle = 0.03;
                    double centerX = GetCircumCenterX(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[ver4].getX(), v[ver4].getY());
                    double centerY = GetCircumCenterY(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[ver4].getX(), v[ver4].getY());
                    //cout << "pdist " << pdist << " centerX " << centerX<< " centerY " << centerY<<"\n";
                    while(sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2)) < (100*pdist)/99){
                        v[verToMove].setX(movePX(v[verToMove].getX(), v[verToMove].getY(), centerX, centerY, angle));
                        v[verToMove].setY(movePY(v[verToMove].getX(), v[verToMove].getY(), centerX, centerY, angle));
                        
                        
                        //cout << ".ver2=3"<<endl;
                    }
                }else if(v[ver1].ifBound() == 0){
                    verToMove = ver1;
                    //cout << ver1<<" "<< ver2 << " "<< last<< " tempAR > ar"<< "\n";
                    double pdist = sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2));
                    //cout<< "pdist "<<pdist<<"\n";
                    double angle = -0.03;
                    double centerX = GetCircumCenterX(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[ver4].getX(), v[ver4].getY());
                    double centerY = GetCircumCenterY(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[ver4].getX(), v[ver4].getY());
                    //cout << "pdist " << pdist << " centerX " << centerX<< " centerY " << centerY<<"\n";
                    while(sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2)) < (100*pdist)/99){
                        v[verToMove].setX(movePX(v[verToMove].getX(), v[verToMove].getY(), centerX, centerY, angle));
                        v[verToMove].setY(movePY(v[verToMove].getX(), v[verToMove].getY(), centerX, centerY, angle));
                        //cout << ".ver2=3, 1"<<endl;
                    }
                }else if(v[ver4].ifBound() == 0){
                    verToMove = ver4;
                    //cout << ver1<<" "<< ver2 << " "<< last<< " tempAR > ar"<< "\n";
                    double pdist = sqrt(pow(v[ver3].getX()-v[ver4].getX(), 2) + pow(v[ver3].getY()-v[ver4].getY(), 2));
                    //cout << ".ver2=3, 2 " << sqrt(pow(v[ver3].getX()-v[ver4].getX(), 2) + pow(v[ver3].getY()-v[ver4].getY(), 2))<<  " "<<(99*pdist)/100 <<endl;
                    //cout<< "pdist "<<pdist<<"\n";
                    double angle = 0.03;
                    double centerX = GetCircumCenterX(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[ver4].getX(), v[ver4].getY());
                    double centerY = GetCircumCenterY(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[ver4].getX(), v[ver4].getY());
                    //cout << "pdist " << pdist << " centerX " << centerX<< " centerY " << centerY<<"\n";
                    while(         sqrt(pow(v[ver3].getX()-v[ver4].getX(), 2) + pow(v[ver3].getY()-v[ver4].getY(), 2)) > (99*pdist)/100){
                        v[verToMove].setX(movePX(v[verToMove].getX(), v[verToMove].getY(), centerX, centerY, angle));
                        v[verToMove].setY(movePY(v[verToMove].getX(), v[verToMove].getY(), centerX, centerY, angle));
                        //cout << ".ver2=3, 2 " << sqrt(pow(v[ver3].getX()-v[ver4].getX(), 2) + pow(v[ver3].getY()-v[ver4].getY(), 2)) <<  " "<<(99*pdist)/100<<endl;
                    }
                }
            }else{// 1 2 3 4
                if(v[ver1].ifBound() == 0 || v[ver2].ifBound() == 0){
                    //cout << ver1<<" "<< ver2 << " tempAR > ar"<< "\n";
                    double pdist = sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2));
                    //cout<< "not conn 1"<<"\n";
                    double angle = -0.03;
                    double centerX = GetCircumCenterX(v[ver4].getX(), v[ver4].getY(), v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY());
                    double centerY = GetCircumCenterY(v[ver4].getX(), v[ver4].getY(), v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY());
                    double centerX2 = GetCircumCenterX(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[ver3].getX(), v[ver3].getY());
                    double centerY2 = GetCircumCenterY(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[ver3].getX(), v[ver3].getY());
                    //cout << "next2: " <<next2 <<endl;
                    
                    /*if(v[ver1].ifBound() == 0 && v[ver2].ifBound() == 0){
                        
                        //cout << "pdist " << pdist << " centerX " << centerX<< " centerY " << centerY<<"\n";
                        while(sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2)) < (100*pdist)/99){
                            v[ver1].setX(movePX(v[ver1].getX(), v[ver1].getY(), centerX, centerY, angle));
                            v[ver1].setY(movePY(v[ver1].getX(), v[ver1].getY(), centerX, centerY, angle));
                            v[ver2].setX(movePX(v[ver2].getX(), v[ver2].getY(), centerX, centerY, 0-angle));
                            v[ver2].setY(movePY(v[ver2].getX(), v[ver2].getY(), centerX, centerY, 0-angle));
                            v[ver1].setX(movePX(v[ver1].getX(), v[ver1].getY(), centerX2, centerY2, angle));
                            v[ver1].setY(movePY(v[ver1].getX(), v[ver1].getY(), centerX2, centerY2, angle));
                            v[ver2].setX(movePX(v[ver2].getX(), v[ver2].getY(), centerX2, centerY2, 0-angle));
                            v[ver2].setY(movePY(v[ver2].getX(), v[ver2].getY(), centerX2, centerY2, 0-angle));
                            //cout << "&& " <<endl;
                            //cout <<" " <<sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2)) <<" "<< (100*pdist)/99<<endl;
                        }
                    }else if(v[ver1].ifBound() == 0){
                        while(sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2)) < (100*pdist)/99){
                            v[ver1].setX(movePX(v[ver1].getX(), v[ver1].getY(), centerX, centerY, angle));
                            v[ver1].setY(movePY(v[ver1].getX(), v[ver1].getY(), centerX, centerY, angle));
                            v[ver1].setX(movePX(v[ver1].getX(), v[ver1].getY(), centerX2, centerY2, angle));
                            v[ver1].setY(movePY(v[ver1].getX(), v[ver1].getY(), centerX2, centerY2, angle));
                            //cout << "ver1 " <<endl;
                        }
                    }else if(v[ver2].ifBound() == 0){
                        while(sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2)) < (100*pdist)/99){
                            v[ver2].setX(movePX(v[ver2].getX(), v[ver2].getY(), centerX, centerY, 0-angle));
                            v[ver2].setY(movePY(v[ver2].getX(), v[ver2].getY(), centerX, centerY, 0-angle));
                            v[ver2].setX(movePX(v[ver2].getX(), v[ver2].getY(), centerX2, centerY2, 0-angle));
                            v[ver2].setY(movePY(v[ver2].getX(), v[ver2].getY(), centerX2, centerY2, 0-angle));
                            //cout << "ver2 " <<endl;
                        }
                    }*/
                //}
                
               /* if(v[ver3].ifBound() == 0 || v[ver4].ifBound() == 0){
                    //cout << ver1<<" "<< ver2 << " "<< last<< " tempAR > ar"<< "\n";
                    double pdist = sqrt(pow(v[ver3].getX()-v[ver4].getX(), 2) + pow(v[ver3].getY()-v[ver4].getY(), 2));
                    //cout<< "not conn 2"<<"\n";
                    //cout<< "pdist "<<pdist<<"\n";
                    double angle = 0.03;
                    double centerX = GetCircumCenterX(v[ver2].getX(), v[ver2].getY(), v[ver3].getX(), v[ver3].getY(), v[ver4].getX(), v[ver4].getY());
                    double centerY = GetCircumCenterY(v[ver2].getX(), v[ver2].getY(), v[ver3].getX(), v[ver3].getY(), v[ver4].getX(), v[ver4].getY());
                    double centerX2 = GetCircumCenterX(v[ver3].getX(), v[ver3].getY(), v[ver4].getX(), v[ver4].getY(), v[ver1].getX(), v[ver1].getY());
                    double centerY2 = GetCircumCenterY(v[ver3].getX(), v[ver3].getY(), v[ver4].getX(), v[ver4].getY(), v[ver1].getX(), v[ver1].getY());
                    //cout << "next2: " <<next2 <<endl;
                    //cout<< "before if"<<"\n";
                    if(v[ver3].ifBound() == 0 && v[ver4].ifBound() == 0){
                        
                        //cout << "in && " <<"\n";
                        while(sqrt(pow(v[ver3].getX()-v[ver4].getX(), 2) + pow(v[ver3].getY()-v[ver4].getY(), 2)) > (99*pdist)/100){
                            v[ver3].setX(movePX(v[ver3].getX(), v[ver3].getY(), centerX, centerY, angle));
                            v[ver3].setY(movePY(v[ver3].getX(), v[ver3].getY(), centerX, centerY, angle));
                            v[ver4].setX(movePX(v[ver4].getX(), v[ver4].getY(), centerX, centerY, 0-angle));
                            v[ver4].setY(movePY(v[ver4].getX(), v[ver4].getY(), centerX, centerY, 0-angle));
                            v[ver3].setX(movePX(v[ver3].getX(), v[ver3].getY(), centerX2, centerY2, angle));
                            v[ver3].setY(movePY(v[ver3].getX(), v[ver3].getY(), centerX2, centerY2, angle));
                            v[ver4].setX(movePX(v[ver4].getX(), v[ver4].getY(), centerX2, centerY2, 0-angle));
                            v[ver4].setY(movePY(v[ver4].getX(), v[ver4].getY(), centerX2, centerY2, 0-angle));
                            //cout << "&& " <<endl;
                            //cout <<" " <<sqrt(pow(v[ver3].getX()-v[ver4].getX(), 2) + pow(v[ver3].getY()-v[ver4].getY(), 2)) <<" "<< (99*pdist)/100<<endl;
                        }
                    }else if(v[ver3].ifBound() == 0){
                        //cout << "in ver3 " <<"\n";
                        //cout <<" " <<sqrt((pow(v[ver3].getX()-v[ver4].getX(), 2) + pow(v[ver3].getY()-v[ver4].getY(), 2))) <<" "<< (99*pdist)/100<<endl;
                        while(sqrt(pow(v[ver3].getX()-v[ver4].getX(), 2) + pow(v[ver3].getY()-v[ver4].getY(), 2)) > (99*pdist)/100){
                            v[ver3].setX(movePX(v[ver3].getX(), v[ver3].getY(), centerX, centerY, angle));
                            v[ver3].setY(movePY(v[ver3].getX(), v[ver3].getY(), centerX, centerY, angle));
                            v[ver3].setX(movePX(v[ver3].getX(), v[ver3].getY(), centerX2, centerY2, angle));
                            v[ver3].setY(movePY(v[ver3].getX(), v[ver3].getY(), centerX2, centerY2, angle));
                            
                            //cout << "ver3 " <<endl;
                            //cout <<" " <<sqrt((pow(v[ver3].getX()-v[ver4].getX(), 2) + pow(v[ver3].getY()-v[ver4].getY(), 2))) <<" "<< (99*pdist)/100<<endl;
                        }
                    }else if(v[ver4].ifBound() == 0){
                        //cout << "in ver4 " <<"\n";
                        while(sqrt(pow(v[ver3].getX()-v[ver4].getX(), 2) + pow(v[ver3].getY()-v[ver4].getY(), 2)) > (99*pdist)/100){
                            v[ver4].setX(movePX(v[ver4].getX(), v[ver4].getY(), centerX, centerY, 0-angle));
                            v[ver4].setY(movePY(v[ver4].getX(), v[ver4].getY(), centerX, centerY, 0-angle));
                            v[ver4].setX(movePX(v[ver4].getX(), v[ver4].getY(), centerX2, centerY2, 0-angle));
                            v[ver4].setY(movePY(v[ver4].getX(), v[ver4].getY(), centerX2, centerY2, 0-angle));
                            //cout << "ver4 " <<endl;
                        }
                    }
                }*/
                }
            }
        }
    }
}



void smoothN(vector<vertex> &v, vector<face> &f, double ar){
    for(int i=0; i<f.size();i++){
        double min, max, vecx, vecy;
        double thisEdge, tempAR;
        int self, next, last, ver1, ver2, ver3, ver4;
        for (int j=0; j<f[i].getList().size(); ++j) { // for each vertex in this face, measure the edge it connects with the next vertex
            //cout << i<<"\n";
            if (j == 0) {
                self = f[i].getList()[j];
                next = f[i].getList()[j+1];
            }else if(j==f[i].getList().size()-1){
                self = f[i].getList()[j];
                next = f[i].getList()[0];
            }else{
                self = f[i].getList()[j];
                next = f[i].getList()[j+1];
            }
            
            vecx = v[next].getX() - v[self].getX();
            vecy = v[next].getY() - v[self].getY();
            
            thisEdge = sqrt(pow(vecx,2)+pow(vecy,2));
            
            if(j==0){
                max = thisEdge;
                min = thisEdge;
                ver1 = self; // shortest
                ver2 = next;
                ver3 = self; // longest
                ver4 = next;
                last = f[i].getList()[f[i].getList().size()-1];
            }else{
                if (thisEdge>max) {
                    max = thisEdge;
                    ver3 = self;
                    ver4 = next;
                }
                if (thisEdge<min) {
                    min = thisEdge;
                    ver1 = self;
                    ver2 = next;
                    if(j==0){
                        last = f[i].getList()[f[i].getList().size()-1];
                    }else{
                        last =f[i].getList()[j-1];
                    }
                }
            }
        }// end of all edges in the face
        tempAR = max/min;
        //cout <<"tempAR "<< tempAR <<" " <<sqrt(pow(v[ver4].getX() - v[ver3].getX(),2)+pow(v[ver4].getY() - v[ver3].getY(),2))/min<< endl;
        
        if(tempAR > ar){
            /*cout << ver1<<" "<< ver2 << " "<< last<< " lk"<< "\n";
             cout << v[ver1].getX()<<" "<< v[ver2].getX() << " "<< v[last].getX()<< " lk"<< "\n";
             cout << v[ver1].getY()<<" "<< v[ver2].getY() << " "<< v[last].getY()<< " lk"<< "\n";*/
            int verToMove;
            
            if(ver1 == ver4)//both ccw
            {
                verToMove = ver1;// Lo 3 4=1 2 Sh
                if(v[verToMove].ifBound() == 0){
                    //cout << ver1<<" "<< ver2 << " "<< last<< " tempAR > ar"<< "\n";
                    double pdist = sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2));
                    //cout<< "pdist "<<pdist<<"\n";
                    double angle = -0.03;
                    double centerX = GetCircumCenterX(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[last].getX(), v[last].getY());
                    double centerY = GetCircumCenterY(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[last].getX(), v[last].getY());
                    //cout << "pdist " << pdist << " centerX " << centerX<< " centerY " << centerY<<"\n";
                    while(sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2)) < (100*pdist)/99){
                        v[verToMove].setX(movePX(v[verToMove].getX(), v[verToMove].getY(), centerX, centerY, angle));
                        v[verToMove].setY(movePY(v[verToMove].getX(), v[verToMove].getY(), centerX, centerY, angle));
                        
                        /*cout <<"1:"<<v[ver1].getX()<<" "<<v[ver1].getY()<<" 2:"<<v[ver2].getX()<<" "<<v[ver2].getY()
                         <<" "<<"\n";
                         
                         cout <<sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2))
                         <<" "<<"pdist "<<pdist << "\n";
                         cout << "\n";*/
                    }
                }else if(v[ver2].ifBound() == 0){
                    verToMove = ver2;
                    //cout << ver1<<" "<< ver2 << " "<< last<< " tempAR > ar"<< "\n";
                    double pdist = sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2));
                    //cout<< "pdist "<<pdist<<"\n";
                    double angle = 0.03;
                    double centerX = GetCircumCenterX(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[last].getX(), v[last].getY());
                    double centerY = GetCircumCenterY(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[last].getX(), v[last].getY());
                    //cout << "pdist " << pdist << " centerX " << centerX<< " centerY " << centerY<<"\n";
                    while(sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2)) < (100*pdist)/99){
                        v[verToMove].setX(movePX(v[verToMove].getX(), v[verToMove].getY(), centerX, centerY, angle));
                        v[verToMove].setY(movePY(v[verToMove].getX(), v[verToMove].getY(), centerX, centerY, angle));
                        //cout << ".ver1=4, 1"<<endl;
                    }
                }else if(v[ver3].ifBound() == 0){
                    verToMove = ver3;
                    //cout << ver1<<" "<< ver2 << " "<< last<< " tempAR > ar"<< "\n";
                    double pdist = sqrt(pow(v[ver3].getX()-v[ver4].getX(), 2) + pow(v[ver3].getY()-v[ver4].getY(), 2));
                    //cout<< "pdist "<<pdist<<"\n";
                    double angle = 0.03;
                    double centerX = GetCircumCenterX(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[last].getX(), v[last].getY());
                    double centerY = GetCircumCenterY(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[last].getX(), v[last].getY());
                    //cout << "pdist " << pdist << " centerX " << centerX<< " centerY " << centerY<<"\n";
                    while(sqrt(pow(v[ver3].getX()-v[ver4].getX(), 2) + pow(v[ver3].getY()-v[ver4].getY(), 2)) > (99*pdist)/100){
                        v[verToMove].setX(movePX(v[verToMove].getX(), v[verToMove].getY(), centerX, centerY, angle));
                        v[verToMove].setY(movePY(v[verToMove].getX(), v[verToMove].getY(), centerX, centerY, angle));
                        //cout << ".ver1=4, 2 "<<sqrt(pow(v[ver3].getX()-v[ver4].getX(), 2) + pow(v[ver3].getY()-v[ver4].getY(), 2))<<" "<< (99*pdist)/100<<endl;
                    }
                }
                //cout << "1 "<<endl;
            }
            
            
            if(ver2 == ver3)
            {
                //cout << "2 "<<endl;
                verToMove = ver3;// Sh 1 2=3 4 Lo
                if(v[verToMove].ifBound() == 0){
                    //cout << ver1<<" "<< ver2 << " "<< last<< " tempAR > ar"<< "\n";
                    double pdist = sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2));
                    //cout<< "pdist "<<pdist<<"\n";
                    double angle = 0.03;
                    double centerX = GetCircumCenterX(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[last].getX(), v[last].getY());
                    double centerY = GetCircumCenterY(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[last].getX(), v[last].getY());
                    //cout << "pdist " << pdist << " centerX " << centerX<< " centerY " << centerY<<"\n";
                    while(sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2)) < (100*pdist)/99){
                        v[verToMove].setX(movePX(v[verToMove].getX(), v[verToMove].getY(), centerX, centerY, angle));
                        v[verToMove].setY(movePY(v[verToMove].getX(), v[verToMove].getY(), centerX, centerY, angle));
                        
                        
                        //cout << ".ver2=3"<<endl;
                    }
                }else if(v[ver1].ifBound() == 0){
                    verToMove = ver1;
                    //cout << ver1<<" "<< ver2 << " "<< last<< " tempAR > ar"<< "\n";
                    double pdist = sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2));
                    //cout<< "pdist "<<pdist<<"\n";
                    double angle = -0.03;
                    double centerX = GetCircumCenterX(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[last].getX(), v[last].getY());
                    double centerY = GetCircumCenterY(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[last].getX(), v[last].getY());
                    //cout << "pdist " << pdist << " centerX " << centerX<< " centerY " << centerY<<"\n";
                    while(sqrt(pow(v[ver1].getX()-v[ver2].getX(), 2) + pow(v[ver1].getY()-v[ver2].getY(), 2)) < (100*pdist)/99){
                        v[verToMove].setX(movePX(v[verToMove].getX(), v[verToMove].getY(), centerX, centerY, angle));
                        v[verToMove].setY(movePY(v[verToMove].getX(), v[verToMove].getY(), centerX, centerY, angle));
                        //cout << ".ver2=3, 1"<<endl;
                    }
                }else if(v[ver4].ifBound() == 0){
                    verToMove = ver4;
                    //cout << ver1<<" "<< ver2 << " "<< last<< " tempAR > ar"<< "\n";
                    double pdist = sqrt(pow(v[ver3].getX()-v[ver4].getX(), 2) + pow(v[ver3].getY()-v[ver4].getY(), 2));
                    //cout << ".ver2=3, 2 " << sqrt(pow(v[ver3].getX()-v[ver4].getX(), 2) + pow(v[ver3].getY()-v[ver4].getY(), 2))<<  " "<<(99*pdist)/100 <<endl;
                    //cout<< "pdist "<<pdist<<"\n";
                    double angle = 0.03;
                    double centerX = GetCircumCenterX(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[last].getX(), v[last].getY());
                    double centerY = GetCircumCenterY(v[ver1].getX(), v[ver1].getY(), v[ver2].getX(), v[ver2].getY(), v[last].getX(), v[last].getY());
                    //cout << "pdist " << pdist << " centerX " << centerX<< " centerY " << centerY<<"\n";
                    while(         sqrt(pow(v[ver3].getX()-v[ver4].getX(), 2) + pow(v[ver3].getY()-v[ver4].getY(), 2)) > (99*pdist)/100){
                        v[verToMove].setX(movePX(v[verToMove].getX(), v[verToMove].getY(), centerX, centerY, angle));
                        v[verToMove].setY(movePY(v[verToMove].getX(), v[verToMove].getY(), centerX, centerY, angle));
                        //cout << ".ver2=3, 2 " << sqrt(pow(v[ver3].getX()-v[ver4].getX(), 2) + pow(v[ver3].getY()-v[ver4].getY(), 2)) <<  " "<<(99*pdist)/100<<endl;
                    }
                }
            }
        }
    }
}



void smoothLapAng(vector<vertex> &v, vector<face> &f){
    for (int i=0; i<v.size(); i++) {
        if (v[i].ifBound()==0) {// for each v to be smoothed
            //cout<< i <<": ";  // for test
            vector<int> neighbors;
            vector<face> neighface;
            for (int j=0; j<f.size(); j++) {//check all faces
                if(f[j].hasV(i) != -1){//if the face include v[i]
                    neighface.push_back(f[j]);//add it to neighbor faces list
                    for (int k=0; k<f[j].getList().size(); k++) {//go through the vertices in that face
                        int fTemp = f[j].getList()[k];
                        if(fTemp!=i){//if it is not v[i] itself, add to neighbor list
                            if (!ifduplicate(fTemp, neighbors)){
                                neighbors.push_back(fTemp);
                                //cout << fTemp<<" ";   // for test neighbor v
                            }
                        }
                    }
                }
            }//end of getting neighbors
            //cout<< "\n";  //for test
            
            double xnew = 0;
            double ynew = 0;
            int lastV = 0;
            int nextV = 0;
            
            for (int j=0; j<neighbors.size(); j++) {//for each vertex
                //cout << " neighbor vertex: " << neighbors[j] << endl; // for test angle vertices
                
                int ifOnly = 0;
                for (int k=0; k<neighface.size(); k++) { //for each neighbor face
                    if(neighface[k].hasV(neighbors[j]) != -1){// if it includes this vertex (actually only two possibility)
                        //cout << "neighbor face: " << neighface[k].listToS() << endl; // for test angle vertices
                        
                        ++ifOnly;
                        if((neighface[k].hasV(neighbors[j]) == neighface[k].getList().size()-1
                            && neighface[k].hasV(i) == 0)
                           || neighface[k].hasV(neighbors[j])+1 == neighface[k].hasV(i)){ //ccw from j to i
                            if(neighface[k].hasV(neighbors[j]) == 0){
                                lastV = neighface[k].getList()[neighface[k].getList().size()-1];
                                //cout << "last v: " << lastV <<endl; // for test angle vertices
                                
                            }else{
                                lastV = neighface[k].getList()[neighface[k].hasV(neighbors[j])-1];
                                //cout << "last v: " << lastV <<endl; // for test angle vertices
                            }
                            
                        }else{// cw from j to i
                            if(neighface[k].hasV(neighbors[j]) == neighface[k].getList().size()-1){
                                nextV = neighface[k].getList()[0];
                            }else{
                                nextV = neighface[k].getList()[neighface[k].hasV(neighbors[j])+1];
                            }
                            //cout << "next v: " << nextV <<endl; // for test angle vertices
                        }
                    }
                }
                
                if(ifOnly == 1){
                    for (int k=0; k<neighface.size(); k++) { //for each neighbor face
                        if(neighface[k].hasV(neighbors[j]) != -1){
                            if(neighface[k].hasV(neighbors[j]) == 0){
                                lastV = neighface[k].getList()[neighface[k].getList().size()-1];
                                //cout << "last v: " << lastV <<endl; // for test angle vertices
                                
                            }else{
                                lastV = neighface[k].getList()[neighface[k].hasV(neighbors[j])-1];
                                //cout << "last v: " << lastV <<endl; // for test angle vertices
                            }
                            
                            if(neighface[k].hasV(neighbors[j]) == neighface[k].getList().size()-1){
                                nextV = neighface[k].getList()[0];
                            }else{
                                nextV = neighface[k].getList()[neighface[k].hasV(neighbors[j])+1];
                            }
                        }
                    }
                }
                
                
                double angle1;  // angle next
                double angle2;  // angle last
                double beta;
                double vecLastX, vecLastY, vecSelfX, vecSelfY, vecNextX, vecNextY;
                
                vecLastX = v[lastV].getX()-v[neighbors[j]].getX();
                vecLastY = v[lastV].getY()-v[neighbors[j]].getY();
                vecSelfX = v[i].getX()-v[neighbors[j]].getX();
                vecSelfY = v[i].getY()-v[neighbors[j]].getY();
                vecNextX = v[nextV].getX()-v[neighbors[j]].getX();
                vecNextY = v[nextV].getY()-v[neighbors[j]].getY();
                
                angle1 = acos((vecNextX*vecSelfX + vecNextY*vecSelfY)
                              /(sqrt(pow(vecNextX,2)+pow(vecNextY,2))
                                *sqrt(pow(vecSelfX,2)+pow(vecSelfY,2)) )  );
                //cout << "angle1: " << angle1*180/3.1415926 << endl;  // for test angle
                angle2 = acos((vecLastX*vecSelfX + vecLastY*vecSelfY)
                              /(sqrt(pow(vecLastX,2)+pow(vecLastY,2))
                                *sqrt(pow(vecSelfX,2)+pow(vecSelfY,2)) )  );
                //cout << " angle2: " << angle2*180/3.1415926 << endl;  //for test angle
                
                
                beta = (angle2 - angle1)/2;
                //cout << beta << endl;
                
                xnew += v[neighbors[j]].getX() + vecSelfX*cos(beta) - vecSelfY*sin(beta);
                ynew += v[neighbors[j]].getY() + vecSelfX*sin(beta) + vecSelfY*cos(beta);
                //cout << i << " x: " << xnew << endl;
                //cout << i << " y: " << ynew << endl;
            }//end of this vertex
            
            v[i].setX(xnew/neighbors.size());
            v[i].setY(ynew/neighbors.size());
            
            //cout << i << ": " << v[i].getX() << endl;
            //cout << i << ": " << v[i].getY() << endl;
            
        }
        
    }
    //outputfile<< "test" << endl;
}

double sortByAR(face &f1, face &f2){
    return f1.getAR() > f2.getAR();
}

double sortByDet(face &f1, face &f2){
    return f1.getDet() > f2.getDet();
}

void maxminAng(vector<vertex> v, vector<face> f, double &max, double &min){
    double angleTemp = 20.123456;
    double vec1x, vec1y, vec2x, vec2y;
    for (int i=0; i<f.size(); ++i) { // for each face
        for (int j=0; j<f[i].getList().size(); ++j) { // for each vertex in this face, there is an angle
            int last, self, next;
            self = f[i].getList()[j];
            if (j == 0) {
                last = f[i].getList()[f[i].getList().size()-1];
                next = f[i].getList()[j+1];
            }else if(j==f[i].getList().size()-1){
                last = f[i].getList()[j-1];
                next = f[i].getList()[0];
            }else{
                last = f[i].getList()[j-1];
                next = f[i].getList()[j+1];
            }
            vec1x = v[last].getX() - v[self].getX();
            vec1y = v[last].getY() - v[self].getY();
            vec2x = v[next].getX() - v[self].getX();
            vec2y = v[next].getY() - v[self].getY();
            
            angleTemp = acos((vec2x*vec1x + vec2y*vec1y)
                             /(sqrt(pow(vec1x,2)+pow(vec1y,2))
                               *sqrt(pow(vec2x,2)+pow(vec2y,2)) )  );
            
            //for test angle
            /*cout << "vec2x*vec1x + vec2y*vec1y: " << vec2x*vec1x + vec2y*vec1y << endl;
             cout << "sqrt(pow(vec1x,2)+pow(vec1y,2))*sqrt(pow(vec2x,2)+pow(vec2y,2)): " << sqrt(pow(vec1x,2)+pow(vec1y,2))*sqrt(pow(vec2x,2)+pow(vec2y,2)) << endl;
             cout << "angleTemp: " << angleTemp << endl;*/
            
            
            if (i==0) {
                max = angleTemp;
                min = angleTemp;
            }else{
                if (angleTemp > max) {
                    max = angleTemp;
                }
                if (angleTemp < min){
                    min = angleTemp;
                }
            }
        }// end of this angle
    }//end of this face
    
}

double aspectR(vector<vertex> v, vector<face> &f, double &med){
    double tempAR, finalAR;
    finalAR = 0;
    double resultVal;

    Vertex face[4];
    
    for (int i=0; i<f.size(); ++i) { // for each face
        double min, max, vecx, vecy;
        double thisEdge;
        
        for (int j = 0; j < f[i].getList().size(); j++) { //jacob and modified det calculate
            face[j].x = v[f[i].getList()[j]].getX();
            face[j].y = v[f[i].getList()[j]].getY();
            face[j].z = v[f[i].getList()[j]].getZ();
            //cout << "j " << j<<endl;
        }
        //cout << analyze(face).jacobianDet <<endl;
        f[i].setDet(analyze(face).jacobianDet);
        
        //worstModDet = analysis.modifiedDet;
        
        
        for (int j=0; j<f[i].getList().size(); ++j) { // for each vertex in this face, measure the edge it connects with the next vertex
            int self, next;
            if (j == 0) {
                self = f[i].getList()[j];
                next = f[i].getList()[j+1];
            }else if(j==f[i].getList().size()-1){
                self = f[i].getList()[j];
                next = f[i].getList()[0];
            }else{
                self = f[i].getList()[j];
                next = f[i].getList()[j+1];
            }
            
            vecx = v[next].getX() - v[self].getX();
            vecy = v[next].getY() - v[self].getY();
            
            thisEdge = sqrt(pow(vecx,2)+pow(vecy,2));
            
            if(j==0){
                max = thisEdge;
                min = thisEdge;
            }else{
                if (thisEdge>max) {
                    max = thisEdge;
                }
                if (thisEdge<min) {
                    min = thisEdge;
                }
            }
        }// end of all edges in the face
        tempAR = max/min;
        //cout << "tempAR "<<tempAR<<endl;
        
        //set aspect ratio in each face
        f[i].setAR(tempAR);
        
        if(finalAR < tempAR){
            finalAR = tempAR;
        }
    }//end of all faces
    
    sort(f.begin(), f.end(), sortByAR);    // !!!!! change here for sorting type!!!!
    
    med = f[2*f.size()/3].getAR();
    
    return finalAR;
}

double GetCircumCenterX(double Ax, double Ay, double Bx, double By, double Cx, double Cy)
{
    double BCx, BCy, ABx, ABy, k1, k2;
    
    BCx = ((Bx + Cx)/2.0);
    BCy = ((By + Cy)/2.0);
    
    ABx = ((Bx + Ax)/2.0);
    ABy = ((By + Ay)/2.0);
    
    k1 = (- Bx + Ax)/(By - Ay);
    k2 = (- Cx + Bx)/(Cy - By);
    
    if(By == Ay){
        return ABx;
    }
    
    if(Cy == By){
        return BCx;
    }
    
    
    return (((ABy - k1*ABx) - (BCy - k2 * BCx)) / (k2 - k1));
    
}

double GetCircumCenterY(double Ax, double Ay, double Bx, double By, double Cx, double Cy)
{
    double BCx, BCy, ABx, ABy, k1, k2, CircumCenterX;
    
    BCx = ((Bx + Cx)/2.0);
    BCy = ((By + Cy)/2.0);
    
    ABx = ((Bx + Ax)/2.0);
    ABy = ((By + Ay)/2.0);
    
    k1 = (- Bx + Ax)/(By - Ay);
    k2 = (- Cx + Bx)/(Cy - By);
    
    CircumCenterX = GetCircumCenterX(Ax, Ay, Bx, By, Cx, Cy);
    
    if(By == Ay){
        return k2*ABx + BCy - k2*BCx;
    }
    
    if(By == Cy){
        return k1*BCx + ABy - k1*ABx;
    }
    
    return (k1 * (CircumCenterX - ABx) + ABy);
    
}

double movePX(double Ax, double Ay, double cenX, double cenY, double ang){
    return cenX + (Ax - cenX)*cos(ang) - (Ay - cenY)*sin(ang);
}

double movePY(double Ax, double Ay, double cenX, double cenY, double ang){
    return cenY + (Ax - cenX)*sin(ang) + (Ay - cenY)*cos(ang);
}




CREAnalysis analyze(Vertex vertices[4]) {
    
    CREAnalysis result;
    Vertex centroid;
    Vertex midpoint[4];
    int j;
    Vertex v06, v07;
    double v06Length;
    Vertex localX, localY, localZ;
    Vertex v06xv07; //V06 cross with V07
    Vertex v0[4];
    Vertex localCoord[4];
    double e1, e2, e3, e4, f1, f2, f3, f4;
    
    double thisAR = 0.0;
    double worstAR = 1.0;
    
    
    centroid.x = 0.25 * (vertices[0].x + vertices[1].x + vertices[2].x + vertices[3].x);
    centroid.y = 0.25 * (vertices[0].y + vertices[1].y + vertices[2].y + vertices[3].y);
    
    //if (DEBUG) printf("centroid.x: %lf\t centroid.y: %lf\n", centroid.x, centroid.y);
    
    for (j=0; j<4; j++) {
        midpoint[j].x = 0.5 * (vertices[j].x + vertices[(j+1)%4].x);
        midpoint[j].y = 0.5 * (vertices[j].y + vertices[(j+1)%4].y);
        //if (DEBUG) printf("midpt[%d]: (%lf, %lf)\n", j, midpoint[j].x, midpoint[j].y);
    }
    
    v06.x = midpoint[1].x - centroid.x;
    v06.y = midpoint[1].y - centroid.y;
    v06Length = sqrt(pow(v06.x, 2) + pow(v06.y, 2));
    
    //if (DEBUG) printf("v06: (%lf, %lf)\n", v06.x, v06.y);
    
    localX.x = v06.x/v06Length;
    localX.y = v06.y/v06Length;
    
    //if (DEBUG) printf("localX: (%lf, %lf)\n", localX.x, localX.y);
    
    v07.x = midpoint[2].x - centroid.x;
    v07.y = midpoint[2].y - centroid.y;
    
    v06xv07.x = 0;
    v06xv07.y = 0;
    v06xv07.z = (v06.x * v07.y) + (v06.y * v07.x);
    
    localZ.z = 1;
    
    localY.x = -localX.y;
    localY.y = localX.x;
    
    //if (DEBUG) printf("localY: (%lf, %lf)\n", localY.x, localY.y);
    
    for (j=0; j<4; j++) {
        v0[j].x = vertices[j].x - centroid.x;
        v0[j].y = vertices[j].y - centroid.y;
        
        /*if (DEBUG) {
            printf("v0%dx: %lf, v0%dy: %lf\n", j+1, v0[j].x, j+1, v0[j].y);
            printf("v0%dx * localX.x: %lf\n", j+1, v0[j].x * localX.x);
            printf("v0%dy * localX.y: %lf\n", j+1, v0[j].y * localX.y);
        }*/
        
        localCoord[j].x = (v0[j].x * localX.x) + (v0[j].y * localX.y);
        localCoord[j].y = (v0[j].x * localY.x) + (v0[j].y * localY.y);
    }
    
    e1 = 0.25 * (localCoord[0].x + localCoord[1].x + localCoord[2].x + localCoord[3].x);
    e2 = 0.25 * (-localCoord[0].x + localCoord[1].x + localCoord[2].x - localCoord[3].x);
    e3 = 0.25 * (-localCoord[0].x - localCoord[1].x + localCoord[2].x + localCoord[3].x);
    e4 = 0.25 * (localCoord[0].x - localCoord[1].x + localCoord[2].x - localCoord[3].x);
    
    f1 = 0.25 * (localCoord[0].y + localCoord[1].y + localCoord[2].y + localCoord[3].y);
    f2 = 0.25 * (-localCoord[0].y + localCoord[1].y + localCoord[2].y - localCoord[3].y);
    f3 = 0.25 * (-localCoord[0].y - localCoord[1].y + localCoord[2].y + localCoord[3].y);
    f4 = 0.25 * (localCoord[0].y - localCoord[1].y + localCoord[2].y - localCoord[3].y);
    
    /*if (DEBUG) {
        for (j=0; j<4;j++) {
            printf("x%d:%lf y%d:%lf\n", j, localCoord[j].x, j, localCoord[j].y);
        }
    }*/
    
    //if (DEBUG) printf("e2:%lf\te3:%lf\tf2:%lf\tf3:%lf\n", e2, e3, f2, f3);
    //printf("e2/f3:%lf\tf3/e2:%lf\n", e2/f3, f3/e2);
    //printf("e3/f3:%lf\tf2/e2:%lf\n", e3/f3, f2/e2);
    
    if (e2/f3 > f3/e2) result.AR = e2/f3;
    else result.AR = f3/e2;
    
    result.skew = e3/f3;
    result.taperX = f4/f3;
    result.taperY = e4/e2;
    result.jacobianDet = pow(f3, 2) * result.AR * (1 + (result.taperX) + (result.taperY - (result.taperX *(result.skew/result.AR))));
    result.modifiedDet = result.AR * (1 + fabs(result.taperX) + (fabs(result.taperY) - (fabs(result.taperX) *(result.skew/result.AR))));
    
    return result;
    //printf("holy shit this worked %lf", v06.x);
}


