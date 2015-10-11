//
//  graph.h
//  mapLoading
//
//  Created by Keerthikan Ratnarajah on 11/10/15.
//  Copyright (c) 2015 Keerthikan Ratnarajah. All rights reserved.
//

#ifndef mapLoading_graph_h
#define mapLoading_graph_h

#include <vector>
#include "vertex.h"
#include "sokoban.h"
class Graph
{
public:
    std::vector <Vertex> vertices;
    Graph(Sokoban Sokoban);
    bool **adjMatrix;
};

#endif
