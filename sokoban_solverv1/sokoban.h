//
//  sokoban.h
//  mapLoading
//
//  Created by Keerthikan Ratnarajah on 11/10/15.
//  Copyright (c) 2015 Keerthikan Ratnarajah. All rights reserved.
//

#ifndef mapLoading_sokoban_h
#define mapLoading_sokoban_h

#include <string>
#include <vector>

using namespace std;

struct Node;

struct Edge
{
   Node *me;
   Node *neighbour;

};

struct Node
{
    char Element;
    int weight; // Cost to  nearest goal
    int weight_diamond; // Cost to nearest diamond
    pair<int,int> position;
    bool deadlock;
    vector<Edge> neighbours;
};

class Sokoban{
public:
    int row;
    int col;
    int diamonds;
    char **board;
    std::vector<Node>  graph;
    bool **AdjMatrix;
    Sokoban(std::string file);
    //A* heuristic added

    int getRow(){
        return row;
    }
    int getCol(){
        return col;
    }
    int getDiamonds(){
        return diamonds;
    }
    char getChar(int x, int y){
        return board[y][x];
    }
    void createGraph();
    void deadlockDetection();
    void generateAdjacencyMatrix();
};

#endif
