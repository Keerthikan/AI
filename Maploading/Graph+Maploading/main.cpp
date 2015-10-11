//
//  main.cpp
//  mapLoading
//
//  Created by Keerthikan Ratnarajah on 07/10/15.
//  Copyright (c) 2015 Keerthikan Ratnarajah. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "edge.h"
#include "vertex.h"
#include "sokoban.h"
#include "graph.h"
#include "boost/graph/adjacency_list.hpp"
#include <boost/graph/graph_traits.hpp>
#include <boost/config.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;

bool isitTrue(char temp)
{
    if (temp == '.' || temp == 'J' || temp == 'G' || temp == 'M')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool distance(std::pair<int, int> lhs, std::pair<int, int> rhs)
{
    if (abs(std::hypot(lhs.first - rhs.first, lhs.second - rhs.second)) <= 1 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

int main(int argc, const char * argv[]) {
    Sokoban map("/Users/keerthikanratnarajah/Desktop/mappe\ uden\ navn/mapLoading/mapLoading/map.txt");
    
    using graph_t = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
    Vertex>;
    using vertex_descriptor = boost::graph_traits < graph_t >::vertex_descriptor;
    
    
    vector<pair<int, int>> edges;
    vector<Vertex> vertices;
    int col = map.getCol();
    int row = map.getRow();
    for(int y = 0; y < col; y++){
        for (int x = 0; x < row; x++) {
            char temp = map.getChar(x, y);
            if(isitTrue(temp)){
                vertices.push_back(Vertex(x,y,temp));
            }
        }
    }
    
    for(int y = 0; y < vertices.size(); ++y){
        for(int x = y + 1; x < vertices.size(); ++x){
            if (distance(vertices[y].getCoord(), vertices[x].getCoord())) {
                edges.emplace_back(y,x);
            }
        }
    }
    cout << vertices.size() << endl;
    cout << edges.size() << endl;
    
    graph_t graph;
    for (auto i: vertices)
    {
        boost::add_vertex(i,graph);
    }
    cout << "Verticies inserted" << endl;
    for (auto i: edges)
    {
        boost::add_edge(i.first,i.second,graph);
    }
    cout << "Edges inserted" << endl;
    
    auto i = boost::vertices(graph);
    
    
    for(auto it = i.first; it != i.second; ++it) {
        std::cout << "vertice " << *it << " {" << std::setw(3) << graph[*it].getCoord().first <<
        ';' << std::setw(3) << graph[*it].getCoord().second << "} \"" <<
        graph[*it].getElement() << "\" adjacent to: ";
        if( graph[*it].getElement() == 'M') {
            cout << "SAY HEYYYYYYYYYYYYYY " << endl;
        }
        auto adj_vert = boost::adjacent_vertices(*it, graph);  // Get adjacent vertex fot *it
        for(auto v = adj_vert.first; v != adj_vert.second; ++v)
            std::cout << *v << ' ' << "coord:(" << graph[*v].getCoord().first << "," << graph[*v].getCoord().second << ")";
        std::cout << '\n';
    }
    auto ad = boost::adjacent_vertices(29, graph);  // Get adjacent vertex fot *it

    
    vertex_descriptor s = vertex('M', graph);
    cout << *boost::adjacent_vertices(3, graph).first << *boost::adjacent_vertices(3, graph).second  << endl;
    std::vector<vertex_descriptor> p(num_vertices(graph));
    std::vector<int> d(num_vertices(graph));
    
    for (auto i = ad.first; i < ad.second; i++) {
        cout << graph[*i].getCoord().first << "," << graph[*i].getCoord().second << endl;
    }

    //boost::dijkstra_shortest_paths(graph, s,
    //                        predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, graph))).
    //                        distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, graph))));

    std::cout << "Hello, World!\n";
    return 0;
}
