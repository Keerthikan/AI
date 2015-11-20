#include "sokoban.cpp"
#include <iostream>

int main() {
  Sokoban map("/home/student/AI/sokoban_solverv1/map.txt");
  std::cout << "Hello, World!\n";
  map.createGraph();
  //cout << map.graph.begin()->Element << map.graph.begin()->neighbours.size() << endl;
  //map.generateAdjacencyMatrix();
  map.deadlockDetection();
  for(auto node: map.graph)
  {
      cout << "node position: " <<"("<< node.position.first <<","<< node.position.second <<")"<< " " << node.Element<< endl; 
      if (node.deadlock)
      {
            cout << node.position.first << node.position.second << endl;
      }
  }
  return 0;
}
