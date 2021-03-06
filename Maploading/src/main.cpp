//
//  main.cpp
//  mapLoading
//
//  Created by Keerthikan Ratnarajah on 07/10/15.
//  Copyright (c) 2015 Keerthikan Ratnarajah. All rights reserved.
//

#include <iostream>
#include<fstream>
#include<string>

using namespace std;

int main(int argc, const char * argv[]) {
    int row;
    int col;
    int diamonds;
    ifstream myReadFile;
    myReadFile.open("/home/student/AI/Maploading/src/map.txt");
    myReadFile >> row;
    myReadFile >> col;
    myReadFile >> diamonds;
    //cout << row << col << diamonds << endl;
    char board[col][row];
     for (int i = 0; i < col; i++) {
        for (int j = 0; j < row; j++) {
            board[i][j] = ' ';
        }
    }
    //GotoLine(myReadFile, 1);
    string str;
    std::getline(myReadFile, str);
    cout << str << endl;
    
    unsigned int column = 0;
    unsigned int rows = 0;
    while(std::getline(myReadFile, str))
    {
        //cout << str << endl;
        while (rows < str.length())
        {
	    cout << "At: " << column << "," << rows <<" "<< str.at(rows) << endl;
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
    
    std::cout << "Hello, World!\n";
    return 0;
}

