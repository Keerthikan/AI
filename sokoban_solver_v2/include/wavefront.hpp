#ifndef WAVEFRONT_HPP
#define WAVEFRONT_HPP

#include "sokoban_map.hpp"

class wavefront
{
public:
    wavefront() = default;
    wavefront(sokoban_map *map);
    int** get_wavefront(){ return wavefront_map; }
    int** get_wavefront(state_s state, char initiator);
    int** get_wavefront(state_s state, char initiator, char obstacle);
    int** get_wavefront(state_s state, char initiator, char obstacle, char obstacle_);
private:
    void impose_state(state_s state);
    void clear_state();
    void print_wavefront(int** wf);
    int** wavefront_map;
    int** init_array();
    sokoban_map *map;
};

#endif
