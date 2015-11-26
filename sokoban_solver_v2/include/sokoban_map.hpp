#ifndef SOKOBAN_MAP_HPP
#define SOKOBAN_MAP_HPP
#include <string>
#include <vector>

typedef std::pair<int,int> position_t;
typedef std::pair<int,int> diamond_t;

struct state_s
{
    position_t man;
    std::vector<diamond_t> diamonds;
    int heuristic;

    bool operator!=(const state_s &rhs )
    {
        bool result = false;
        for(int i = 0; i < this->diamonds.size(); i++)
        {
            if(this->diamonds.at(i) != rhs.diamonds.at(i))
            {
                result = true;
                break;
            }
        }
        return result;
    }
};

class sokoban_map
{
public:
    sokoban_map() = default;
    sokoban_map(std::string);
    state_s get_init_state();
    state_s get_final_state();
    void print();
    void print(state_s state );
    int get_row(){ return row; }
    int get_col(){ return col; }
    char** get_map(){ return map; }
    state_s get_child( state_s current, diamond_t diamond, position_t push_direction);

private:
    int col, row, diamonds;
    void clear_map();
    char** map;

};
#endif
