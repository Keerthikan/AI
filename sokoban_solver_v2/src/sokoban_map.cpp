#include "sokoban_map.hpp"
#include <fstream>
#include <iostream>

using namespace std;

state_s sokoban_map::get_final_state()
{
    state_s state;
    state.man.first = 0;
    state.man.second = 0;

    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            if(map[i][j] == 'G')
            {
                state.diamonds.push_back(make_pair(i,j));
            }
        }
    }
    return state;
}


state_s sokoban_map::get_child( state_s current, diamond_t diamond, position_t push_direction)
{
    state_s child;

    int dif_row = diamond.first - push_direction.first;
    int dif_col = diamond.second - push_direction.second;
    diamond_t new_diamond_position;
    if(dif_row != 0)
    {
        new_diamond_position = make_pair(diamond.first + (dif_row *(-1)),diamond.second);
    }
    else if(dif_col != 0)
    {
        new_diamond_position = make_pair(diamond.first,diamond.second + + (dif_col *(-1)));
    }

    int diamond_position;

    for(diamond_t current_diamond: current.diamonds)
    {
        if(current_diamond == diamond)
        {
            break;
        }
        diamond_position++;
    }

    child.man = current.diamonds.at(diamond_position);
    child.diamonds.at(diamond_position) = new_diamond_position;

    return child;
}

state_s sokoban_map::get_init_state()
{
    state_s state;
    state.heuristic = 0;
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            if(map[i][j] == 'M')
            {
                state.man = make_pair(i,j);
            }
            else if(map[i][j] == 'J')
            {
                state.diamonds.push_back(make_pair(i,j));
            }
        }
    }
    //clear_map();
    return state;
}


void sokoban_map::clear_map()
{

    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            if(map[i][j] == 'M' || map[i][j] == 'J' )
            {
                map[i][j] = '.';
            }
            else if(map[i][j] == 'j' || map[i][j] == 'm' )
            {
                map[i][j] = 'g';
            }
        }
    }
}



void sokoban_map::print()
{
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            cout << map[i][j] << " ";
        }
        cout << endl;
    }
    cout << "----------------------------" << endl;
}

void sokoban_map::print(state_s state)
{

    if(map[state.man.first][state.man.second] == 'G')
    {
        map[state.man.first][state.man.second] ='m';
    }
    else
    {
        map[state.man.first][state.man.second] ='M';
    }

    for(int i = 0; i < state.diamonds.size(); i++)
    {
        if(map[state.diamonds.at(i).first][state.diamonds.at(i).second] == 'G')
        {
            map[state.diamonds.at(i).first][state.diamonds.at(i).second] = 'j';
        }
        else
        {
            map[state.diamonds.at(i).first][state.diamonds.at(i).second] = 'J';
        }
    }
    cout << endl;

    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            cout << map[i][j] << " ";
        }
        cout << endl;
    }
    cout << "----------------------------" << endl;
    clear_map();
}

sokoban_map::sokoban_map(string file)
{
    ifstream myReadFile;
    myReadFile.open(file.c_str());
    myReadFile >> col;
    myReadFile >> row;
    myReadFile >> diamonds;
    cout << col << row << diamonds << endl;
    map = new char*[row];

    for (int i = 0; i < row; i++) {
        map[i] = new char[col];
        for (int j = 0; j < col; j++) {
            map[i][j] = ' ';
        }
    }

    //GotoLine(myReadFile, 1);
    string str;
    getline(myReadFile, str);
    cout << str << endl;

    int column = 0;
    int rows = 0;
    while(getline(myReadFile, str))
    {
        //cout << str << endl;
        while ((unsigned)column < (unsigned)str.length())
        {
            //cout << "lenght of string: " << str.length() << " at: " << rows << endl;

            map[rows][column] = str.at(column);

            column++;
        }
        column = 0;
        rows++;
    }

    cout << "Map loaded" << endl;
}
