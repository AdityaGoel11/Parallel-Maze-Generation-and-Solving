#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "kruskal.hpp"

using namespace std;

// Define a structure to represent an edge
struct Edge
{
    pair<int, int> cell1;
    pair<int, int> cell2;
};

bool IS__VALID(int x, int y, int row1, int row2, int cols)
{
    return x >= row1 && x <= row2 && y >= 0 && y < cols;
}

// Function to initialize the maze
vector<vector<char>> initializeMaze(int rows, int cols)
{
    vector<vector<char>> maze(rows, vector<char>(cols, '*'));

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (i % 2 == 0 && j % 2 == 1)
            {
                maze[i][j] = ' ';
            }
        }
    }

    
    maze[rows - 1][0] = ' ';

    return maze;
}

// Function to add edges to the graph
vector<Edge> addEdges(const vector<vector<char>> &maze, int rows, int cols)
{
    vector<Edge> edges;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (maze[i][j] == ' ')
            {
                // Check all four neighbors

                if (IS__VALID(i + 1, j, 0, rows - 1, cols) && maze[i + 1][j] == '*')
                {
                    edges.push_back({{i, j}, {i + 1, j}});
                }
                if (IS__VALID(i - 1, j, 0, rows - 1, cols) && maze[i - 1][j] == '*')
                {
                    edges.push_back({{i, j}, {i - 1, j}});
                }
                if (IS__VALID(i, j - 1, 0, rows - 1, cols) && maze[i][j - 1] == '*')
                {
                    edges.push_back({{i, j}, {i, j - 1}});
                }

                if (IS__VALID(i, j + 1, 0, rows - 1, cols) && maze[i][j + 1] == '*')
                {
                    edges.push_back({{i, j}, {i, j + 1}});
                }
            }
        }
    }

    return edges;
}

int findRoot(vector<int> &parent, int cell)
{
    while (cell != parent[cell])
    {
        cell = parent[cell];
    }
    return cell;
}

// Function to perform union operation
void performUnion(vector<int> &parent, int root1, int root2)
{
    
    if (root1 > root2)
        parent[root1] = root2;
    else
        parent[root2] = root1;
}

// Function to generate the maze using Kruskal's algorithm
vector<vector<char>> generateMazeH(vector<vector<char>> &maze, int rows, int cols)
{
    vector<Edge> edges = addEdges(maze, rows, cols);

    random_device rd;
    mt19937 g(rd());
    shuffle(edges.begin(), edges.end(), g);

    vector<int> parent(rows * cols);
    for (int i = 0; i < rows * cols; i++)
    {
        parent[i] = i;
    }

    performUnion(parent, findRoot(parent, (rows - 1) * cols), findRoot(parent, (rows - 1) * cols + 1));

    for (const auto &edge : edges)
    {
        int cell1 = edge.cell1.first * cols + edge.cell1.second;
        int cell2 = edge.cell2.first * cols + edge.cell2.second;

        // if(maze[edge.cell1.first][edge.cell2.second] = ' ' && maze[edge.cell2.first][edge.cell2.second] = ' ')

        int root1 = findRoot(parent, cell1);
        int root2 = findRoot(parent, cell2);

        int i = edge.cell2.first;
        int j = edge.cell2.second;
        bool isCycle = false;

        if (IS__VALID(i + 1, j, 0, rows - 1, cols))
        {
            int cell = (i + 1) * cols + j;

            if (cell1 != cell)
            {
                isCycle |= (findRoot(parent, cell) == root1);
            }
        }
        if (IS__VALID(i - 1, j, 0, rows - 1, cols))
        {
            int cell = (i - 1) * cols + j;
            if (cell1 != cell)
            {
                isCycle |= (findRoot(parent, cell) == root1);
            }
        }
        if (IS__VALID(i, j - 1, 0, rows - 1, cols))
        {
            int cell = (i)*cols + j - 1;
            if (cell1 != cell)
            {
                isCycle |= (findRoot(parent, cell) == root1);
            }
        }
        if (IS__VALID(i, j + 1, 0, rows - 1, cols))
        {
            int cell = (i)*cols + j + 1;
            if (cell1 != cell)
            {
                isCycle |= (findRoot(parent, cell) == root1);
            }
        }

        if (root1 != root2 && (!isCycle))
        {
            performUnion(parent, root1, root2);
            maze[edge.cell2.first][edge.cell2.second] = ' '; // Break the wall

            if (IS__VALID(i + 1, j, 0, rows - 1, cols))
            {
                int cell = (i + 1) * cols + j;
                if (maze[i + 1][j] == ' ')
                {
                    performUnion(parent, root1, findRoot(parent, cell));
                }
            }
            if (IS__VALID(i - 1, j, 0, rows - 1, cols))
            {
                int cell = (i - 1) * cols + j;
                if (maze[i - 1][j] == ' ')
                {
                    performUnion(parent, root1, findRoot(parent, cell));
                }
            }
            if (IS__VALID(i, j - 1, 0, rows - 1, cols))
            {
                int cell = (i)*cols + j - 1;
                if (maze[i][j - 1] == ' ')
                {
                    performUnion(parent, root1, findRoot(parent, cell));
                }
            }
            if (IS__VALID(i, j + 1, 0, rows - 1, cols))
            {
                int cell = (i)*cols + j + 1;
                if (maze[i][j + 1] == ' ')
                {
                    performUnion(parent, root1, findRoot(parent, cell));
                }
            }
        }
    }
return maze;
 
}
void KruskalMazeGenerator::generateMaze(vector<vector<char>>& maze, int startX, int startY, int endX, int endY)
{

    int row1 = startX;
    int row2 = endX;
    int cols = maze[0].size();
    int row = row2-row1+1;
    // vector<vector<char>>maze(maze.size(),vector<char>(maze[0].size(),'*'));
    vector<vector<char>> maze1 = initializeMaze(row, cols);
    maze1=generateMazeH(maze1, row, cols);

    for (int i = row1; i <= row2; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            maze[i][j] = maze1[i - row1][j];
        }
    }

    
}