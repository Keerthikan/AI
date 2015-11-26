#include "sokoban.h"
#include "wavefront.hpp"
#include "a_star.hpp"

#include <iostream>

int main() {

  Sokoban map("../maps/map.txt");
  map.createGraph();
  

  return 0;
}
