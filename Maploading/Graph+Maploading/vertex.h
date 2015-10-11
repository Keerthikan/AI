//
//  vertex.h
//  mapLoading
//
//  Created by Keerthikan Ratnarajah on 11/10/15.
//  Copyright (c) 2015 Keerthikan Ratnarajah. All rights reserved.
//

#ifndef mapLoading_vertex_h
#define mapLoading_vertex_h
#include <vector>
#include <list>

//#include "edge.h"
#include <boost/shared_ptr.hpp>

class Vertex {
    char element;
    int num_Edges;
public:
    std::pair<int,int> coord;
    Vertex();
    Vertex(int x, int y, char element);
    std::pair<int,int> getCoord()
    {
        return coord;
    }
    char getElement()
    {
        return element;
    }
    int getNumEdges(){
        return num_Edges;
    }
};


#endif
