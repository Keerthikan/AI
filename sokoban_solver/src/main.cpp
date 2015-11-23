#include "sokoban.h"
#include "wavefront.hpp"
#include "a_star.hpp"

#include <iostream>

int main() {

  Sokoban map("../maps/map.txt");
  map.createGraph();
  map.deadlockDetection();
  cout << map.board[1][3]  << endl;
  cout << "col: " << map.col << " row: " << map.row << endl;
  wavefront wavefront(map.row, map.col, map.graph);
  wavefront.get_wavefront('M');

  a_star a_solver(map.graph, &wavefront);
  a_solver.solve();

  return 0;
}
