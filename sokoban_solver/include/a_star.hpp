#ifndef A_STAR_HPP
#define A_STAR_HPP
#include "sokoban.h"
#include "wavefront.hpp"

#include <utility>
#include <string>
#include <vector>

#define DIAMOND 'J'
#define MAN     'M'
#define GOAL    'G'
#define DIAMOND_ON_GOAL 'j'

#define UP      0
#define DOWN    1
#define LEFT    2
#define RIGHT   3

typedef std::pair<int,Node*> heuristic_t;
typedef bool (*comp)(Sokoban*,Sokoban*);


struct find_neighbour {
    const std::pair<int,int> position;
    find_neighbour(const std::pair<int,int> position) : position(position) {}

    bool operator()(const Node& j) const
    {
        return j.position == position;
    }
};



class a_star
{
public:
    a_star(Sokoban sokoban, Sokoban sokoban_final, wavefront *wavefront_ptr);
    std::string solve();
private:
    Sokoban sokoban, sokoban_final;
    graph_t graph;
    std::vector< std::pair<int, int> > goal;
    wavefront *wavefront_ptr;
    vector<Node*> push_directions;

    static bool compare_heuristic(Sokoban *A, Sokoban *B);
    bool check_goal();
    bool validate_push_direction(Edge *edge);
    static bool find_position(Node node);
    static bool find_man(Node node);
    vector<Node*> find_diamonds();
    int get_heuristic(vector<Node*> diamonds);
    int get_move_cost(Node* node);
    Node* get_push_direction(Node* node);
};
#endif
