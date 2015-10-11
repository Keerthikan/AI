//
//  sokoban.h
//  mapLoading
//
//  Created by Keerthikan Ratnarajah on 11/10/15.
//  Copyright (c) 2015 Keerthikan Ratnarajah. All rights reserved.
//

#ifndef mapLoading_sokoban_h
#define mapLoading_sokoban_h
#include <string>

class Sokoban{
    int row;
    int col;
    int diamonds;
    char **board;
public:
    Sokoban(std::string file);
    int getRow(){
        return row;
    }
    int getCol(){
        return col;
    }
    int getDiamonds(){
        return diamonds;
    }
    char getChar(int x, int y){
        return board[y][x];
    }
};

#endif
