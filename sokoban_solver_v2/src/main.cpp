#include <iostream>
#include "sokoban_map.hpp"
#include "wavefront.hpp"
#include "a_star.hpp"
using namespace std;

int main()
{
    sokoban_map map("../maps/map.txt");
    map.print();

    wavefront wf(&map);
    state_s init = map.get_init_state();
    state_s final = map.get_final_state();
    wf.get_wavefront(init, 'G');

    a_star solver(init,final,wf,map);
    solver.solve();
    return 0;
}