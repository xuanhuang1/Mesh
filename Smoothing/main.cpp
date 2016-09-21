//
//  main.cpp
//
//
//  Created by Xuan Huang on 6/23/16.
//
//

#include "functions.h"

using namespace std;

int main(int argc, char* argv[]){
    ifstream inputFile(argv[1]);
    
    if(!inputFile){
        cout << "Cannot open file" << endl;
        return 1;
    }
    
    string str;
    int line = 0;
    vector<vertex> v;
    vector<vertex> nearb;
    vector<edge> e;
    vector<face> f;
    
    while (inputFile) {
        getline(inputFile, str);
        int nums[3];
        if(line == 0){
            if (str.compare("OFF") != 0) {   // read the first line
                cout << "Not an .OFF file" << endl;
                return 1;
            }
        }else if(line == 1){  // read numbers of v f e
            int i = 0;
            stringstream ss(str);
            while (i < 3) {
                ss >> nums[i];
                i++;
            }
        }else if(line >1 && line < 1+nums[0]+1){  // read vertices
            double x,y,z;
            stringstream ss(str);
            ss >>x>>y >>z;
            v.push_back(vertex(x,y,z));
        }else if(line >1+nums[0] && line < 1+nums[0]+nums[1]+1){  // read faces
            int vs;
            int num;
            vector<int> numfs;
            stringstream ss(str);
            ss >> vs;
            for (int i=0; i<vs; i++) {  // read edges
                ss >> num;
                numfs.push_back(num);
                if (i != 0) {
                    edge eTemp(numfs[i-1],numfs[i]);
                    for (int j =0; j<e.size(); j++) {
                        if( (e[j].get1()== numfs[i-1]&&e[j].get2()== numfs[i]) || (e[j].get2()== numfs[i-1]&&e[j].get1()== numfs[i]) ){
                            eTemp.addRep();
                            e[j].addRep();
                        }
                    }
                    if(eTemp.getRep() == 0)
                        e.push_back(eTemp);
                }
            }
            // add the tail with head - the last edge in list
            edge eTemp(numfs[vs-1],numfs[0]);
            for (int j =0; j<e.size(); j++) {
                if( (e[j].get1()== numfs[vs-1]&&e[j].get2()== numfs[0]) || (e[j].get2()== numfs[vs-1]&&e[j].get1()== numfs[0])){
                    eTemp.addRep();
                    e[j].addRep();
                }
            }
            if(eTemp.getRep() == 0)
                e.push_back(eTemp);
            
            
            f.push_back(face(numfs));
        }// end if, end process of this line
        
        ++line;
    }//end while input
    
    for (int i=0; i<e.size(); i++) {
        if (e[i].getRep() == 0) {
            v[e[i].get1()].onBound();
            v[e[i].get2()].onBound();
        }
    }//mark the vertices on boundary
    
    
    // end reading file
    inputFile.close();
    
    //initial max min angle, aspect ratio, "standard" aspect ratio - below which do not smooth
    double max, min, ap, threA;
    
    maxminAng(v, f, max, min);
    cout <<"max: " <<max*180/3.1415926 <<endl;
    cout <<"min: " <<min*180/3.1415926 <<endl;
    
    ap = aspectR(v, f, threA);
    cout << "aspect ratio: " << ap <<" threshold: " << threA<<endl;
    
    //smooth until converge
    vector<vertex> v2 = v;
    double diff = 999;
    double diff2 = 1000;
    double count = 6;
    
    while( diff <diff2 || diff >0.3){
        diff2 = diff;
        smooth(v2, f, threA);
        //ap = aspectR(v2, f, threA);
        //smoothLapAng(v2, f);
        diff = dif(v2, v);
        v = v2;
        //cout << diff <<" ";
        //cout << diff2 <<" ";
        --count;
    }
    cout <<"\n";
    
    
    maxminAng(v, f, max, min);
    cout <<"max: " <<max*180/3.1415926 <<endl;
    cout <<"min: " <<min*180/3.1415926 <<endl;
    
    ap = aspectR(v, f, threA);
    cout << "aspect ratio: " << ap <<endl;
    
    //output
    ofstream outputfile;
    outputfile.open(argv[2], ios::out | ios::trunc);
    
    outputfile << "OFF" << endl;
    outputfile << v.size()<<" "<< f.size()<<" "<<e.size()<<" "<< endl;
    
    for (int i =0; i<v.size(); i++){
        outputfile << v[i].getX() << " " << v[i].getY() << " " << v[i].getZ() <<endl;
    }
    
    for (int i =0; i<f.size(); i++){
        outputfile << f[i].getList().size() << " ";
        for (int j =0; j<f[i].getList().size(); j++)
            outputfile << f[i].getList()[j] << " ";
        outputfile <<"\n";
    }
    outputfile.close();
    
    return 0;
    
}
//test for class vertex
/*
 vertex v(1.3, 2);
 v.setNum(0);
 vertex v2(3,5.2);
 v2.setNum(1);
 cout << "v1 getX: " << v.getX() << " num: " << v.getNum() << endl;
 cout << "v1 getY: " << v.getY() << endl;
 cout << "v2 getX: " << v2.getX() << " num: " << v2.getNum()<< endl;
 cout << "v2 getY: " << v2.getY() << endl;
 */

//test for class face
/*
 vector<int> list;
 list.push_back(1);
 list.push_back(8);
 list.push_back(10);
 list.push_back(12);
 list.push_back(7);
 face f(list);
 
 cout << "face v number: " << f.getList().size() <<endl;
 for (int i =0; i<f.getList().size(); i++) {
 cout << "face v :" << f.getList()[i] << endl;
 }
 
 //test for read v, e, f
 for (int i =0; i<v.size(); i++)
 cout << v[i].getX() << " " << v[i].getY() << " " << v[i].getZ() <<endl;
 
 for (int i =0; i<f.size(); i++){
 for (int j =0; j<f[i].getList().size(); j++)
 cout << f[i].getList()[j] << " ";
 cout <<"\n";
 }
 
 for (int i =0; i<e.size(); i++)
 cout << e[i].get1() << " " << e[i].get2() <<endl;
 */

//test for read v, e, f
/*for (int i =0; i<v.size(); i++){
 if (v[i].ifBound() == 1) {
 cout << " vertice on bound" <<endl;
 }
 cout << v[i].getX() << " " << v[i].getY() << " " << v[i].getZ() <<endl;
 }
 
 for (int i =0; i<f.size(); i++){
 for (int j =0; j<f[i].getList().size(); j++)
 cout << f[i].getList()[j] << " ";
 cout <<"\n";
 }
 
 for (int i =0; i<e.size(); i++)
 cout << e[i].get1() << " " << e[i].get2() <<endl;
 */
