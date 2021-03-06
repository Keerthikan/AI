#include "a_star.hpp"

#include <iostream>
#include <algorithm>
#include <queue>
#include <limits>

using namespace std;

bool a_star::validate_push_direction(Edge *edge, int** wavefront_obstacle)
{
    bool result;
    int direction_row = edge->me->position.first - edge->neighbour->position.first;
    int direction_col = edge->me->position.second - edge->neighbour->position.second;

    if(direction_row != 0)
    {
            if(edge->me->neighbours[UP].me == NULL ||
                    edge->me->neighbours[UP].neighbour->Element == DIAMOND ||
                    edge->me->neighbours[UP].neighbour->Element == DIAMOND_ON_GOAL ||
                    edge->me->neighbours[DOWN].me == NULL ||
                    edge->me->neighbours[DOWN].neighbour->Element == DIAMOND ||
                    edge->me->neighbours[DOWN].neighbour->Element == DIAMOND_ON_GOAL ||
                    edge->neighbour->deadlock ||
                    wavefront_obstacle[edge->neighbour->position.first][edge->neighbour->position.second] == -1)
            {
                result = false;
            }            
            else
            {
                result = true;
            }

    }
    else if(direction_col != 0)
    {
         if(edge->me->neighbours[LEFT].me == NULL ||
                 edge->me->neighbours[LEFT].neighbour->Element == DIAMOND ||
                 edge->me->neighbours[LEFT].neighbour->Element == DIAMOND_ON_GOAL ||
                 edge->me->neighbours[RIGHT].me == NULL ||
                 edge->me->neighbours[RIGHT].neighbour->Element == DIAMOND ||
                 edge->me->neighbours[RIGHT].neighbour->Element == DIAMOND_ON_GOAL ||
                 edge->neighbour->deadlock ||
                 wavefront_obstacle[edge->neighbour->position.first][edge->neighbour->position.second] == -1)
            {
                result = false;

            }
            else
            {
                result = true;
            }
     }

    return result;
}

int a_star::get_move_cost(Node* node)
{
    if(node != NULL )
    {
        return wavefront_ptr->get_wavefront(MAN)[node->position.first][node->position.second];
    }
    return std::numeric_limits<int>::max();
}

Node* a_star::get_push_direction(Node* node, int** wavefront_obstacle)
{
    int wavefront_val;
    Node *push_direction = NULL;
    int count  =0 ;
    for(Edge &edge : node->neighbours)
    {
        count++;
        if(edge.neighbour != NULL)
        {
            wavefront_val = wavefront_ptr->get_wavefront()[edge.neighbour->position.first][edge.neighbour->position.second];
            if(validate_push_direction(&edge, wavefront_obstacle))
            {
                push_direction = edge.neighbour;
                cout << count<<  " " <<edge.neighbour->Element << endl;
                push_directions.push_back(make_pair(node, push_direction));
            }
            else
            {
                push_direction = NULL;
                push_directions.push_back(make_pair(push_direction, push_direction));
            }
        }
        else
        {
            push_direction = NULL;
            push_directions.push_back(make_pair(push_direction, push_direction));
        }
    }
    return push_direction;
}

int a_star::get_heuristic(vector<Node*> diamonds)
{
    int diamond_to_goal_cost, move_cost;
    int heuristic;

    push_directions.clear();
    int** wavefront_obstacle = wavefront_ptr->get_wavefront(MAN,DIAMOND);
    for(Node *node : diamonds)
    {
        diamond_to_goal_cost = wavefront_ptr->get_wavefront()[node->position.first][node->position.second];
        move_cost = get_move_cost(get_push_direction(node,wavefront_obstacle));
        heuristic += diamond_to_goal_cost + move_cost;
    }
    return heuristic;
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

a_star::a_star(Sokoban *sokoban, Sokoban *sokoban_final, wavefront *wavefront_ptr)
{
    this->wavefront_ptr = wavefront_ptr;
    this->graph = sokoban->graph;
    this->sokoban = sokoban;
    for(Node node : graph)
    {
        if(node.Element == 'G')
        {
            goal.push_back(node.position);
        }
    }
}

bool a_star::compare_heuristic(Sokoban *A, Sokoban *B)
{
    return (A->heuristic < B->heuristic);
}

std::string a_star::solve()
{
    string path;
    int current_heuristic;
    Sokoban* current;

    std::vector<Sokoban*> closed;
    priority_queue<Sokoban*, std::vector<Sokoban*>, comp> open(compare_heuristic);

    open.push(sokoban);
    current_heuristic = get_heuristic(find_diamonds());

    while(open.top() != &sokoban_final)
    {
        current = open.top();
        open.pop();
        closed.push_back(current);
        for(pair<Node*,Node*> new_state_pair : push_directions)
        {
            if(new_state_pair.first != NULL)
            {
                Sokoban *child = new Sokoban(*current);

                // cout <<"new_state: " <<neighbour.graph.begin()->Element << endl;
                // neighbour.graph.begin()->Element = 'K';
                //cout <<"current_state: " << current->graph.begin()->Element << endl;



                Node *A = NULL;
                Node *B = NULL;
                Node *C = NULL;
                for(Node &node :  child->graph)
                {
                    if(node.Element == 'M'|| node.Element == 'm')
                    {
                        A = &node;
                    }
                    else if(node.position == new_state_pair.first->position)
                    {
                        B = &node;
                    }
                    else if(node.position == new_state_pair.second->position)
                    {
                        C = &node;
                    }
                }

                if(child->swap(A, B, C))
                {
                    cout << "We got a child... Hovsa!" << endl;
                    child->print_map();
                    //current->print_map();
                }
                // std::vector<Node*>::iterator new_position = find_if(neighbour.graph.begin(), neighbour.graph.end(), find_neighbour(push_directions.at(new_states - begin(push_directions))->position));
                // std::vector<Node*>::iterator current_position = find_if(neighbour.graph.begin(),neighbour.graph.end(),find_man);

                 //cout << neighbour.graph.at(new_position-neighbour.graph.begin()).Element << endl;
                 //cout << neighbour.graph.at(current_position - neighbour.graph.begin()).Element << endl;
            }

        }
    }

    //cout << open.front().Element << endl;
    return path;
}

bool a_star::find_man(Node node)
{
    return (node.Element == 'M' || node.Element == 'm') ? true : false;
}
