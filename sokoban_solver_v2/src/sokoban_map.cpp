#include "sokoban_map.hpp"
#include <fstream>
#include <iostream>

using namespace std;

void sokoban_map::delete_map(char** state_map)
{
    for(int i = 0; i < get_row(); i++)
    {
        delete state_map[i];
    }
    delete state_map;
}

char** sokoban_map::get_map(state_s state)
{
    char** return_map;
    return_map = new char*[row];

    for (int i = 0; i < row; i++) {
        return_map[i] = new char[col];
        for (int j = 0; j < col; j++) {
            return_map[i][j] = map[i][j];
        }
    }
    if(return_map[state.man.first][state.man.second] == '.')
    {
        return_map[state.man.first][state.man.second] = 'M';
    }
    else if(return_map[state.man.first][state.man.second] == 'G')
    {
        return_map[state.man.first][state.man.second] = 'm';
    }
    else if(return_map[state.man.first][state.man.second] == 'D')
    {
        return_map[state.man.first][state.man.second] = 'd';
    }
    for(diamond_t diamond : state.diamonds)
    {
        if(return_map[diamond.first][diamond.second] == '.')
        {
            return_map[diamond.first][diamond.second] = 'J';
        }
        else if(return_map[diamond.first][diamond.second] == 'G')
        {
            return_map[diamond.first][diamond.second] = 'j';
        }
    }
    return return_map;
}

state_s sokoban_map::get_final_state()
{
    state_s state;
    //state.man.first = 0;
    //state.man.second = 0;

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

void sokoban_map::deadlock_detection()
{

    //Detect corners
    vector<position_t> corners;
    for(int i = 1 ;  i< get_row() ; i++)
    {
        int corner = 0;
        for(int j = 1 ; j < get_col() ;  j++)
        {
            if(map[i][j] == '.')
            {
                if(map[i-1][j] == 'X' &&  map[i][j-1] == 'X'  && map[i-1][j-1] == 'X' )
                {
                    corner++;
                }



                if(map[i+1][j] == 'X' &&  map[i][j-1] == 'X'&& map[i+1][j-1] == 'X' )
                {
                    corner++;
                }


                if(map[i+1][j] == 'X' &&  map[i][j+1] == 'X' && map[i+1][j+1] == 'X' )
                {
                    corner++;
                }


                if(map[i-1][j] == 'X' &&  map[i][j+1] == 'X' && map[i-1][j+1] == 'X' )
                {
                    corner++;
                }


                if(corner == 1 )
                {
                    corners.push_back(make_pair(i,j));
                    cout << "i: "<< i << " j: " << j << endl;
                    if(map[i][j] == 'M')
                    {
                        map[i][j] = 'm';
                    }
                    else
                    {
                        map[i][j] = 'D';
                    }
                    corner = 0;

                }
            }
        }
    }


    for(position_t corner : corners)
    {
        position_t first_corner = corner;
        position_t second_corner = make_pair(NULL,NULL);

        // check for rows
        bool second_corner_found = false;
        for(position_t second_corner_ : corners)
        {
            if(first_corner.first == second_corner_.first && first_corner.second < second_corner_.second)
            {
                second_corner = second_corner_;
                second_corner_found = true;
                break;
            }

        }

        // check elements in row
        if(second_corner_found)
        {
            bool deadlock_row = true;
            while(first_corner != second_corner)
            {
                first_corner.second++;
                if((map[first_corner.first][first_corner.second] == 'J' || map[first_corner.first][first_corner.second] == 'G') && (map[first_corner.first+1][first_corner.second] != 'X' || map[first_corner.first-1][first_corner.second] != 'X')   )
                {
                    deadlock_row = false;
                }

            }

            first_corner = corner;

            if(deadlock_row)
            {
                while(first_corner != second_corner)
                {
                    if(map[first_corner.first][first_corner.second] == 'M')
                    {
                        map[first_corner.first][first_corner.second]  = 'd';
                    }
                    else if(map[first_corner.first][first_corner.second] == 'X')
                    {
                        map[first_corner.first][first_corner.second]  = 'X';
                    }
                    else
                    {
                        map[first_corner.first][first_corner.second] = 'D';
                    }
                    cout << "deadlocked row: " << first_corner.first << "," <<first_corner.second << endl;
                    first_corner.second++;
                }
            }



            second_corner_found = false;
            deadlock_row = false;
        }

        first_corner = corner;
        second_corner = make_pair(NULL,NULL);

        // check for cols
        second_corner_found = false;
        for(position_t second_corner_ : corners)
        {
            if(first_corner.second == second_corner_.second && first_corner.first <  second_corner_.first)
            {
                second_corner = second_corner_;
                second_corner_found = true;
                break;
            }

        }
        if(second_corner_found)
        {
            bool deadlock_col = true;
            while(first_corner != second_corner)
            {
                first_corner.first++;

                if((map[first_corner.first][first_corner.second] == 'J' || map[first_corner.first][first_corner.second] == 'G') && (map[first_corner.first][first_corner.second+1] != 'X' || map[first_corner.first][first_corner.second-1] != 'X')  )
                {
                    deadlock_col = false;
                }

            }

            first_corner = corner;

            if(deadlock_col)
            {
                while(first_corner != second_corner)
                {
                    if(map[first_corner.first][first_corner.second] == 'M')
                    {
                        map[first_corner.first][first_corner.second]  = 'd';
                    }
                    else if(map[first_corner.first][first_corner.second] == 'X')
                    {
                        map[first_corner.first][first_corner.second]  = 'X';
                    }
                    else
                    {
                        map[first_corner.first][first_corner.second] = 'D';
                    }
                    cout << "deadlocked col: " << first_corner.first << "," <<first_corner.second<< endl;
                    first_corner.first++;
                }
            }

            second_corner_found = false;
            deadlock_col = false;
        }

    }

    cout << corners.size() << endl;
    cout << "done" << endl;
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
        new_diamond_position = make_pair(diamond.first,diamond.second + (dif_col *(-1)));
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
            if(map[i][j] == 'M' ||  map[i][j] == 'd')
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
                map[i][j] = 'G';
            }
            else if(map[i][j] == 'd' )
            {
                map[i][j] = 'D';
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

void sokoban_map::print_final(state_s final)
{
    count++;
    cout << count << endl;

    for(int i = 0; i < final.diamonds.size(); i++)
    {
        if(map[final.diamonds.at(i).first][final.diamonds.at(i).second] == 'G')
        {
            map[final.diamonds.at(i).first][final.diamonds.at(i).second] = 'j';
        }
        else
        {
            map[final.diamonds.at(i).first][final.diamonds.at(i).second] = 'J';
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

void sokoban_map::print(state_s state)
{
    //count++;
    //cout << count << endl;
    if(map[state.man.first][state.man.second] == 'G')
    {
        map[state.man.first][state.man.second] ='m';
    }
    else if(map[state.man.first][state.man.second] == 'D')
    {
        map[state.man.first][state.man.second] ='d';
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
    //cout << "----------------------------" << endl;
    clear_map();
}

sokoban_map::sokoban_map(string file)
{
    count = 0;
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
