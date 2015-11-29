#include "wavefront.hpp"
#include "a_star.hpp"
#include <queue>
#include <iostream>

using namespace std;

void wavefront::print_wavefront(int **wf)
{
    for(int i = 0; i < map->get_row(); i++)
    {
        for(int j = 0; j < map->get_col(); j++)
        {
            cout << wf[i][j] << " ";
        }
        cout << endl;
    }
    cout <<"--------------------------"<< endl;
}

int** wavefront::init_array()
{
    int** map_array = new int*[map->get_col()];
    for(int i = 0; i < map->get_row(); i++)
    {
        map_array[i] = new int[map->get_col()];
        for(int j = 0; j < map->get_col(); j++)
        {
            map_array[i][j] = 0;
        }
    }
    return map_array;
}

wavefront::wavefront(sokoban_map *map)
{
    this->map = map;
    wavefront_map = init_array();
}

void wavefront::impose_state(state_s state)
{
    if(map->get_map()[state.man.first][state.man.second] == 'G')
    {
        map->get_map()[state.man.first][state.man.second] = 'm';
    }
    else if(map->get_map()[state.man.first][state.man.second] == 'D' || map->get_map()[state.man.first][state.man.second] == 'd')
    {
        map->get_map()[state.man.first][state.man.second] = 'd';
    }
    else
    {
        map->get_map()[state.man.first][state.man.second] = 'M';
    }
    for(pair<int,int> diamond : state.diamonds)
    {
        if(map->get_map()[diamond.first][diamond.second] == 'G')
        {
            map->get_map()[diamond.first][diamond.second] = 'j';
        }
        else
        {
            map->get_map()[diamond.first][diamond.second] = 'J';
        }
    }
}

void wavefront::clear_state()
{
    for(int i = 0; i < map->get_row(); i++)
    {
        for(int j = 0; j < map->get_col(); j++)
        {
            if(map->get_map()[i][j] == 'J' || map->get_map()[i][j] == 'M' )
            {
                map->get_map()[i][j] = '.';
            }
            else if(map->get_map()[i][j] == 'j' || map->get_map()[i][j] == 'm')
            {
                map->get_map()[i][j] = 'G';
            }
            else if(map->get_map()[i][j] == 'd')
            {
                map->get_map()[i][j] = 'D';
            }

        }
    }
}

int** wavefront::get_wavefront(state_s state, char initiator)
{
    int** wf = init_array();
    impose_state(state);

    queue<pair<int,int>> open;

    for(int i = 0; i < map->get_row(); i++)
    {
        for(int j = 0; j < map->get_col(); j++)
        {
            if(initiator == 'G' && map->get_map()[i][j] == 'j')
            {
                wf[i][j] = 1;
                open.push(make_pair(i,j));
            }
            if(map->get_map()[i][j] == initiator)
            {
                wf[i][j] = 1;
                open.push(make_pair(i,j));
            }
            else if(map->get_map()[i][j] == 'X')
            {
                wf[i][j] = -1;
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
                    if(wf[wave.first-1][wave.second] == 0)
                    {
                        open.push(make_pair(wave.first-1, wave.second));
                        wf[wave.first-1][wave.second] = wf[wave.first][wave.second] + 1;
                    }
                    break;
                case 1:
                    if(wf[wave.first][wave.second+1] == 0)
                    {
                        open.push(make_pair(wave.first, wave.second+1));
                        wf[wave.first][wave.second+1] = wf[wave.first][wave.second] + 1;
                    }
                    break;
                case 2:
                    if(wf[wave.first+1][wave.second] == 0)
                    {
                        open.push(make_pair(wave.first+1, wave.second));
                        wf[wave.first+1][wave.second] = wf[wave.first][wave.second] + 1;
                    }
                    break;
                case 3:
                    if(wf[wave.first][wave.second-1] == 0)
                    {
                        open.push(make_pair(wave.first, wave.second-1));
                        wf[wave.first][wave.second-1] = wf[wave.first][wave.second] + 1;
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

int** wavefront::get_wavefront(state_s state, char initiator, char obstacle)
{
    int** wf = init_array();
    impose_state(state);

    char obstacle_other_type;


    queue<pair<int,int>> open;

    for(int i = 0; i < map->get_row(); i++)
    {
        for(int j = 0; j < map->get_col(); j++)
        {
            if(map->get_map()[i][j] == initiator)
            {
                wf[i][j] = 1;
                open.push(make_pair(i,j));
            }
            else if(map->get_map()[i][j] == 'X' || map->get_map()[i][j] == obstacle || map->get_map()[i][j] == DIAMOND_ON_GOAL)
            {

                wf[i][j] = -1;
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
                    if(wf[wave.first-1][wave.second] == 0)
                    {
                        open.push(make_pair(wave.first-1, wave.second));
                        wf[wave.first-1][wave.second] = wf[wave.first][wave.second] + 1;
                    }
                    break;
                case 1:
                    if(wf[wave.first][wave.second+1] == 0)
                    {
                        open.push(make_pair(wave.first, wave.second+1));
                        wf[wave.first][wave.second+1] = wf[wave.first][wave.second] + 1;
                    }
                    break;
                case 2:
                    if(wf[wave.first+1][wave.second] == 0)
                    {
                        open.push(make_pair(wave.first+1, wave.second));
                        wf[wave.first+1][wave.second] = wf[wave.first][wave.second] + 1;
                    }
                    break;
                case 3:
                    if(wf[wave.first][wave.second-1] == 0)
                    {
                        open.push(make_pair(wave.first, wave.second-1));
                        wf[wave.first][wave.second-1] = wf[wave.first][wave.second] + 1;
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

int** wavefront::get_wavefront(state_s state, char initiator, char obstacle, char obstacle_)
{
    int** wf = init_array();
    impose_state(state);

    queue<pair<int,int>> open;

    for(int i = 0; i < map->get_row(); i++)
    {
        for(int j = 0; j < map->get_col(); j++)
        {
            if(map->get_map()[i][j] == initiator)
            {
                wf[i][j] = 1;
                open.push(make_pair(i,j));
            }
            else if(map->get_map()[i][j] == 'X' ||
                    map->get_map()[i][j] == obstacle ||
                    map->get_map()[i][j] == obstacle_ ||
                    map->get_map()[i][j] == DIAMOND_ON_GOAL)
            {

                wf[i][j] = -1;
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
                    if(wf[wave.first-1][wave.second] == 0)
                    {
                        open.push(make_pair(wave.first-1, wave.second));
                        wf[wave.first-1][wave.second] = wf[wave.first][wave.second] + 1;
                    }
                    break;
                case 1:
                    if(wf[wave.first][wave.second+1] == 0)
                    {
                        open.push(make_pair(wave.first, wave.second+1));
                        wf[wave.first][wave.second+1] = wf[wave.first][wave.second] + 1;
                    }
                    break;
                case 2:
                    if(wf[wave.first+1][wave.second] == 0)
                    {
                        open.push(make_pair(wave.first+1, wave.second));
                        wf[wave.first+1][wave.second] = wf[wave.first][wave.second] + 1;
                    }
                    break;
                case 3:
                    if(wf[wave.first][wave.second-1] == 0)
                    {
                        open.push(make_pair(wave.first, wave.second-1));
                        wf[wave.first][wave.second-1] = wf[wave.first][wave.second] + 1;
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
