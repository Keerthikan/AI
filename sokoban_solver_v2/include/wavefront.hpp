#ifndef WAVEFRONT_HPP
#define WAVEFRONT_HPP

#include "sokoban_map.hpp"

 typedef std::vector<int> row_t;
 typedef std::vector<row_t> matrix_t;


class wavefront
{
public:
    wavefront() = default;
    wavefront(sokoban_map *map);
    matrix_t get_wavefront(){ return wavefront_map; }
    matrix_t get_wavefront(state_s state, char initiator);
    matrix_t get_wavefront(state_s state, char initiator, char obstacle);
    matrix_t get_wavefront(state_s state, char initiator, char obstacle, char obstacle_);

private:
    void impose_state(state_s state);
    void clear_state();
    void print_wavefront(matrix_t wf);
    matrix_t wavefront_map;
    void init_array(matrix_t matrix);
    sokoban_map *map;
};

#endif
