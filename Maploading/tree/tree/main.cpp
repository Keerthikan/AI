//
//  main.cpp
//  tree
//
//  Created by Keerthikan Ratnarajah on 18/10/15.
//  Copyright (c) 2015 Keerthikan Ratnarajah. All rights reserved.
//

#include <iostream>
using namespace std;
struct node{
    char data[3][3];
    node* up;
    node* down;
    node* left;
    node* right;
    node* parent; 
    pair<int,int> coord;
    void movePosition()
    {
        for (int i = 0; i < 3; i++) {
            for (int j  = 0; j< 3; j++) {
                if (data[i][j] == 'm')
                {
                    data[i][j] = 'o';
                    cout << "delete previous position: " << i << " , " << j  <<  endl;

                }
                
                else if (i == coord.first && j == coord.second)
                {
                    data[i][j] = 'm';
                    cout << "insert new position: "  <<  i << " , " << j    << endl;

                }
            }
        }
        
    }
    void printArray()
    {
        for(int i = 0; i < 3 ; i++)
        {
            for(int j = 0; j < 3 ; j++)
            {
                cout << data[i][j];
            }
            cout << endl; 
        }
        cout << endl;
    }
    
}*root;

class tree{
public:
    tree(char data[3][3]);
    void insert(pair<int,int> coord);
    void insertp(node* ,pair<int,int> coord);
    void getDepth();
    node* root;
    node* New;
    pair<int, int> getPosition(char data[3][3]);

};
tree::tree(char Data[3][3]) : New(new node)
{
    New = new node;
    New->coord = getPosition(Data);
    New->left = NULL;
    New->down = NULL;
    New->up = NULL;
    New->right = NULL;
    New->parent = NULL;
    std::copy(&Data[0][0], &Data[0][0]+3*3,&New->data[0][0]);
    root = New;
}


pair<int, int> tree::getPosition(char data[3][3])
{
    pair<int, int > result;

    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            if (data[x][y] == 'm') {
                //cout << "value found at " << x << ","<< y << endl;
                result = make_pair(x, y);
            }
        }
    }
    return result;

}
void tree::insert(pair<int,int> coord)
{
    insertp(root, coord);
}


void tree::insertp(node* pointer, pair<int,int> coord)
{
    if (coord.first <  pointer->coord.first)
    {
        if(pointer->left != NULL)
        {
            insertp(pointer->left, coord);
        }
    else
    {
        New = new node;
        pointer->left = New;
        New->coord = coord;
        New->left = NULL;
        New->down = NULL;
        New->up = NULL;
        New->right = NULL;
        New->parent = pointer;
        //New->data[3][3] = pointer->data[3][3];
        std::copy(&pointer->data[0][0], &pointer->data[0][0]+3*3,&New->data[0][0]);
        New->movePosition();
        New->printArray();
    }
    }
   else if( coord.first > pointer->coord.first)
   {
       if(pointer->right != NULL)
       {
           insertp(pointer->right, coord);
       }
   else
   {
       New = new node;
       New->coord = coord;
       pointer->right = New;
       New->left = NULL;
       New->down = NULL;
       New->up = NULL;
       New->right = NULL;
       New->parent = pointer;
       //New->data[3][3] = pointer->data[3][3];
       std::copy(&pointer->data[0][0], &pointer->data[0][0]+3*3,&New->data[0][0]);
       New->movePosition();
       New->printArray();


   }
   }
   else if( coord.second > pointer->coord.second)
   {
       if(pointer->up != NULL)
       {
           insertp(pointer->up, coord);
       }
    else
    {
        New = new node;
        New->coord = coord;
        pointer->up = New;
        New->left = NULL;
        New->down = NULL;
        New->up = NULL;
        New->right = NULL;
        New->parent = pointer;
        //New->data[3][3] = pointer->data[3][3];
        std::copy(&pointer->data[0][0], &pointer->data[0][0]+3*3,&New->data[0][0]);
        New->movePosition();
        New->printArray();


    }
    }
    
   else if( coord.second < pointer->coord.second)
   {
       if(pointer->down != NULL)
       {
           insertp(pointer->down, coord);
           
       }
       else
       {
           New = new node;
           New->coord = coord;
           pointer->down = New;
           New->left = NULL;
           New->down = NULL;
           New->up = NULL;
           New->right = NULL;
           New->parent = pointer;
           std::copy(&pointer->data[0][0], &pointer->data[0][0]+3*3,&New->data[0][0]);
           New->movePosition();
           New->printArray();


       }
   }
}
int main(int argc, const char * argv[]) {
    char board[3][3] = {'o'   ,   'o'   ,   'o',
                        'o'   ,   'o'   ,   'o',
                        'o'   ,   'm'   ,   'o'
                       };
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j<3; j++) {
            cout << board[i][j];
        }
        cout << endl; 
    }
    tree Tree(board);
    int i = 0;
    pair<int, int> start = Tree.root->coord;
    while(i < 3)
    {   
        Tree.insert(make_pair(start.first+i,start.second));
        Tree.insert(make_pair(start.first,start.second-i));
        Tree.insert(make_pair(start.first,start.second+i));
        Tree.insert(make_pair(start.first-i,start.second));
        i++;

    }
    

    
    cout << "Hello World!" << endl;
    return 0;
}
