//
//  edge.h
//  mapLoading
//
//  Created by Keerthikan Ratnarajah on 11/10/15.
//  Copyright (c) 2015 Keerthikan Ratnarajah. All rights reserved.
//

#ifndef mapLoading_edge_h
#define mapLoading_edge_h
#include <iostream>
#include "vertex.h"
#include <vector>
class Edge {
public:
    Vertex Start;
    Vertex end;
public:
    Edge(); 
    Edge(Vertex start, Vertex end);
};

#endif
