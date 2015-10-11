//
//  vertex.cpp
//  mapLoading
//
//  Created by Keerthikan Ratnarajah on 11/10/15.
//  Copyright (c) 2015 Keerthikan Ratnarajah. All rights reserved.
//

#include "vertex.h"

Vertex::Vertex(int X, int Y, char Element)
{
    coord = std::make_pair(X,Y);
    element = Element;
    num_Edges = 0;
    
};

Vertex::Vertex()
{
    coord = std::make_pair(NULL,NULL);
    element = NULL;
    num_Edges = NULL;
}