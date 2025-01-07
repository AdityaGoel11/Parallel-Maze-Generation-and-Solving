#include "mazegenerator.hpp"
#include "bfs.hpp"
#include "kruskal.hpp"

void MazeGenerator::generateBFSMaze( std::vector<std::vector<char>>& maze, int startX, int startY, int endX, int endY) {
    BFSMazeGenerator::generateMaze(maze, startX, startY, endX, endY);
}

void MazeGenerator::generateKruskalMaze( std::vector<std::vector<char>>& maze, int startX, int startY, int endX, int endY) {
    KruskalMazeGenerator::generateMaze(maze, startX, startY, endX, endY);
}
