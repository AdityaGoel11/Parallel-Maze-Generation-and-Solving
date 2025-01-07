#include "dfs.hpp"
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <functional> 
#include <random>     



   static bool isVALID(std::vector<std::vector<char>>& maze, int x, int y) {
        // Check if the next cell is within bounds and is a valid path cell
        return x >= 0 && x < maze.size() && y >= 0 && y < maze[0].size() && (maze[x][y] == ' ' || maze[x][y] == 'E');
    }
    
void DFSMazeSolver::solveMaze(std::vector<std::vector<char>>& maze, std::vector<std::vector<char>>& visited, int rootX, int rootY) {
        int x = rootX;
        int y = rootY;
        // Mark the current cell as visited
        visited[x][y] = '1';

     
        // Define possible moves (up, down, left, right)
        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        // Loop through all possible moves
        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            // Check if the next cell is within bounds and is a valid path
            if (isVALID(maze, nx, ny) && visited[nx][ny] == '0') {
                // Recursively call DFS for the next cell
                solveMaze(maze, visited, nx, ny);

                // If the end point is reached, mark the current cell as part of the path
                if (maze[nx][ny] == 'E') {
                    maze[rootX][rootY] = 'P';
                    // cout << "reached end point" << endl;
                    return;
                }

                // If the current cell is part of the path, mark it as 'P'
                if (maze[nx][ny] == 'P') {
                    maze[rootX][rootY] = 'P';
                    return;
                }
            }
        }
}
