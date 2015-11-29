#ifndef A_STAR_HPP
#define A_STAR_HPP

#include "sokoban_map.hpp"
#include "wavefront.hpp"

#include <string>
#include <set>

#define GOAL            'G'
#define MAN             'M'
#define MAN_ON_GOAL     'm'
#define DIAMOND         'J'
#define DIAMOND_ON_GOAL 'j'
#define DEADLOCK        'D'
#define MAN_ON_DEADLOCK 'd'

typedef bool (*comp)(state_s, state_s);

class a_star
{
public:
    a_star(state_s initial, state_s final, wavefront &wf, sokoban_map &sokoban);
    std::string solve();
private:
    static bool compare_heuristic(state_s A, state_s B);
    bool validate_push_direction(diamond_t diamond, position_t position, int** wavefront_obstacle);
    std::string stringify(state_s state);
    int get_heuristic(state_s &child);
    int get_move_cost(state_s &child, state_s &parent);
    position_t get_push_direction(diamond_t diamond, int** wavefront_obstacle);
    void get_children(state_s parent, std::vector<state_s> &children);
    void print_solution(state_s *final_state);

    state_s initial, final;
    wavefront wavefront_ptr;
    sokoban_map sokoban;
};

#endif
