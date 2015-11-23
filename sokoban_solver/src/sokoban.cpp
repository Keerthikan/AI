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
            node.weight_diamond = 0;
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
    //cout << node << endl;
    node++;
    double  beforeCount = 0;
    double  nextCount = 0;
    double  upCount  = 0;
    double  downCount = 0;

    if (it->position.first >= 0 && it->position.first <= getRow())
    {
      //cout << "doing something" << endl;
      auto before = std::find_if(graph.begin(), graph.end(),find_neigbour(make_pair(it->position.first-1,it->position.second)));
      auto next = std::find_if(graph.begin(), graph.end(),find_neigbour(make_pair(it->position.first+1,it->position.second)));
      if(before != graph.end() && !isspace(graph[before-graph.begin()].Element))
      {
           Edge edge;
           edge.me = &*it;
           edge.neighbour = &graph[before - graph.begin()];
           //cout <<"before is: "<< edge.neighbour->Element << endl;
           //cout << "at: " << "(" <<  edge.neighbour->position.first << "," <<edge.neighbour->position.second<< ")" << endl;
           //cout << "now: " << "(" << edge.me->position.first << "," << edge.me->position.second << ")" << endl;
           beforeCount++;
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
             edge.neighbour = &graph[next - graph.begin()];
             //cout <<"next is: "<< edge.neighbour->Element << endl;
             //cout << "at: " << "(" <<  edge.neighbour->position.first << "," <<edge.neighbour->position.second<< ")" << endl;
             //cout << "now: " << "(" << edge.me->position.first << "," << edge.me->position.second << ")" << endl;
             nextCount++;
             it->neighbours.push_back(edge);
             //cout << "-next arc - added" << endl;
             //cout <<"size: "<< it->neighbours.size() << endl;
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
             edge.neighbour = &graph[up - graph.begin()];
             //cout <<"up is: "<< edge.neighbour->Element << endl;
             //cout << "at: " << "(" <<  edge.neighbour->position.first << "," <<edge.neighbour->position.second<< ")" << endl;
             //cout << "now: " << "(" << edge.me->position.first << "," << edge.me->position.second << ")" << endl;
             upCount++;
             it->neighbours.push_back(edge);
             //cout << "up - arc - added" << endl;
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
               edge.neighbour = &graph[down - graph.begin()];
               //cout <<"down is: "<< edge.neighbour->Element << endl;
               //cout << "at: " << "(" <<  edge.neighbour->position.first << "," <<edge.neighbour->position.second<< ")" << endl;
               //cout << "now: " << "(" << edge.me->position.first << "," << edge.me->position.second << ")" << endl;
               downCount++;
               it->neighbours.push_back(edge);
               //cout << " down - arc - added" << endl;
      }
      else
      {
          Edge empty;
          empty.me = NULL;
          empty.neighbour = NULL;
          it->neighbours.push_back(empty);
      }
    }

      //cout << "beforeCount: " << beforeCount << endl;
      //cout << "nextCount: " << nextCount << endl;
      //cout << "downCount: " << downCount << endl;
      //cout << "upCount: " << upCount << endl;
      //cout << "Size of neig: "<< it->neighbours.size() << endl;
      //cout << endl;
      //cout << "first occurence" << endl;


      if(it->neighbours.size() <= 1 )
      {
        cout << "______________________________________________Hey something is wrong here ___________________________________ " << endl;
        cout <<"("<<it->position.first << "," << it->position.second << ")" << endl;
        cout << "beforeCount: " << beforeCount << endl;
        cout << "nextCount: " << nextCount << endl;
        cout << "downCount: " << downCount << endl;
        cout << "upCount: " << upCount << endl;
        cout << "Size of neig: "<< it->neighbours.size() << endl;
        cout << "neighbour is: " << it->neighbours.begin()->neighbour->position.first << it->neighbours.begin()->neighbour->position.second <<  endl;
      }

      downCount = 0;
      upCount = 0;
      beforeCount = 0;
      nextCount = 0;
  }
  //cout << "I know who is next to me " << endl;
  //cout << graph.begin()->Element << graph.begin()->neighbours.size() << graph.begin()->neighbours.begin()->neighbour->Element << endl;
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
