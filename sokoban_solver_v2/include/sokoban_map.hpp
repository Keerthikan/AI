#ifndef SOKOBAN_MAP_HPP
#define SOKOBAN_MAP_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

typedef std::pair<char,char> position_t;
typedef std::pair<char,char> diamond_t;

struct state_s
{
    state_s() = default;
    state_s(const state_s &obj)
    {
        this->man = obj.man;
        for(diamond_t diamond : obj.diamonds)
        {
            this->diamonds.push_back(diamond);
        }
        this->heuristic = obj.heuristic;
        this->cost = obj.cost;
    }

    position_t man;
    std::vector<diamond_t> diamonds;
    char heuristic = 0, cost = 0;
    state_s *parent;

    bool operator!=(const state_s &rhs ) const
    {
        bool return_val = true;
        for (diamond_t diamond: this->diamonds)
        {

                if (std::find(rhs.diamonds.begin(), rhs.diamonds.end(),diamond) != rhs.diamonds.end())
                {
                    return_val = false;
                }
                else
                {
                    return_val = true;
                    break;
                }

        }
        return return_val;
    }


    bool operator==(const state_s &rhs ) const
    {
        bool ret_val = false;
        if(this->man.first == rhs.man.first && this->man.second == rhs.man.second )
        {
            ret_val = true;

            for (diamond_t diamond: this->diamonds)
            {

                    if (std::find(rhs.diamonds.begin(), rhs.diamonds.end(),diamond) != rhs.diamonds.end())
                    {
                        ret_val= true;
                    }
                    else
                    {
                        ret_val= false;
                        break;
                    }
            }

            return ret_val;
        }
        else
        {
            return false;
        }

    }

    bool operator()(const state_s &obj)
    {
        int ret_val = 0;
        if(this->man == obj.man)
        {
            for(int i = 0; i < this->diamonds.size(); i++)
            {
                for (diamond_t obj_diamond : obj.diamonds)
                {
                    if(this->diamonds.at(i) == obj_diamond){ ret_val++; }
                }
            }
            if(ret_val == this->diamonds.size())
            {
                return this->heuristic < obj.heuristic;
            }
        }
        return false;
    }
};

class sokoban_map
{
public:
    sokoban_map() = default;
    sokoban_map(std::string);
    state_s get_init_state();
    state_s get_final_state();
    void deadlock_detection();
    void print();
    void print(state_s state );
    void print_final(state_s final);
    int get_row(){ return row; }
    int get_col(){ return col; }
    void delete_map(char** state_map);
    char** get_map(){ return map; }
    char** get_map(state_s state);
    state_s get_child( state_s current, diamond_t diamond, position_t push_direction);

private:
    unsigned long int count;
    int col, row, diamonds;
    void clear_map();
    char** map;

};
#endif
