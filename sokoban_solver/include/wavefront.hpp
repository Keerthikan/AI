#ifndef WAVEFRONT_HPP
#define WAVEFRONT_HPP
#include "sokoban.h"

#include <vector>
#include <queue>

class wavefront
{
public:
    wavefront() = default;
    wavefront(const int &row, const int &col, const int &diamonds, char **board);
    wavefront(int row, int col, std::vector<Node> graph);
    int **get_wavefront(char initiator);
    int **get_wavefront();
private:
    int row, col, dia;
    std::vector<Node> graph;
    std::queue<Node> wave_queue;
    int** wavefront_map;
    typedef std::pair<int, int> diamond;
    std::vector<diamond> diamonds;

    int **get_map_array();
};
#endif
