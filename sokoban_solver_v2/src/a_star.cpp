#include "a_star.hpp"
#include <iostream>
#include <queue>
#include <map>
#include <algorithm>
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
    int **wavefront_obstacle;
    char** map;

    for(diamond_t diamond : parent.diamonds)
    {


        for(int i = 0; i < 4; i++)
        {
            position_t push_direction;
            state_s child = parent;
            child.heuristic = 0;
            map = sokoban.get_map(child);
            if(map[child.man.first][child.man.second] == 'd')
            {
                wavefront_obstacle = wavefront_ptr.get_wavefront(child, MAN_ON_DEADLOCK, DIAMOND);
            }
            else if(map[child.man.first][child.man.second] == 'm')
            {
                wavefront_obstacle = wavefront_ptr.get_wavefront(child, MAN_ON_GOAL, DIAMOND);
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
            sokoban.delete_map(map);
            wavefront_ptr.delete_wavefront(wavefront_obstacle);
        }
        diamond_num++;
    }
}

bool a_star::compare_heuristic(state_s A, state_s B)
{
    return (A.heuristic >= B.heuristic);
}

string a_star::solve()
{
    string path;
    state_s current;

    vector<state_s> children;
    priority_queue<state_s, vector<state_s>, comp> open(compare_heuristic);
    map<string, state_s> closed;

    vector<state_s>::iterator open_it;
    //vector<state_s>::iterator closed_it;

    initial.parent = NULL;
    open.push(initial);
    cout << initial.heuristic << endl;
    int cost = 0, count = 0;

    while(open.top() != final)
    {
        count++;
        current = open.top();
        open.pop();
        closed.emplace(stringify(current), current);

        get_children(current, children);

//        cout << "Current state info:" << endl;
//        sokoban.print(current);
//        cout << "Cost: " << current.cost << endl;
//        cout << "Rank: " << current.heuristic << endl;
//        cout << "open.size(): " << open.size() << " closed.size(): " << closed.size() << endl;

        /*if(current.man.first == 2 && current.man.second == 7  &&
                        current.diamonds.at(0).first == 3 &&
                        current.diamonds.at(0).second == 2  &&
                        current.diamonds.at(1).first == 2 &&
                        current.diamonds.at(1).second == 3 &&
                        current.diamonds.at(2).first == 3 &&
                        current.diamonds.at(2).second == 4 &&
                        current.diamonds.at(3).first == 2 &&
                        current.diamonds.at(3).second == 8)
            {
            cout << "current found: " << endl;
            sokoban.print(current);
            cout << endl;
            cout << endl;
            for(state_s child :  children)
            {
                sokoban.print(child);
                cout << endl;
                cout << "get_heuristic() = " << get_heuristic(child) << endl;
                cout << "Cost: " << child.cost << endl;
                cout << "Rank: " << child.heuristic << endl;
                cout << "open.size(): " << open.size() << " closed.size(): " << closed.size() << endl;

            }
            cout << "-----------------------------" << endl;
            break;
        }*/

        for(state_s child : children)
        {


            cost = current.cost + get_move_cost(child, current);

//            open_it = open.find(state_s(child));
//            if(open_it != open.end())
//            {
//                cout << "I already exist" << endl;
//            }

//            if( open_it != open.end() && cost < (open.find(child))->cost)
//            {
//                cout << "erased from open" << endl;
//                open.erase(open_it);
//            }

            if( closed.find(stringify(child)) != closed.end() && cost < closed.find(stringify(child))->second.cost)
            {
                closed.erase(stringify(child));
                continue;
            }

            if( closed.find(stringify(child)) == closed.end()) //is end() == state?
            {
                child.cost = cost;
                child.heuristic = child.cost + get_heuristic(child);
                child.parent = &current;
                open.push(child);

            }

//            sokoban.print(child);
//            cout << endl;
//            cout << "get_heuristic() = " << get_heuristic(child) << endl;
//            cout << "Cost: " << child.cost << endl;
//            cout << "Rank: " << child.heuristic << endl;
//            cout << "open.size(): " << open.size() << " closed.size(): " << closed.size() << endl;
        }
//      cout << "------------------------------------" << endl;
        children.clear();
    }
    cout << count << endl;

    cout << endl;
    cout << endl;
    cout << "i was solved using this state: " << endl;
    sokoban.print(open.top());
    state_s final_state = open.top();
    cout << "final state: " << endl;
    sokoban.print_final(final);
    //print_solution(&final_state);
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
    int** wf;
    int diamond_to_goal_cost, heuristic = 0;

    for(diamond_t diamond : child.diamonds)
    {
        wf = wavefront_ptr.get_wavefront(child,GOAL);
        diamond_to_goal_cost = wf[diamond.first][diamond.second];
        heuristic += diamond_to_goal_cost;
        wavefront_ptr.delete_wavefront(wf);
    }
    return heuristic;
}

int a_star::get_move_cost(state_s &child, state_s &parent)
{
    int** wf;
    int move_cost;
    for(int i = 0; i < child.diamonds.size(); i++)
    {
        if(child.diamonds.at(i) != parent.diamonds.at(i))
        {
            wf = wavefront_ptr.get_wavefront(parent,MAN,DIAMOND);
            move_cost = wf[parent.diamonds.at(i).first + (parent.diamonds.at(i).first-child.diamonds.at(i).first)]
                                                           [parent.diamonds.at(i).second + (parent.diamonds.at(i).second-child.diamonds.at(i).second)];
            wavefront_ptr.delete_wavefront(wf);
            return move_cost;
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
