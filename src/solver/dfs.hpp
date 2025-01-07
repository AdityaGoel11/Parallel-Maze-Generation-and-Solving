#ifndef DFS_HPP
#define DFS_HPP

#include <vector>
#include <string>

class DFSMazeSolver {
public:
    static void solveMaze(std::vector<std::vector<char>>& maze, std::vector<std::vector<char>>& visited, int rootX, int rootY);
};

#endif