#include <random>
#include <algorithm>
#include <queue>
#include <iostream>
#include <iostream>
#include <chrono>
#include <stack>
#include "bfs.hpp"
using namespace std;

const int rows = 64;
const int cols = 64;

// Function to print the maze
void printMaze(const vector<vector<char>> &maze)
{
    for (int i = 0; i < maze.size(); i++)
    {
        for (int j = 0; j < maze[0].size(); j++)
        {
            cout << maze[i][j] << " ";
        }
        cout << endl;
    }
}
bool IS_VALI_D(int x, int y, int row1, int row2, int cols)
{
    return x >= row1 && x <= row2 && y >= 0 && y < cols;
}
void printMaze(const std::vector<std::string> &maze)
{
    for (const std::string &row : maze)
    {
        std::cout << row << std::endl;
    }
}

bool hasOnePathNeighbour(vector<vector<char>> &maze, int x, int y, int row1, int row2, int cols)
{
    int count = 0;
    if (IS_VALI_D(x + 1, y, row1, row2, cols) && (maze[x + 1][y] == ' ' || maze[x + 1][y] == 'Q'))
        count++;
    if (IS_VALI_D(x - 1, y, row1, row2, cols) && (maze[x - 1][y] == ' ' || maze[x - 1][y] == 'Q'))
        count++;
    if (IS_VALI_D(x, y + 1, row1, row2, cols) && (maze[x][y + 1] == ' ' || maze[x][y + 1] == 'Q'))
        count++;
    if (IS_VALI_D(x, y - 1, row1, row2, cols) && (maze[x][y - 1] == ' ' || maze[x][y - 1] == 'Q'))
        count++;

    return count == 1 || count == 0;
}

bool generateMazeHelper(vector<vector<char>> &maze, int x, int y, int finalX, int finalY, int row1, int row2, int cols, vector<int> &directions)
{
    if (IS_VALI_D(x, y, row1, row2, cols) && hasOnePathNeighbour(maze, x, y, row1, row2, cols))
        maze[x][y] = ' ';
    else
        maze[x][y] = '*';

    if (x == finalX && y == finalY)
        return true;

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(directions.begin(), directions.end(), g);

    for (int i = 0; i < 2; ++i)
    {
        int nextX = x, nextY = y;
        switch (directions[i])
        {
        case 0:
            nextY++;
            break;
        case 1:
            nextY--;
            break;
        case 2:
            nextX--;
            break;
        case 3:
            nextX++;
            break;
        }

        if (IS_VALI_D(nextX, nextY, row1, row2, cols) && maze[nextX][nextY] == '*' && hasOnePathNeighbour(maze, nextX, nextY, row1, row2, cols))
        {
            if (generateMazeHelper(maze, nextX, nextY, finalX, finalY, row1, row2, cols, directions))
                return true;
        }
    }

    maze[x][y] = '*';
    return false;
}

void extendPaths(vector<vector<char>> &maze, int startX, int startY, int row1, int row2, int cols)
{
    // Array to represent the four possible directions: up, down, left, right
    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};
    int directions[4] = {0, 1, 2, 3};
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(directions, directions + 4, g);

    // Create a queue to store the next cells to be extended
    std::queue<std::pair<int, int>> pathQueue;

    // Add the starting cell to the queue

    for (int i = row1; i <= row2; i++)
    {
        for (int j = 0; j < cols; j++)
        {

            if (maze[i][j] == ' ')
            {
                // Mark the adjacent cell as space and add it to the queue
                pathQueue.push({i, j});
            }
        }
    }

    // While the queue is not empty, extend the path
    while (!pathQueue.empty())
    {
        // Get the current cell from the front of the queue
        int x = pathQueue.front().first;
        int y = pathQueue.front().second;
        pathQueue.pop();
        // maze[x][y]=' ';

        // Mark the current cell as part of the extended path

        for (int i = 0; i < 4; ++i)
        {
            int nextX = x, nextY = y;
            switch (directions[i])
            {
            case 0:
                nextY++;
                break;
            case 1:
                nextY--;
                break;
            case 2:
                nextX--;
                break;
            case 3:
                nextX++;
                break;
            }

            if (IS_VALI_D(nextX, nextY, row1, row2, cols) && maze[nextX][nextY] == '*' && hasOnePathNeighbour(maze, nextX, nextY, row1, row2, cols))
            {
                pathQueue.push({nextX, nextY});
                maze[nextX][nextY] = ' ';
            }

        }
    }
}

bool acycle(vector<vector<char>> &maze, int row1, int row2, int cols, int startx, int starty)
{

    vector<int> isvisited((row2 - row1 + 1) * cols, 0);
    stack<pair<int, int>> s;
    s.push({startx * cols + starty, -1});
    isvisited[startx * cols + starty] = 1;
    int directions[4] = {0, 1, 2, 3};

    while (!s.empty())
    {

        pair<int, int> pr = s.top();
        s.pop();
        int tp = pr.first;
        int parent = pr.second;
        int x = tp / cols, y = tp % cols;

        // cout << x << ", " << y << "\n";

        // isvisited[nextX * cols + nextY] = 1;

        for (int i = 0; i < 4; ++i)
        {
            int nextX = x, nextY = y;
            switch (directions[i])
            {
            case 0:
                nextY++;
                break;
            case 1:
                nextY--;
                break;
            case 2:
                nextX--;
                break;
            case 3:
                nextX++;
                break;
            }
            if (nextX == parent / cols && nextY == parent % cols)
                continue;

            // cout << nextX << ", " << nextY << "\n";
            if (IS_VALI_D(nextX, nextY, row1, row2, cols) && maze[nextX][nextY] == ' ')
            {
                if (isvisited[nextX * cols + nextY] == 1)
                {
                    maze[nextX][nextY] = 'Q';
                    maze[x][y] = 'R';
                    maze[parent / cols][parent % cols] = 'O';
                    cout << nextX << " " << nextY << "\n";
                    return false;
                }
                s.push({nextX * cols + nextY, tp});
                isvisited[nextX * cols + nextY] = 1;
            }
        }
    }
    return true;
}

void BFSMazeGenerator::generateMaze(vector<vector<char>>& maze, int startX, int startY, int endX, int endY)
{
    int row1 = startX;
    int row2 = endX;

    vector<int> directions = {3, 0};

    if (startY < endY)
        directions[1] = 0;
   
   if(startY > endY)
        directions[1] = 1;
    

   
    generateMazeHelper(maze, startX, startY, endX, endY, row1, row2, cols, directions);
  
    extendPaths(maze, startX, startY, row1, row2, cols);

   
    
}
