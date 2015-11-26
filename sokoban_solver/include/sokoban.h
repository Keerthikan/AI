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

#define DIAMOND         'J'
#define MAN             'M'
#define GOAL            'G'
#define DIAMOND_ON_GOAL 'j'

#define UP      0
#define DOWN    1
#define LEFT    2
#define RIGHT   3

struct Node;
struct Edge;

typedef std::pair<int,int> position_t;
typedef std::vector<Node> graph_t;

struct Node
{
    Node() = default;
    Node(const Node &obj)
    {
        this->deadlock = obj.deadlock;
        this->Element = obj.Element;
        this->weight = obj.weight;
        this->position = obj.position;
        for(int i = 0; i < neighbours.size(); i++)
        {
            this->neighbours.push_back(obj.neighbours.at(i));
        }
    }

    char Element;
    bool deadlock;
    int weight; // Cost to  nearest goal
    position_t position;
    vector<Edge> neighbours;
};

struct Edge
{
    Edge() = default;
    Edge(const Edge &obj)
    {
        if(obj.me != NULL)
        {
            this->me = new Node(*obj.me);
            this->neighbour = new Node(*obj.neighbour);

        }
        else
        {
            this->me = NULL;
            this->neighbour = NULL;
        }
    }

    Node *me;
    Node *neighbour;

};
//int traverseNeighbour(Edge edge, Edge last_edge);


class Sokoban{
public:
    Sokoban() = default;
    Sokoban(const Sokoban &obj);
    int row;
    int col;
    int diamonds;
    char **board;
    int heuristic;
    graph_t graph;
    graph_t *graph_test;
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

    void print_map();
    void createGraph();
    void deadlockDetection();
    void generateAdjacencyMatrix();
    void make_final();
    void operator =(const Sokoban &rhs);
    static bool swap(Node *&A, Node *&B, Node *&C);
private:
    static bool move_diamond(Node *&B, int position);
    bool operator !=(const Sokoban &rhs);

};

#endif
