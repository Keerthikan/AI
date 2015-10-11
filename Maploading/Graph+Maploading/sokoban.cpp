//
//  sokoban.cpp
//  mapLoading
//
//  Created by Keerthikan Ratnarajah on 11/10/15.
//  Copyright (c) 2015 Keerthikan Ratnarajah. All rights reserved.
//

#include <iostream>
#include "sokoban.h"
#include <string>
#include<fstream>


using namespace std;
Sokoban::Sokoban(string file)
{
    ifstream myReadFile;
    myReadFile.open(file);
    myReadFile >> row;
    myReadFile >> col;
    myReadFile >> diamonds;
    //cout << row << col << diamonds << endl;
    board = new char*[row];
    for (int i = 0; i < col; i++) {
        board[i] = new char[row];
    }
    //char board[col][row];
    for (int i = 0; i < col; i++) {
        for (int j = 0; j < row; j++) {
            board[i][j] = ' ';
        }
    }
    //GotoLine(myReadFile, 1);
    string str;
    std::getline(myReadFile, str);
    cout << str << endl;
    
    int column = 0;
    int rows = 0;
    while(std::getline(myReadFile, str))
    {
        //cout << str << endl;
        while (rows < str.length())
        {
            //cout << "lenght of string: " << str.length() << " at: " << rows << endl;
            board[column][rows] = str.at(rows);
            
            rows++;
        }
        rows = 0;
        column++;
    }
    for (int i = 0; i < col;  i++) {
        for (int j  = 0; j < row; j++) {
            cout << board[i][j];
        }
        cout << endl;
    }
    cout << endl;
    cout << endl;
    
    cout << "Map loaded" << endl;
    
}