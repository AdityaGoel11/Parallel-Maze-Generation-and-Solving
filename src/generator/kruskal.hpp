#ifndef KRUSKAL_HPP
#define KRUSKAL_HPP

#include <vector>
#include <string>

class KruskalMazeGenerator{
public:
    static void generateMaze(std::vector<std::vector<char>> &maze, int startX, int startY, int endX, int endY);
};

#endif 
