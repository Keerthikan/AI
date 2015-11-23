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

#define UP      0
#define DOWN    1
#define LEFT    2
#define RIGHT   3

typedef std::pair<int,Node*> heuristic_t;

class a_star
{
public:
    a_star(std::vector<Node> graph, wavefront *wavefront_ptr);
    std::string solve();
private:
    std::vector<Node> graph;
    std::vector< std::pair<int, int> > goal;
    wavefront *wavefront_ptr;

    bool check_goal();
    bool validate_push_direction(Edge *edge);
    static bool find_position(Node node);
    static bool find_man(Node node);
    vector<Node*> find_diamonds();
    heuristic_t get_heuristic(vector<Node*> diamonds, char initiator);
    int get_move_cost(Node* node);
    Node* get_push_direction(Node* node);
};
#endif
