#include <iostream>
#include "sokoban_map.hpp"
#include "wavefront.hpp"
#include "a_star.hpp"

using namespace std;

int main()
{
    sokoban_map map("/home/thomas/Documents/Git/AI/sokoban_solver_v2/maps/heuristictest.txt");
    map.deadlock_detection();

    state_s init = map.get_init_state();
    state_s final = map.get_final_state();

    wavefront wf(&map);

    wf.get_wavefront(init, 'G');

    a_star solver(init,final,wf,map);
    cout << solver.solve()<< endl;
    return 0;
}
