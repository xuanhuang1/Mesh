//
//  edge.h
//  
//
//  Created by Xuan Huang on 6/27/16.
//
//

#ifndef edge_h
#define edge_h

class edge{
    int node1, node2;
    int edgeRep;
public:
    edge(int,int);
    int get1();
    int get2();
    void addRep();
    int getRep();
};

#endif /* edge_h */
