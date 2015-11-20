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
    board = new char*[col];

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
        while ((unsigned)rows < (unsigned)str.length())
        {
            //cout << "lenght of string: " << str.length() << " at: " << rows << endl;
            board[column][rows] = str.at(rows);

            rows++;
        }
        rows = 0;
        column++;
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
for (int col = 0; col < getCol(); col++) {
  for (int row = 0; row < getRow(); row++) {
        if (!isspace(getChar(row,col)) && getChar(row, col) != 'X')
        {
            Node node;
            node.Element = getChar(row,col);
            node.position = make_pair(row,col);
            node.deadlock = false;
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
  for(auto it = begin(graph); it != end(graph) ; ++it)
  {
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
    }



      //cout << "beforeCount: " << beforeCount << endl;
      //cout << "nextCount: " << nextCount << endl;
      //cout << "downCount: " << downCount << endl;
      //cout << "upCount: " << upCount << endl;
      //cout << "Size of neig: "<< it->neighbours.size() << endl;
      //cout << endl;

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
  cout << graph.begin()->Element << graph.begin()->neighbours.size() << graph.begin()->neighbours.begin()->neighbour->Element << endl;

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
    for(auto graph_it = begin(graph); graph_it != end(graph); graph_it++)
    {
        if(graph_it->neighbours.size() == 2 && graph_it->Element == '.')
        {
            vector<Node*> trace;
            cout << "corner found " <<"("<< graph_it->position.first <<","<< graph_it->position.second << ")" << endl;
            graph_it->deadlock = true;

            for (Edge& edge : graph_it->neighbours)
            {
                cout << "Check neighbour direction" << endl;
                int changeX = 0;
                int changeY = 0;

                changeX = graph_it->position.first - edge.neighbour->position.first;
                changeY = graph_it->position.second - edge.neighbour->position.second;

                cout << "neighbour direction is first: " << changeX << changeY << endl;
                auto start_position = edge.neighbour;
                vector<Node*> trace;
                bool endIsCorner = false;
                bool conditionMet = false;
                cout << endl;
                if (start_position->neighbours.size() == 2 && start_position->Element == '.')
                {
                        start_position->deadlock = true;
                        endIsCorner = true;/* code */
                }
                else if(start_position->neighbours.size() == 4)
                {
                    conditionMet = true;
                }
                else

                while((start_position->neighbours.size() != 2|| start_position->neighbours.size() != 4) && endIsCorner == false && conditionMet == false && start_position->Element =='.' )
                {
                    for(Edge& traversing_edge : start_position->neighbours)
                    {
                        cout <<"position before: " << graph_it->position.first << graph_it->position.second << " now position: "<< start_position->position.first << start_position->position.second <<  " change is: " << (start_position->position.first - traversing_edge.neighbour->position.first) <<  " " << start_position->position.second - traversing_edge.neighbour->position.second  << " Element is: " << traversing_edge.neighbour->Element << endl;
                        if (traversing_edge.neighbour->neighbours.size() == 2 && traversing_edge.neighbour->Element == '.')
                        {
                            cout << "error found case 1" << endl;
                            cout << "position: " << traversing_edge.neighbour->Element << traversing_edge.neighbour->position.first << traversing_edge.neighbour->position.second << endl;
                            start_position = traversing_edge.neighbour;
                            start_position->deadlock =true;
                            trace.push_back(start_position);
                            endIsCorner = true;
                            conditionMet = true;
                            break;
                        }
                        else if(traversing_edge.neighbour->neighbours.size() == 4 && traversing_edge.neighbour->Element == '.')
                        {
                            cout << "error found case 2" << endl;
                            cout << "position: " << traversing_edge.neighbour->Element << traversing_edge.neighbour->position.first << traversing_edge.neighbour->position.second << endl;
                            conditionMet = true;
                            break;
                        }

                        if (start_position->position.first - traversing_edge.neighbour->position.first  == changeX && start_position->position.second - traversing_edge.neighbour->position.second == changeY  && traversing_edge.neighbour->Element == '.')
                        {
                            if (traversing_edge.neighbour->neighbours.size() == 3 && traversing_edge.neighbour->Element == '.')
                            {
                                cout << "traversed in right direction . " << endl;
                                start_position = traversing_edge.neighbour;
                                cout << "traversed to position: " << start_position->position.first << start_position->position.second <<" Element: "<<start_position->Element<< endl;
                                trace.push_back(start_position);
                            }

                            else if (traversing_edge.neighbour->neighbours.size() == 2 && traversing_edge.neighbour->Element == '.')
                            {
                                edge.neighbour->deadlock = true;
                                start_position = traversing_edge.neighbour;
                                cout << "traversed to position being corner: " << start_position->position.first << start_position->position.second <<" Element: "<<start_position->Element<< endl;
                                trace.push_back(start_position);
                                endIsCorner = true;
                            }

                            else if (traversing_edge.neighbour->neighbours.size() == 4 && traversing_edge.neighbour->Element == '.')
                            {
                                cout << "traversed something else: " << start_position->position.first << start_position->position.second <<" Element: "<<start_position->Element<< endl;
                                start_position = traversing_edge.neighbour;
                            }
                        }
                        cout << endl;
                    }
                    if (conditionMet)
                    {
                        break;/* code */
                    }

                }

                if (endIsCorner == true)
                {
                    for(auto traced: trace)
                    {
                        cout << "Traced for deadlocking position: " <<traced->position.first << traced->position.second << traced->Element<< endl;
                        if (traced->Element == '.')
                        {
                            cout << "deadlocking position: " <<traced->position.first << traced->position.second << traced->Element<< endl;
                            traced->deadlock = true;
                        }
                    }
                }
                else
                {
                    trace.empty();
                    endIsCorner = false;
                }
                cout<<endl;
            }

        }
        cout << endl;
    }

  cout << "deadLock detected" << endl;
}
