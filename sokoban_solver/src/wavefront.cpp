#include "wavefront.hpp"

#include <iostream>

using namespace std;

int** wavefront::get_map_array()
{
    int** map_array = new int*[col];
    for(int i = 0; i < row; i++)
    {
        map_array[i] = new int[col];
        for(int j = 0; j < col; j++)
        {
            map_array[i][j] = -1;
        }
    }
    return map_array;
}

wavefront::wavefront(const int row, const int col, std::vector<Node> graph)
{
    this->graph = graph;
    this->col = col;
    this->row = row;
    wavefront_map = get_map_array();
    wavefront_map = get_wavefront('G');
}

int** wavefront::get_wavefront()
{
    return wavefront_map;
}

int** wavefront::get_wavefront(char initiator)
{
    int** wavefront_map_l = get_map_array();
    for(Node node : graph)
    {
        if(node.Element == initiator)
        {
            wavefront_map_l[node.position.first][node.position.second] = 1;
            wave_queue.push(node);
        }
    }

    while(!wave_queue.empty())
    {
        Node node = wave_queue.front();
        wave_queue.pop();
        for(Edge edge : node.neighbours)
        {
            if(edge.neighbour != NULL && wavefront_map_l[edge.neighbour->position.first][edge.neighbour->position.second] == -1)
            {
                wave_queue.push(*edge.neighbour);
                wavefront_map_l[edge.neighbour->position.first][edge.neighbour->position.second] =
                        wavefront_map_l[edge.me->position.first][edge.me->position.second] + 1;
            }
        }
    }
/*
    cout << "printing wavefront:" << endl;
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            cout << wavefront_map_l[i][j] << " ";
        }
        cout << endl;
    }
*/
    return wavefront_map_l;
}

int** wavefront::get_wavefront(char initiator, char obstacle)
{
    int** wavefront_map_l = get_map_array();
    for(Node node : graph)
    {
        if(node.Element == initiator)
        {
            wavefront_map_l[node.position.first][node.position.second] = 1;
            wave_queue.push(node);
        }
    }

    while(!wave_queue.empty())
    {
        Node node = wave_queue.front();
        wave_queue.pop();
        for(Edge edge : node.neighbours)
        {
            if(edge.neighbour != NULL && wavefront_map_l[edge.neighbour->position.first][edge.neighbour->position.second] == -1 && edge.neighbour->Element != obstacle)
            {
                wave_queue.push(*edge.neighbour);
                wavefront_map_l[edge.neighbour->position.first][edge.neighbour->position.second] =
                        wavefront_map_l[edge.me->position.first][edge.me->position.second] + 1;
            }
        }
    }

    /*cout << "printing wavefront:" << endl;
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            cout << wavefront_map_l[i][j] << " ";
        }
        cout << endl;
    }*/

    return wavefront_map_l;
}
