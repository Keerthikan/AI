#include "wavefront.hpp"
#include "a_star.hpp"
#include <queue>
#include <iostream>

using namespace std;

void wavefront::print_wavefront(matrix_t wf)
{
    for(int i = 0; i < map->get_row(); i++)
    {
        for(int j = 0; j < map->get_col(); j++)
        {
            cout << wf.at(i).at(j) << " ";
        }
        cout << endl;
    }
    cout <<"--------------------------"<< endl;
}

void wavefront::init_array(matrix_t matrix)
{
    matrix.resize(map->get_row(),vector<int>(map->get_col(),0));
}


wavefront::wavefront(sokoban_map *map)
{
    this->map = map;
    init_array(wavefront_map);
}

void wavefront::impose_state(state_s state)
{
    if(map->get_map().at(state.man.first).at(state.man.second) == 'G')
    {
        map->get_map().at(state.man.first).at(state.man.second) = 'm';
    }
    else if(map->get_map().at(state.man.first).at(state.man.second) == 'D' || map->get_map().at(state.man.first).at(state.man.second) == 'd')
    {
        map->get_map().at(state.man.first).at(state.man.second) = 'd';
    }
    else
    {
        map->get_map().at(state.man.first).at(state.man.second) = 'M';
    }
    for(pair<int,int> diamond : state.diamonds)
    {
        if(map->get_map().at(diamond.first).at(diamond.second) == 'G')
        {
            map->get_map().at(diamond.first).at(diamond.second) = 'j';
        }
        else
        {
            map->get_map().at(diamond.first).at(diamond.second) = 'J';
        }
    }
}

void wavefront::clear_state()
{
    for(int i = 0; i < map->get_row(); i++)
    {
        for(int j = 0; j < map->get_col(); j++)
        {
            if(map->get_map().at(i).at(j) == 'J' || map->get_map().at(i).at(j) == 'M' )
            {
                map->get_map().at(i).at(j) = '.';
            }
            else if(map->get_map().at(i).at(j) == 'j' || map->get_map().at(i).at(j) == 'm')
            {
                map->get_map().at(i).at(j) = 'G';
            }
            else if(map->get_map().at(i).at(j) == 'd')
            {
                map->get_map().at(i).at(j) = 'D';
            }

        }
    }
}

matrix_t wavefront::get_wavefront(state_s state, char initiator)
{
    matrix_t wf(map->get_row(),vector<int>(map->get_col(),0));
    //init_array(wf);
    impose_state(state);

    queue<pair<int,int>> open;

    for(int i = 0; i < map->get_row(); i++)
    {
        for(int j = 0; j < map->get_col(); j++)
        {
            if(initiator == 'G' && map->get_map().at(i).at(j) == 'j')
            {
                wf.at(i).at(j) = 1;
                open.push(make_pair(i,j));
            }
            if(map->get_map().at(i).at(j) == initiator)
            {
                wf.at(i).at(j) = 1;
                open.push(make_pair(i,j));
            }
            else if(map->get_map().at(i).at(j) == 'X')
            {
                wf.at(i).at(j) = -1;
            }
        }
    }

    while(!open.empty())
    {
        pair<int,int> wave = open.front();
        open.pop();
        for(int i = 0; i < 4; i++)
        {
            switch(i)
            {
                case 0:
                    if(wf.at(wave.first-1).at(wave.second) == 0)
                    {
                        open.push(make_pair(wave.first-1, wave.second));
                        wf.at(wave.first-1).at(wave.second) = wf.at(wave.first).at(wave.second) + 1;
                    }
                    break;
                case 1:
                    if(wf.at(wave.first).at(wave.second+1) == 0)
                    {
                        open.push(make_pair(wave.first, wave.second+1));
                        wf.at(wave.first).at(wave.second+1) = wf.at(wave.first).at(wave.second) + 1;
                    }
                    break;
                case 2:
                    if(wf.at(wave.first+1).at(wave.second) == 0)
                    {
                        open.push(make_pair(wave.first+1, wave.second));
                        wf.at(wave.first+1).at(wave.second) = wf.at(wave.first).at(wave.second) + 1;
                    }
                    break;
                case 3:
                    if(wf.at(wave.first).at(wave.second-1) == 0)
                    {
                        open.push(make_pair(wave.first, wave.second-1));
                        wf.at(wave.first).at(wave.second-1) = wf.at(wave.first).at(wave.second) + 1;
                    }
                    break;
                default:
                    break;
            }
        }
    }

    clear_state();
    //print_wavefront(wf);

    return wf;
}

matrix_t wavefront::get_wavefront(state_s state, char initiator, char obstacle)
{
    matrix_t wf(map->get_row(),vector<int>(map->get_col(),0));
    impose_state(state);

    char obstacle_other_type;


    queue<pair<int,int>> open;

    for(int i = 0; i < map->get_row(); i++)
    {
        for(int j = 0; j < map->get_col(); j++)
        {
            if(map->get_map().at(i).at(j) == initiator)
            {
                wf.at(i).at(j) = 1;
                open.push(make_pair(i,j));
            }
            else if(map->get_map().at(i).at(j) == 'X' || map->get_map().at(i).at(j) == obstacle || map->get_map().at(i).at(j) == DIAMOND_ON_GOAL)
            {

                wf.at(i).at(j) = -1;
            }
        }
    }

    while(!open.empty())
    {
        pair<int,int> wave = open.front();
        open.pop();
        for(int i = 0; i < 4; i++)
        {
            switch(i)
            {
                case 0:
                    if(wf.at(wave.first-1).at(wave.second) == 0)
                    {
                        open.push(make_pair(wave.first-1, wave.second));
                        wf.at(wave.first-1).at(wave.second) = wf.at(wave.first).at(wave.second) + 1;
                    }
                    break;
                case 1:
                    if(wf.at(wave.first).at(wave.second+1) == 0)
                    {
                        open.push(make_pair(wave.first, wave.second+1));
                        wf.at(wave.first).at(wave.second+1) = wf.at(wave.first).at(wave.second) + 1;
                    }
                    break;
                case 2:
                    if(wf.at(wave.first+1).at(wave.second) == 0)
                    {
                        open.push(make_pair(wave.first+1, wave.second));
                        wf.at(wave.first+1).at(wave.second) = wf.at(wave.first).at(wave.second) + 1;
                    }
                    break;
                case 3:
                    if(wf.at(wave.first).at(wave.second-1) == 0)
                    {
                        open.push(make_pair(wave.first, wave.second-1));
                        wf.at(wave.first).at(wave.second-1) = wf.at(wave.first).at(wave.second) + 1;
                    }
                    break;
                default:
                    break;
            }
        }
    }
    clear_state();
    //print_wavefront(wf);

    return wf;
}

matrix_t wavefront::get_wavefront(state_s state, char initiator, char obstacle, char obstacle_)
{
    matrix_t wf(map->get_row(),vector<int>(map->get_col(),0));
    impose_state(state);

    queue<pair<int,int>> open;

    for(int i = 0; i < map->get_row(); i++)
    {
        for(int j = 0; j < map->get_col(); j++)
        {
            if(map->get_map().at(i).at(j) == initiator)
            {
                wf.at(i).at(j) = 1;
                open.push(make_pair(i,j));
            }
            else if(map->get_map().at(i).at(j) == 'X' ||
                    map->get_map().at(i).at(j) == obstacle ||
                    map->get_map().at(i).at(j) == obstacle_ ||
                    map->get_map().at(i).at(j) == DIAMOND_ON_GOAL)
            {

                wf.at(i).at(j) = -1;
            }
        }
    }

    while(!open.empty())
    {
        pair<int,int> wave = open.front();
        open.pop();
        for(int i = 0; i < 4; i++)
        {
            switch(i)
            {
                case 0:
                    if(wf.at(wave.first-1).at(wave.second) == 0)
                    {
                        open.push(make_pair(wave.first-1, wave.second));
                        wf.at(wave.first-1).at(wave.second) = wf.at(wave.first).at(wave.second) + 1;
                    }
                    break;
                case 1:
                    if(wf.at(wave.first).at(wave.second+1) == 0)
                    {
                        open.push(make_pair(wave.first, wave.second+1));
                        wf.at(wave.first).at(wave.second+1) = wf.at(wave.first).at(wave.second) + 1;
                    }
                    break;
                case 2:
                    if(wf.at(wave.first+1).at(wave.second) == 0)
                    {
                        open.push(make_pair(wave.first+1, wave.second));
                        wf.at(wave.first+1).at(wave.second) = wf.at(wave.first).at(wave.second) + 1;
                    }
                    break;
                case 3:
                    if(wf.at(wave.first).at(wave.second-1) == 0)
                    {
                        open.push(make_pair(wave.first, wave.second-1));
                        wf.at(wave.first).at(wave.second-1) = wf.at(wave.first).at(wave.second) + 1;
                    }
                    break;
                default:
                    break;
            }
        }
    }
    clear_state();
    //print_wavefront(wf);

    return wf;
}
