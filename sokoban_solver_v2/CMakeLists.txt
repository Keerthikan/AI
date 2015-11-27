cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(sokoban_solver)

add_definitions(-std=c++11)

add_executable(${PROJECT_NAME}  src/main.cpp
                                src/sokoban_map.cpp include/sokoban_map.hpp
                                src/wavefront.cpp   include/wavefront.hpp
                                src/a_star.cpp      include/a_star.hpp)

include_directories(include/)

#target_link_libraries(${PROJECT_NAME} ${BOOST_SYSTEM_LIBRARY})

set(CMAKE_BUILD_TYPE Release)
