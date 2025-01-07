#ifndef MAZESOLVER_HPP
#define MAZESOLVER_HPP

#include <vector>
#include <string>

class MazeSolver {
public:
    static void solveDFSMaze(std::vector<std::vector<char>>& maze, std::vector<std::vector<char>>& visited, int rootX, int rootY);
    static void solveDijkstraMaze(std::vector<std::vector<char>>& maze, std::vector<std::vector<char>>& visited, int rootX, int rootY);
};

#endif 