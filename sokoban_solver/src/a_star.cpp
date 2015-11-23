#include "a_star.hpp"

#include <iostream>
#include <algorithm>
#include <queue>
#include <limits>

using namespace std;

bool a_star::validate_push_direction(Edge *edge)
{
    bool result;
    int direction_row = edge->me->position.first - edge->neighbour->position.first;
    int direction_col = edge->me->position.second - edge->neighbour->position.second;

    if(direction_row != 0)
    {
        if(direction_row == -1)
        {
            //RIGHT
            //DOWN - Need to be corrected
            if(edge->me->neighbours[UP].me == NULL || edge->me->neighbours[UP].neighbour->Element == DIAMOND || edge->neighbour->deadlock)
            {
                result = false;
            }
            else
            {
                result = true;
            }
        }
        else
        {
            //LEFT
            //UP - need to be corrected
            if(edge->me->neighbours[DOWN].me == NULL || edge->me->neighbours[DOWN].neighbour->Element == DIAMOND || edge->neighbour->deadlock)
            {
                result = false;
            }
            else
            {
                result = true;
            }
        }
    }
    else if(direction_col  != 0)
    {
        if(direction_col == -1)
        {
            //DOWN
            if(edge->me->neighbours[LEFT].me == NULL || edge->me->neighbours[LEFT].neighbour->Element == DIAMOND || edge->neighbour->deadlock)
            {
                result = false;

            }
            else
            {
                result = true;
            }
        }
        else
        {
            //UP
            if(edge->me->neighbours[RIGHT].me == NULL || edge->me->neighbours[RIGHT].neighbour->Element == DIAMOND || edge->neighbour->deadlock)
            {
                result = false;
            }
            else
            {
                result = true;
            }
        }
    }

    return result;
}

int a_star::get_move_cost(Node* node)
{
    return wavefront_ptr->get_wavefront(MAN)[node->position.first][node->position.second];
}

Node* a_star::get_push_direction(Node* node)
{
    int wavefront_val, wavefront_val_min = numeric_limits<int>::max();
    Node *push_direction = NULL;
    for(Edge &edge : node->neighbours)
    {
        if(edge.neighbour != NULL)
        {
            wavefront_val = wavefront_ptr->get_wavefront()[edge.neighbour->position.first][edge.neighbour->position.second];
            if(wavefront_val < wavefront_val_min && validate_push_direction(&edge))
            {
                wavefront_val_min = wavefront_val;
                push_direction = edge.neighbour;
            }
        }
    }
    return push_direction;
}

heuristic_t a_star::get_heuristic(vector<Node*> diamonds, char initiator)
{
    int diamond_to_goal_cost, diamond_try_cost, move_cost;
    heuristic_t heuristic, heuristic_min;
    heuristic_min.first = numeric_limits<int>::max();

    for(Node *node : diamonds)
    {
        diamond_try_cost = node->weight_diamond;
        diamond_to_goal_cost = wavefront_ptr->get_wavefront()[node->position.first][node->position.second];
        move_cost = get_move_cost(get_push_direction(node));
        heuristic.first = diamond_try_cost + diamond_to_goal_cost + move_cost;
        heuristic.second = node;
        if(heuristic.first < heuristic_min.first)
        {
            heuristic_min = heuristic;
        }
    }
    return heuristic_min;
}

vector<Node*> a_star::find_diamonds()
{
    vector<Node*> diamonds;
    for(Node &node : graph)
    {
        if(node.Element == DIAMOND)
        {
            diamonds.push_back(&node);
        }
    }
    return diamonds;
}

a_star::a_star(std::vector<Node> graph, wavefront *wavefront_ptr)
{
    this->wavefront_ptr = wavefront_ptr;
    this->graph = graph;
    for(Node node : graph)
    {
        if(node.Element == 'G')
        {
            goal.push_back(node.position);
        }
    }
}

std::string a_star::solve()
{
    string path;
    heuristic_t current_heuristic;
    //queue<vector<Node>> open, closed;

    //open.push(graph);

    current_heuristic = get_heuristic(find_diamonds(),MAN);

    //cout << open.front().Element << endl;
    return path;
}

bool a_star::find_man(Node node)
{
    return node.Element == 'M' ? true : false;
}

struct find_neigbour {
    const std::pair<int,int> position;
    find_neigbour(const std::pair<int,int> position) : position(position) {}

    bool operator()(const Node& j) const
    {
        return j.position == position;
    }
};
