#ifndef BFS_HPP
#define BFS_HPP

#include <vector>
#include <string>

class BFSMazeGenerator {
public:
    static void generateMaze( std::vector<std::vector<char>>& maze, int startX, int startY, int endX, int endY);
};

#endif  
