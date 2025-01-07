#ifndef MAZEGENERATOR_HPP
#define MAZEGENERATOR_HPP

#include <vector>
#include <string>

class MazeGenerator {
public:
    static void generateBFSMaze( std::vector<std::vector<char>>& maze, int startX, int startY, int endX, int endY);
    static void generateKruskalMaze( std::vector<std::vector<char>>& maze, int startX, int startY, int endX, int endY);
};

#endif  
