#include "a_star.hpp"
#include <iostream>
#include <queue>

using namespace std;

a_star::a_star(state_s initial, state_s final, wavefront &wf, sokoban_map &sokoban)
{
    this->initial = initial;
    this->final = final;
    this->wavefront_ptr = wf;
    this->sokoban = sokoban;
}

void a_star::get_children(state_s parent, vector<state_s> &children)
{
    state_s child = parent;

    int **wavefront_obstacle = wavefront_ptr.get_wavefront(child, MAN, DIAMOND);

    for(diamond_t diamond : parent.diamonds)
    {
        for(int i = 0; i < 4; i++)
        {
            position_t position;
            switch(i)
            {
                case 0:
                    position = make_pair(diamond.first-1,diamond.second);
                    if(wavefront_obstacle[diamond.first-1][diamond.second] > 0 && validate_push_direction(diamond, position, wavefront_obstacle))
                    {
                        child.man = child.diamonds.at(i);
                        child.diamonds.at(i) = position;
                        children.push_back(child);
                    }
                    break;
                case 1:
                    position = make_pair(diamond.first,diamond.second+1);
                    if(wavefront_obstacle[diamond.first][diamond.second+1] > 0 && validate_push_direction(diamond, position, wavefront_obstacle))
                    {
                        child.man = child.diamonds.at(i);
                        child.diamonds.at(i) = position;
                        children.push_back(child);
                    }
                    break;
                case 2:
                    position = make_pair(diamond.first+1,diamond.second);
                    if(wavefront_obstacle[diamond.first+1][diamond.second] > 0 && validate_push_direction(diamond, position, wavefront_obstacle))
                    {
                        child.man = child.diamonds.at(i);
                        child.diamonds.at(i) = position;
                        children.push_back(child);
                    }
                    break;
                case 3:
                    position = make_pair(diamond.first,diamond.second-1);
                    if(wavefront_obstacle[diamond.first][diamond.second-1] > 0 && validate_push_direction(diamond, position, wavefront_obstacle))
                    {
                        child.man = child.diamonds.at(i);
                        child.diamonds.at(i) = position;
                        children.push_back(child);
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

bool a_star::compare_heuristic(state_s A, state_s B)
{
    return (A.heuristic < B.heuristic);
}

string a_star::solve()
{
    string path;
    state_s current;

    vector<state_s> closed, children;
    priority_queue<state_s, std::vector<state_s>, comp> open(compare_heuristic);
    get_heuristic(initial);
    open.push(initial);

    while(&open.top() != &final)
    {
        current = open.top();
        open.pop();
        closed.push_back(current);

        get_children(current, children);

        for(state_s child : children)
        {
            sokoban.print(child);
            open.push(child);
        }

        cout << "child :" << endl;


    }

    return path;
}

bool a_star::validate_push_direction(diamond_t diamond, position_t position, int** wavefront_obstacle)
{
    bool result;
    int direction_row = diamond.first - position.first;
    int direction_col = diamond.second - position.second;

    if(direction_row != 0)
    {
        if(wavefront_obstacle[diamond.first+1][diamond.second] == -1 ||
                wavefront_obstacle[diamond.first-1][diamond.second] == -1 ||
                sokoban.get_map()[diamond.first + (direction_row*-1)][diamond.second] == DEADLOCK)
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
        if(wavefront_obstacle[diamond.first][diamond.second+1] == -1 ||
            wavefront_obstacle[diamond.first][diamond.second-1] == -1 ||
            sokoban.get_map()[diamond.first][diamond.second + (direction_col*-1)] == DEADLOCK)
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

position_t a_star::get_push_direction(diamond_t diamond,int **wavefront_obstacle)
{
    position_t position;
    for(int i = 0; i < 4; i++)
    {
        switch(i)
        {
            case 0:
                position = make_pair(diamond.first-1,diamond.second);
                if(wavefront_obstacle[diamond.first-1][diamond.second] > 0 && validate_push_direction(diamond, position, wavefront_obstacle))
                {
                    break;
                }
                break;
            case 1:
                position = make_pair(diamond.first,diamond.second+1);
                if(wavefront_obstacle[diamond.first][diamond.second+1] > 0 && validate_push_direction(diamond, position, wavefront_obstacle))
                {
                    break;
                }
                break;
            case 2:
                position = make_pair(diamond.first+1,diamond.second);
                if(wavefront_obstacle[diamond.first+1][diamond.second] > 0 && validate_push_direction(diamond, position, wavefront_obstacle))
                {
                    break;
                }
                break;
            case 3:
                position = make_pair(diamond.first,diamond.second-1);
                if(wavefront_obstacle[diamond.first][diamond.second-1] > 0 && validate_push_direction(diamond, position, wavefront_obstacle))
                {
                    break;
                }
                break;
            default:
                break;
        }
    }

    return position;
}

int a_star::get_move_cost(state_s state, position_t position)
{
    return wavefront_ptr.get_wavefront(state, MAN)[position.first][position.second];
}

void a_star::get_heuristic(state_s &state)
{
    int diamond_to_goal_cost, move_cost, heuristic = 0;
    push_directions.clear();

    int **wavefront_obstacle = wavefront_ptr.get_wavefront(state, MAN, DIAMOND);
    for(diamond_t diamond : state.diamonds)
    {
        diamond_to_goal_cost = wavefront_ptr.get_wavefront()[diamond.first][diamond.second];
        move_cost = get_move_cost(state, get_push_direction(diamond, wavefront_obstacle));
        heuristic += diamond_to_goal_cost + move_cost;
    }
}
