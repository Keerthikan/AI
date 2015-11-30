#include "a_star.hpp"
#include <iostream>
#include <queue>
#include <map>
#include <algorithm>
#include <set>
#include <math.h>

using namespace std;

string a_star::stringify(state_s state)
{
    string ret_val;
    for(diamond_t diamond : state.diamonds)
    {
        ret_val += to_string(diamond.first) + to_string(diamond.second);
    }
    ret_val += to_string(state.man.first) + to_string(state.man.second);
    return ret_val;
}

a_star::a_star(state_s initial, state_s final, wavefront &wf, sokoban_map &sokoban)
{
    this->initial = initial;
    this->final = final;
    this->wavefront_ptr = wf;
    this->sokoban = sokoban;
}

void a_star::get_children(state_s parent, vector<state_s> &children)
{

    int diamond_num = 0;
    for(diamond_t diamond : parent.diamonds)
    {


        for(int i = 0; i < 4; i++)
        {
            position_t push_direction;
            state_s child = parent;
            child.heuristic = 0;
            int **wavefront_obstacle;
            if(sokoban.get_map(child)[child.man.first][child.man.second] == 'd')
            {
                wavefront_obstacle = wavefront_ptr.get_wavefront(child, MAN_ON_DEADLOCK, DIAMOND);
            }
            else
            {
                wavefront_obstacle = wavefront_ptr.get_wavefront(child, MAN, DIAMOND);
            }

            switch(i)
            {
                case 0:
                    push_direction = make_pair(diamond.first-1,diamond.second);
                    if(wavefront_obstacle[diamond.first-1][diamond.second] > 0)
                    {
                        if(validate_push_direction(diamond, push_direction, wavefront_obstacle))
                        {
                            child.man = diamond;
                            child.diamonds.at(diamond_num).first += 1;
                            children.push_back(child);
                            //get_heuristic(child, parent);
                            //sokoban.print(child);
                        }

                    }
                    break;
                case 1:
                    push_direction = make_pair(diamond.first,diamond.second+1);
                    if(wavefront_obstacle[diamond.first][diamond.second+1] > 0)
                    {
                        if(validate_push_direction(diamond, push_direction, wavefront_obstacle))
                        {
                            child.man = diamond;
                            child.diamonds.at(diamond_num).second -= 1;
                            children.push_back(child);
                            //get_heuristic(child, parent);
                            //sokoban.print(child);
                        }

                    }
                    break;
                case 2:
                    push_direction = make_pair(diamond.first+1,diamond.second);
                    if(wavefront_obstacle[diamond.first+1][diamond.second] > 0)
                    {
                        if(validate_push_direction(diamond, push_direction, wavefront_obstacle))
                        {
                            child.man = diamond;
                            child.diamonds.at(diamond_num).first -= 1;
                            children.push_back(child);
                            //get_heuristic(child, parent);
                            //sokoban.print(child);
                        }

                    }
                    break;
                case 3:
                    push_direction = make_pair(diamond.first,diamond.second-1);
                    if(wavefront_obstacle[diamond.first][diamond.second-1] > 0)
                    {
                        if(validate_push_direction(diamond, push_direction, wavefront_obstacle))
                        {
                            child.man = diamond;
                            child.diamonds.at(diamond_num).second += 1;
                            children.push_back(child);
                            //get_heuristic(child, parent);
                            //sokoban.print(child);
                        }

                    }
                    break;
                default:
                    break;
            }
        }
        diamond_num++;
    }
}

bool a_star::compare_heuristic(state_s A, state_s B)
{
    return (A.heuristic > B.heuristic);
}

string a_star::solve()
{
    string path;
    state_s current;

    vector<state_s> children;
    //priority_queue<state_s, vector<state_s>, comp> open(compare_heuristic);
    std::set<state_s,comp> open(compare_heuristic);
    map<string, state_s> closed;

    //vector<state_s>::iterator open_it;
    //vector<state_s>::iterator closed_it;

    initial.parent = NULL;
    open.insert(initial);
    cout << initial.heuristic << endl;
    int cost = 0, count = 0;

    while(*open.begin() != final)
    {
        count++;
        current = *open.begin();
        open.erase(*open.begin());
        closed.emplace(stringify(current), current);

        get_children(current, children);

//        cout << "Current state info:" << endl;
//        sokoban.print(current);
//        cout << "Cost: " << current.cost << endl;
//        cout << "Rank: " << current.heuristic << endl;
//        cout << "open.size(): " << open.size() << " closed.size(): " << closed.size() << endl;


        for(state_s child : children)
        {
            cost = current.cost + get_move_cost(child, current);

            //open_it = find_if(open.begin(), open.end(), state_s(child));
            auto open_it = open.find(child);
            if( open.find(child) != open.end() && cost < open_it->cost)
            {
                cout << "erased from open" << endl;
                open.erase(child);
            }
            //closed_it = find_if(closed.begin(), closed.end(), state_s(child));
            if( closed.find(stringify(child)) != closed.end() && cost < closed.find(stringify(child))->second.cost)
            {
                closed.erase(stringify(child));
//                cout << "erased from closed" << endl;
                continue;
            }
            //open_it = find_if(open.begin(), open.end(), state_s(child));
            if(open.find(child) == open.end() && closed.find(stringify(child)) == closed.end()) //is end() == state?
            {
                child.cost = cost;
                child.heuristic = child.cost + get_heuristic(child);
                child.parent = &current;
                open.insert(child);
//                cout << "child pushed0" << endl;
            }
            //sort(open.begin(), open.end(), compare_heuristic);

            sokoban.print(child);
            cout << endl;
            cout << "get_heuristic() = " << get_heuristic(child) << endl;
            cout << "Cost: " << child.cost << endl;
            cout << "Rank: " << child.heuristic << endl;
            cout << "open.size(): " << open.size() << " closed.size(): " << closed.size() << endl;

            /*cout << open.back().heuristic << endl;
            for(state_s state : open)
            {
                cout << state.heuristic << " : " ;
            }
            cout << endl;*/
        }
        cout << "------------------------------------" << endl;
        children.clear();
    }
    cout << count << endl;

    cout << endl;
    cout << endl;
    cout << "i was solved using this state: " << endl;
    sokoban.print(*open.begin());
    cout << "final state: " << endl;
    sokoban.print_final(final);
    //(print_solution(&final_state);
    return "jubii jeg er løst";
}

bool a_star::validate_push_direction(diamond_t diamond, position_t push_direction, int** wavefront_obstacle)
{
    bool result;
    int direction_row = diamond.first - push_direction.first;
    int direction_col = diamond.second - push_direction.second;

    if(direction_row != 0)
    {
       // cout << "row: "<< (direction_row)  << endl;
       // cout <<"row: "<< diamond.first + direction_row << "," << diamond.second << " : " << sokoban.get_map()[diamond.first + direction_row][diamond.second] << endl;
        if(wavefront_obstacle[diamond.first+1][diamond.second] == -1 ||
                wavefront_obstacle[diamond.first-1][diamond.second] == -1 ||
                sokoban.get_map()[diamond.first + direction_row][diamond.second] == DEADLOCK)
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
      //  cout << "col: "<< (direction_col)  << endl;
      //  cout <<"col: "<< diamond.first  << "," << diamond.second + direction_col << " : " << sokoban.get_map()[diamond.first][diamond.second + direction_col] << endl;
      //  cout << "first: "<< wavefront_obstacle[diamond.first][diamond.second+1]  << endl;
      //  cout << "second: "<< wavefront_obstacle[diamond.first][diamond.second-1]  << endl;

        if(wavefront_obstacle[diamond.first][diamond.second+1] == -1 ||
            wavefront_obstacle[diamond.first][diamond.second-1] == -1 ||
            sokoban.get_map()[diamond.first][diamond.second + direction_col] == DEADLOCK)
        {

            result = false;
           // cout << "false" << endl;
        }
        else
        {
            result = true;

        }
    }
    return result;
}

int a_star::get_heuristic(state_s &child)
{
    int diamond_to_goal_cost, heuristic = 0;

    for(diamond_t diamond : child.diamonds)
    {
        diamond_to_goal_cost = wavefront_ptr.get_wavefront(child,GOAL)[diamond.first][diamond.second];
        heuristic += diamond_to_goal_cost;
    }
    return heuristic;
}

int a_star::get_move_cost(state_s &child, state_s &parent)
{
    for(int i = 0; i < child.diamonds.size(); i++)
    {
        if(child.diamonds.at(i) != parent.diamonds.at(i))
        {
            return wavefront_ptr.get_wavefront(parent,MAN,DIAMOND)[parent.diamonds.at(i).first + (parent.diamonds.at(i).first-child.diamonds.at(i).first)]
                                                           [parent.diamonds.at(i).second + (parent.diamonds.at(i).second-child.diamonds.at(i).second)];
        }
    }
    return 0;
}

void a_star::print_solution(state_s *final_state)
{
    while(final_state->parent != NULL)
    {
        print_solution(final_state->parent);
    }
    sokoban.print(*final_state);
}
