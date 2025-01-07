#include "mazesolver.hpp"
#include "dfs.hpp"
#include "dijkstra.hpp"

void MazeSolver::solveDFSMaze(std::vector<std::vector<char>>& maze, std::vector<std::vector<char>>& visited, int rootX, int rootY) {
    return DFSMazeSolver::solveMaze(maze, visited, rootX, rootY);
}

void MazeSolver::solveDijkstraMaze(std::vector<std::vector<char>>& maze, std::vector<std::vector<char>>& visited, int rootX, int rootY) {
    return DijkstraMazeSolver::solveMaze(maze, visited, rootX, rootY);
}