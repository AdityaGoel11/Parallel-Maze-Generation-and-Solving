#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include <vector>
#include <string>

class DijkstraMazeSolver {
public:
    static void solveMaze(std::vector<std::vector<char>>& maze, std::vector<std::vector<char>>& visited, int rootX, int rootY);
};

#endif