#include "dijkstra.hpp"
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <functional> 
#include <random>     
#include <queue>
#include <limits> 
#include <utility> 




void DijkstraMazeSolver::solveMaze(std::vector<std::vector<char>>& maze, std::vector<std::vector<char>>& visited, int rootX, int rootY) {
    int rows = 64;
    int cols = 64;
   
    const int INF = std::numeric_limits<int>::max();

   
    std::vector<std::vector<int>> distance(rows, std::vector<int>(cols, INF));
    std::vector<std::vector<std::pair<int,int>>> predecessor(rows, std::vector<std::pair<int, int>>(cols, std::make_pair(-1, -1)));

    // Define possible moves (up, down, left, right)
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    // Create a priority queue for Dijkstra's algorithm
    std::priority_queue<std::pair<int, std::pair<int, int>>, std::vector<std::pair<int, std::pair<int, int>>>, std::greater<std::pair<int, std::pair<int, int>>>> pq;

    // Set distance of root cell to 0 and push it to the priority queue
    distance[rootX][rootY] = 0;
    predecessor[rootX][rootY] = {rootX,rootY};

    pq.push({0, {rootX, rootY}});

    // Dijkstra's algorithm
    while (!pq.empty()) {
        int x = pq.top().second.first;
        int y = pq.top().second.second;
        int dist = pq.top().first;
        pq.pop();

        // If current cell is the end point, break
        if (maze[x][y] == 'E') {
            break;
        }

        // Check all possible moves from current cell
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            // Check if the next cell is within bounds and is a valid path
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && maze[nx][ny] != '*' && dist + 1 < distance[nx][ny]) {
                distance[nx][ny] = dist + 1;
                predecessor[nx][ny] = {x,y};
                pq.push({distance[nx][ny], {nx, ny}});
                        if (maze[nx][ny] == 'E') {
            break;
        }
            }
        }
    }

    int x = 63; 
    int y = 0;

    while(!(predecessor[x][y].first == x && predecessor[x][y].second == y)){
        int curX = x;
        int curY = y;
        x = predecessor[curX][curY].first;
        y = predecessor[curX][curY].second;
        maze[x][y] = 'P';

    }
    return;
}