//
//  sokoban.cpp
//  mapLoading
//
//  Created by Keerthikan Ratnarajah on 11/10/15.
//  Copyright (c) 2015 Keerthikan Ratnarajah. All rights reserved.
//

#include <iostream>
#include "sokoban.h"
#include <algorithm>
#include <string>
#include <fstream>

using namespace std;

void Sokoban::print_map()
{
    for(Node node : graph)
    {
        board[node.position.first][node.position.second] = node.Element;
    }

    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
    cout << "----------------------------" << endl;
}

void Sokoban::operator=(const Sokoban &rhs)
{
    row = rhs.row;
    col = rhs.col;
    diamonds = rhs.diamonds;
    board = rhs.board;
    heuristic = rhs.heuristic;
    graph = rhs.graph;
    AdjMatrix = rhs.AdjMatrix;
}


bool Sokoban::operator!=(const Sokoban &rhs )
{
    bool result = false;
    for(int i = 0; i < this->graph.size(); i++)
    {
        if(this->graph.at(i).Element != 'M' &&
                this->graph.at(i).Element != rhs.graph.at(i).Element &&
                rhs.graph.at(i).Element != 'M')
        {
            result = true;
            break;
        }
    }
    return result;
}

void Sokoban::make_final()
{
    for(std::vector<Node>::iterator node = begin(graph); node != end(graph); node++)
    {
        if(graph.at(node - graph.begin()).Element == 'J')
        {
            graph.at(node - graph.begin()).Element = '.';
        }

        if(graph.at(node-graph.begin()).Element== 'G')
        {
            graph.at(node - graph.begin()).Element = 'j';
        }
    }
}

bool Sokoban::move_diamond(Node *&B, int position)
{
    switch(B->neighbours.at(position).neighbour->Element)
    {
        case '.':
            std::swap(B->Element,B->neighbours.at(position).neighbour->Element);
            break;
        case 'G':
            if(B->Element == 'j')
            {
                B->Element = 'G';
            }
            else
            {
                B->Element = '.';
            }
            B->neighbours.at(position).neighbour->Element = 'j';
            break;
        default:
            break;
    }
}

bool Sokoban::swap(Node *&A, Node *&B, Node *&C)
{
    int position;
    for(int i = 0; i < B->neighbours.size(); i++)
    {
        if(B->neighbours.at(i).me != NULL && C->neighbours.at(i).neighbour->position == B->position)
        {
            position = i;
            break;
        }
    }
    // Check B-> element
    switch(B->Element)
    {
    case '.':
        std::swap(A->Element, B->Element);
        break;
    case 'J':
        move_diamond(B, position);
        std::swap(A->Element, B->Element);
        break;
    case 'G':
        if(A->Element == 'm')
        {
            A->Element = 'G';
        }
        else
        {
            A->Element = '.';
        }
        B->Element = 'm';
        break;
    case 'j':
        move_diamond(B, position);
        std::swap(A->Element, B->Element);
        break;
    default:
        break;
    }
    return true;
}

Sokoban::Sokoban(string file)
{
    ifstream myReadFile;
    myReadFile.open(file.c_str());
    myReadFile >> col;
    myReadFile >> row;
    myReadFile >> diamonds;
    cout << col << row << diamonds << endl;
    board = new char*[row];

    for (int i = 0; i < row; i++) {
        board[i] = new char[col];
    }
    //char board[col][row];
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            board[i][j] = ' ';
        }
    }
    //GotoLine(myReadFile, 1);
    string str;
    std::getline(myReadFile, str);
    cout << str << endl;

    int column = 0;
    int rows = 0;
    while(std::getline(myReadFile, str))
    {
        //cout << str << endl;
        while ((unsigned)column < (unsigned)str.length())
        {
            //cout << "lenght of string: " << str.length() << " at: " << rows << endl;

            board[rows][column] = str.at(column);

            column++;
        }
        column = 0;
        rows++;
    }

    for (int i = 0; i < row;  i++) {
        for (int j  = 0; j < col; j++) {
            cout << board[i][j];
        }
        cout << endl;
    }
    cout << endl;
    cout << endl;

    cout << "Map loaded" << endl;

}


Sokoban::Sokoban(const Sokoban &obj)
{
    row = obj.row;
    col = obj.col;
    diamonds = obj.diamonds;
    board = obj.board;
    heuristic = obj.heuristic;
    for(int i = 0; i < obj.graph.size(); i++)
    {
        graph.push_back(obj.graph.at(i));
        for(int j = 0; j < obj.graph.at(i).neighbours.size(); j++)
        {
            graph.at(i).neighbours.at(j) = *(new Edge(obj.graph.at(i).neighbours.at(j)));
        }
    }
    AdjMatrix = obj.AdjMatrix;
}

struct find_neigbour {
    const std::pair<int,int> position;
    find_neigbour(const std::pair<int,int> position) : position(position) {}
    bool operator()(const Node& j) const {
        return j.position == position;
    }
};

struct find_next_neighbour{
    const std::pair<int,int> position;
    find_next_neighbour(const std::pair<int,int> position) : position(position) {}
    bool operator()(const Edge& j) const{
      return j.neighbour->position == position;

    }
};


void Sokoban::createGraph()
{
  cout << "I was called, graph size:  " << graph.size() << endl;
//  Creating Each node
//  By looking at each entry of the 2d array
//--------------------------------------------------------------//
for (int row = 0; row < getRow(); row++) {
    for (int col = 0; col < getCol(); col++) {
        if (!isspace(getChar(row,col)) && getChar(row, col) != 'X')
        {
            Node node;
            node.Element = getChar(row,col);
            node.position = make_pair(row,col);
            node.deadlock = false;
            node.weight = 0;
            if(node.Element == 'X')
            {
              node.deadlock = true;
              cout << "node is deadlocked: " << node.position.first<< node.position.second << endl;
            }
            //cout << edge.Element << " at: " << edge.label.first << edge.label.second << endl;
            graph.push_back(node);
            //cout << edge.Element << endl;
            //cout << graph.size() << endl;
        }
    }
  }
  cout << "I end with graph size: " << graph.size() << endl;
  cout << endl;

  //finding NeigbourNodes
  //Looking at each node neigbour.
  //-------------------------------------------------------//
  int node = 0;
  for(auto it = begin(graph); it != end(graph) ; ++it)
  {
    node++;

    if (it->position.first >= 0 && it->position.first <= getRow())
    {
      //cout << "doing something" << endl;
      auto before = std::find_if(graph.begin(), graph.end(),find_neigbour(make_pair(it->position.first-1,it->position.second)));
      auto next = std::find_if(graph.begin(), graph.end(),find_neigbour(make_pair(it->position.first+1,it->position.second)));
      if(before != graph.end() && !isspace(graph[before-graph.begin()].Element))
      {
           Edge edge;
           edge.me = &*it;
           edge.neighbour = &*before;
           //edge.me = &graph[it-graph.begin()];
           //edge.neighbour = &graph[before-graph.begin()];
           it->neighbours.push_back(edge);
      }
      else
      {
          Edge empty;
          empty.me = NULL;
          empty.neighbour = NULL;
          it->neighbours.push_back(empty);
      }
      if(next != graph.end()&& !isspace(graph[next-graph.begin()].Element))
      {
             Edge edge;
             edge.me = &*it;
             edge.neighbour = &*next;
             //edge.me = &graph[it-graph.begin()];
             //edge.neighbour = &graph[next - graph.begin()];

             it->neighbours.push_back(edge);
      }
      else
      {
          Edge empty;
          empty.me = NULL;
          empty.neighbour = NULL;
          it->neighbours.push_back(empty);
      }
    }
    if (it->position.second >= 0 && it->position.second <= getCol() )
    {
      auto down = std::find_if(graph.begin(), graph.end(),find_neigbour(make_pair(it->position.first,it->position.second-1)));
      auto up = std::find_if(graph.begin(), graph.end(),find_neigbour(make_pair(it->position.first,it->position.second+1)));
      if(up != graph.end()&& !isspace(graph[up-graph.begin()].Element))
      {
          Edge edge;
          edge.me = &*it;
          edge.neighbour = &*up;
          //edge.me = &graph[it-graph.begin()];
          //edge.neighbour = &graph[up - graph.begin()];
          it->neighbours.push_back(edge);
      }
      else
      {
          Edge empty;
          empty.me = NULL;
          empty.neighbour = NULL;
          it->neighbours.push_back(empty);
      }

      if(down != graph.end() && !isspace(graph[down-graph.begin()].Element))
      {
           Edge edge;
           edge.me = &*it;
           edge.neighbour = &*down;
           //edge.me = &graph[it-graph.begin()];
           //edge.neighbour = &graph[down - graph.begin()];
           it->neighbours.push_back(edge);
      }
      else
      {
          Edge empty;
          empty.me = NULL;
          empty.neighbour = NULL;
          it->neighbours.push_back(empty);
      }
    }

      if(it->neighbours.size() <= 1 )
      {
        cout << "______________________________________________Hey something is wrong here ___________________________________ " << endl;       
      }
  }
  cout << "done" << endl;
}

void Sokoban::generateAdjacencyMatrix()
{
  AdjMatrix = new bool*[graph.size()];
  for (int i = 0; (unsigned)i <= graph.size(); i++)
  {
    AdjMatrix[i] = new bool[graph.size()];
  }

  //init
  for (size_t i = 0; i <= graph.size(); i++) {
      for (size_t j = 0; j <= graph.size(); j++) {
        AdjMatrix[i][j] = false;
      }
  }

  for(auto it = begin(graph); it != end(graph) ; ++it)
  {
    for(auto it_neighbour = begin(it->neighbours); it_neighbour != end(it->neighbours); it_neighbour++)
    {
      auto position_col_neighbour = find_if(graph.begin(),graph.end(),find_neigbour(it_neighbour->neighbour->position));
      auto position_row_me = find_if(graph.begin(),graph.end(),find_neigbour(it_neighbour->me->position));
      //cout <<  position_col_neighbour-graph.begin() << " " << position_row_me-graph.begin() << endl;
      AdjMatrix[position_col_neighbour-graph.begin()][position_row_me-graph.begin()] = true;
    }
  }

  cout << "AdjacencyMatrix" << endl;
  cout << endl;
  // for (int col = 0; (unsigned)col <= graph.size(); col++)
  // {
  //     for (int row = 0; (unsigned)row <= graph.size(); row++)
  //     {
  //         cout << AdjMatrix[col][row];
  //     }
  //     cout << endl;
  // }



}

void Sokoban::deadlockDetection()
{
    cout << "yolo" << endl;
    //Detect corners
    int counter = 0;
    for(auto node = begin(graph); node != end (graph) ;  ++node )
    {
        //cout << counter << endl;

        counter++;
        if (node->neighbours.size() == 4 && node->Element == '.')
        {
            //Is it neighbouring a goal or a diamond?
            bool incorrectCorner = true;
            // Neighbournode[0] = up
            // Neighbournode[1] = down
            // Neighbournode[2] = right
            // Neighbournode[3] = left


            // corner = [case1][case3], [case1][case4], [case2][case4],[case2][case3]
            if ((node->neighbours.at(0).me == NULL && node->neighbours.at(2).me == NULL) || (node->neighbours.at(0).me == NULL && node->neighbours.at(3).me == NULL) || (node->neighbours.at(1).me == NULL && node->neighbours.at(3).me == NULL) || (node->neighbours.at(1).me == NULL && node->neighbours.at(2).me == NULL) )
            {
                    incorrectCorner = false;
            }

            if (!incorrectCorner)
            {
                //cout << "corner detected" << endl;
                node->deadlock = true;
                node->weight = 100;
                //cout << "Locked" << endl;
            }
        }
    }

    //cout << "corners locked" << endl;

    //cout << "Lock in betweeen " << endl;
    vector<Node*> trace;

    //Find Corner pairs
    Node* firstCorner = NULL;
    Node* secondCorner = NULL;
    for (size_t i = 0; i < graph.size(); i++)
    {
        //cout << i << endl;
        for (auto node = begin(graph) ;  node!= end(graph) ; ++node)
        {
            if (node->deadlock ==true && firstCorner == NULL && secondCorner == NULL && node->weight == 100)
            {
                firstCorner = &*node;
                node->weight = 200;
                //cout << "first found" << firstCorner->position.first << firstCorner->position.second  << endl;
            }
            if(node->deadlock == true && secondCorner == NULL && firstCorner != NULL && node->weight == 100 && (firstCorner->position.first == node->position.first || firstCorner->position.second == node->position.second))
            {
                secondCorner = &*node;
                node->weight = 200;
                //cout << "second found" << secondCorner->position.first << secondCorner->position.second  << endl;
                break;
            }
        }

        //Pair of corners found
        // lock in between

        if(firstCorner != NULL and secondCorner != NULL)
        {
            Node* traversingNode = firstCorner;

            int changeX = secondCorner->position.first - firstCorner->position.first;
            int changeY = secondCorner->position.second - firstCorner->position.second;
            vector<Node*> trace;
            bool specialCase = false;
            while(traversingNode != secondCorner)
            {
                if(traversingNode->neighbours.size()== 0)
                {
                    cout  << "I got no friends " << endl;
                }
                for(auto findNext = begin(traversingNode->neighbours); findNext != end(traversingNode->neighbours) ; ++findNext)
                {
                    if(findNext->me != NULL)
                    { //Tjekke at den ikke deadlocker en diamant.
                        if (findNext->neighbour->position.first - traversingNode->position.first == changeX || findNext->neighbour->position.second - traversingNode->position.second == changeY)
                        {
                            traversingNode = findNext->neighbour;
                            trace.push_back(traversingNode);
                            if(traversingNode->Element != '.')
                            {
                                specialCase = true;
                                cout << "_____________________________Locking incorrect traceback need to be implemented ______________________" << endl;
                            }
                            //cout << traversingNode->position.first << traversingNode->position.second << endl;
                            break;
                        }
                    }
                }
                //traversingNode->deadlock = true;
            }
            //check Content
            for (auto trace_it = begin(trace); trace_it != end(trace); trace_it++ )
            {
                if(specialCase)
                {
                    //trace.at(0)->deadlock = true;
                }
                else
                {
                    (*trace_it)->deadlock = true;
                }
            }
            //cout << "line locked" << endl;
        }
        firstCorner = NULL;
        secondCorner = NULL;

        //cout << endl;
    }

    // reset weights
    for (auto node  =  begin(graph); node != end(graph); node++)
    {
        node->weight = 0;
    }

    cout << "done d" << endl;
}
