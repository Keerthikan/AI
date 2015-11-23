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

typedef std::pair<int,int> position_t;

struct Node;

struct Edge
{
   Node *me;
   Node *neighbour;

};
//int traverseNeighbour(Edge edge, Edge last_edge);

struct Node
{
    char Element;
    int weight; // Cost to  nearest goal
    int weight_diamond; // Cost to nearest diamond
    position_t position;
    bool deadlock;
    vector<Edge> neighbours;
};

class Sokoban{
public:
    int row;
    int col;
    int diamonds;
    char **board;
    int heuristics;
    std::vector<Node>  graph;
    bool **AdjMatrix;
    Sokoban(std::string file);
    void printMap();
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
        return board[x][y];
    }
    void createGraph();
    void deadlockDetection();
    void generateAdjacencyMatrix();
};

#endif
