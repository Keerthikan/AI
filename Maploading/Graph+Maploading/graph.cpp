//
//  graph.cpp
//  mapLoading
//
//  Created by Keerthikan Ratnarajah on 11/10/15.
//  Copyright (c) 2015 Keerthikan Ratnarajah. All rights reserved.
//

#include <iostream>
#include "graph.h"
#include "sokoban.h"
#include <vector>
#include "vertex.h"

Graph::Graph(Sokoban Sokoban)
{
    int col = Sokoban.getCol();
    int row = Sokoban.getRow();
    
    for (int y = 0; y < col; y++) {
        for (int x = 0; x < row; x++) {
            char element = Sokoban.getChar(x, y);
            if(element == '.' || element == 'G' || element == 'J' || element == 'M')
            {
                this->vertices.emplace_back(x,y,element);
                
            }
            
        }
        
    }
    int matrix_size = int(this->vertices.size());
    this->adjMatrix = new bool*[matrix_size];
    for (int i = 0; i < matrix_size; i++) {
        this->adjMatrix[i] = new bool[matrix_size];
    }
    
    for(int i = 0  ; i < matrix_size ; i++)
    {
        for(int y = 0; y < matrix_size  ;y++)
        {
            adjMatrix[i][y] = false;
        }
    }
    
    
    std::cout << "Vertex added to graph" << std::endl;
}