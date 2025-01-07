#include "generator/mazegenerator.hpp"
#include "solver/mazesolver.hpp"
#include <cstring>
#include <iostream>
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <queue>
#include <map>

struct Point
{
    int x;
    int y;
    // Default constructor
    Point() : x(0), y(0) {}
};

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int rows = 64;
    const int cols = 64;

    std::vector<std::vector<char>> mergedMaze(rows, std::vector<char>(cols, '*'));
    std::vector<std::vector<char>> maze(rows, std::vector<char>(cols, '*'));
    std::vector<std::vector<char>> visited(rows, std::vector<char>(cols, '0'));

    if (argc != 5)
    {
        if (rank == 0)
        {
            std::cerr << "Usage: " << argv[0] << " -g [bfs/kruskal] -s [dfs/dijkstra]" << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    std::string genAlgorithm = argv[2];
    std::string solveAlgorithm = argv[4];

    if ((genAlgorithm != "bfs" && genAlgorithm != "kruskal") || (solveAlgorithm != "dfs" && solveAlgorithm != "dijkstra"))
    {
        if (rank == 0)
        {
            std::cerr << "Invalid input. Please use -g [bfs/kruskal] -s [dfs/dijkstra]" << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    // Initialize random number generator
    std::mt19937 rng(rank * (unsigned int)time(0)); // Use rank and time to seed the RNG
    std::uniform_int_distribution<int> dist(0, rows - 1);

    int startX, startY, endX, endY;
    // Randomly generate start and end coordinates for each process
    if (rank == 0)
    {
        startX = 0;
        startY = 63;
        endX = 14;
        endY = dist(rng);
    }
    else if (rank == 1)
    {
        startX = 16;
        startY = dist(rng);
        endX = 30;
        endY = dist(rng);
    }
    else if (rank == 2)
    {
        startX = 32;
        startY = dist(rng);
        endX = 46;
        endY = dist(rng);
    }
    else if (rank == 3)
    {
        startX = 48;
        startY = dist(rng);
        endX = 63;
        endY = 0;
    }

    if (genAlgorithm == "bfs")
    {
        MazeGenerator::generateBFSMaze(maze, startX, startY, endX, endY);
    }
    else if (genAlgorithm == "kruskal")
    {
        MazeGenerator::generateKruskalMaze(maze, startX, startY, endX, endY);
    }

    if (rank == 0)
    {
        // mergedMaze.resize(rows);
        for (int i = 3; i >= 0; i--)
        {
            std::vector<char> buffer(rows * cols / size);
            if (i == 0)
            {
                for (int j = 0; j < 16; j++)
                {
                    for (int k = 0; k < 64; k++)
                    {
                        mergedMaze[j][k] = maze[j][k];
                    }
                }
               
            }
            else
            {
               
                MPI_Status status;
                MPI_Recv(&buffer[0], rows * cols / size, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                int sender_rank = status.MPI_SOURCE; // Get the rank of the sender
                for (int j = 0; j < 16; j++)
                {
                    for (int k = 0; k < 64; k++)
                    {
                        mergedMaze[sender_rank * 16 + j][k] = buffer[j * 64 + k];
                    }
                }
               
            }
        }
    }
    else if (rank == 1)
    {
        std::vector<char> flatmaze(rows * cols / size);
        for (int a = 16; a < 32; a++)
        {
            for (int b = 0; b < 64; b++)
            {
                flatmaze[64 * (a - 16) + b] = maze[a][b];
            }
        }
             MPI_Send(&flatmaze[0], rows * cols / size, MPI_CHAR, 0, rank, MPI_COMM_WORLD);
    }
    else if (rank == 2)
    {
        std::vector<char> flatmaze(rows * cols / size);
        for (int a = 32; a < 48; a++)
        {
            for (int b = 0; b < 64; b++)
            {
                flatmaze[64 * (a - 32) + b] = maze[a][b];
            }
        }
     
        MPI_Send(&flatmaze[0], rows * cols / size, MPI_CHAR, 0, rank, MPI_COMM_WORLD);
    }
    else if (rank == 3)
    {
        std::vector<char> flatmaze(rows * cols / size);
        for (int a = 48; a < 64; a++)
        {
            for (int b = 0; b < 64; b++)
            {
                flatmaze[64 * (a - 48) + b] = maze[a][b];
            }
        }
     
        MPI_Send(&flatmaze[0], rows * cols / size, MPI_CHAR, 0, rank, MPI_COMM_WORLD);
    }

    // Synchronize processes
    MPI_Barrier(MPI_COMM_WORLD);
   

    if (rank == 0)
    {
       
        int y14, y16, y30, y32, y46, y48;

        for (int i = 0; i < 64; i++)
        {
            if (mergedMaze[14][i] == ' ')
            {
                y14 = i;
               
                break;
            }
        }
        for (int i = 0; i < 64; i++)
        {
            if (mergedMaze[16][i] == ' ')
            {
                y16 = i;
              
                break;
            }
        }

        for (int i = 0; i < 64; i++)
        {
            if (mergedMaze[15][i] == ' ')
            {
                y16 = i;
              
                break;
            }
        
        }
        if (y16 >= y14)
        {
            for (int i = y16; i >= y14; i--)
            {
                mergedMaze[15][i] = ' ';
            
                if (mergedMaze[14][i] == ' ')
                {
                    break;
                }
            }
        }
        else
        {
            for (int i = y14; i >= y16; i--)
            {
                mergedMaze[15][i] = ' ';
                if (mergedMaze[16][i] == ' ')
                {
                    break;
                }
            }
        }

        for (int i = 0; i < 64; i++)
        {
            if (mergedMaze[30][i] == ' ')
            {
                y30 = i;
            
                break;
            }
        }
        for (int i = 0; i < 64; i++)
        {
            if (mergedMaze[32][i] == ' ')
            {
                y32 = i;
               
                break;
            }
        }

        if (y32 >= y30)
        {
            for (int i = y32; i >= y30; i--)
            {
                mergedMaze[31][i] = ' ';
                if (mergedMaze[30][i] == ' ')
                {
                    break;
                }
            }
        }
        else
        {
            for (int i = y30; i >= y32; i--)
            {
                mergedMaze[31][i] = ' ';
                if (mergedMaze[32][i] == ' ')
                {
                    break;
                }
            }
        }

        for (int i = 0; i < 64; i++)
        {
            if (mergedMaze[46][i] == ' ')
            {
                y46 = i;
                break;
            }
        }
        for (int i = 0; i < 64; i++)
        {
            if (mergedMaze[48][i] == ' ')
            {
                y48 = i;
                break;
            }
        }
        if (y48 >= y46)
        {
            for (int i = y48; i >= y46; i--)
            {
                mergedMaze[47][i] = ' ';
                if (mergedMaze[46][i] == ' ')
                {
                    break;
                }
            }
        }
        else
        {
            for (int i = y46; i >= y48; i--)
            {
                mergedMaze[47][i] = ' ';
                if (mergedMaze[48][i] == ' ')
                {
                    break;
                }
            }
        }
        mergedMaze[63][0] = 'E';
        std::vector<char> flatmaze(rows * cols);
        for (int a = 0; a < 64; a++)
        {
            for (int b = 0; b < 64; b++)
            {
                flatmaze[64 * a + b] = mergedMaze[a][b];
            }
        }
        MPI_Bcast(&flatmaze[0], rows * cols, MPI_CHAR, 0, MPI_COMM_WORLD);
    }
    else
    {
        std::vector<char> buffer(rows * cols);
        MPI_Status status;
        MPI_Bcast(&buffer[0], rows * cols, MPI_CHAR, 0, MPI_COMM_WORLD);
        for (int j = 0; j < 64; j++)
        {
            for (int k = 0; k < 64; k++)
            {
                mergedMaze[j][k] = buffer[j * 64 + k];
            }
        }
        
    }

    std::queue<std::pair<int, int>> q;
    using ValueType = Point;
    std::vector<std::vector<std::pair<int, int>>> predecessor(rows, std::vector<std::pair<int, int>>(cols, std::make_pair(-1, -1)));
    int sX = 0;
    int sY = 63;
    visited[0][63] = '1';
    q.push({sX, sY});
    predecessor[sX][sY] = {sX, sY};

    while (!q.empty())
    {
        int x = q.front().first;
        int y = q.front().second;
        q.pop();

        // Explore neighboring cells
        std::vector<std::pair<int, int>> neighbors = {{x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}};
        for (const auto &neighbor : neighbors)
        {
            int nx = neighbor.first;
            int ny = neighbor.second;
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && visited[nx][ny] == '0' && mergedMaze[nx][ny] != '*')
            {
                visited[nx][ny] = '1';
                q.push({nx, ny});
                predecessor[nx][ny] = {x, y};
            }
        }
        if (q.size() >= 4)
        {
            break;
        }
    }

    int rootX;
    int rootY;

    if (rank == 0)
    {
        rootX = q.front().first;
        rootY = q.front().second;
    }
    if (rank == 1)
    {
        q.pop();
        rootX = q.front().first;
        rootY = q.front().second;
    }
    if (rank == 2)
    {
        q.pop();
        q.pop();
        rootX = q.front().first;
        rootY = q.front().second;
    }
    if (rank == 3)
    {
        q.pop();
        q.pop();
        q.pop();
        rootX = q.front().first;
        rootY = q.front().second;
    }

    bool solved = false;
    mergedMaze[63][0] = 'E';

    
    if (solveAlgorithm == "dfs")
    {
        MazeSolver::solveDFSMaze(mergedMaze, visited, rootX, rootY);
        if (mergedMaze[63][1] == 'P' || mergedMaze[62][0] == 'P' || mergedMaze[rootX][rootY] == 'P')
            solved = true;

        if (rank == 0 && solved == false)
        {
            q.pop();
            q.pop();
            q.pop();
            q.pop();
            if (!(q.empty()))
            {

                rootX = q.front().first;
                rootY = q.front().second;

                MazeSolver::solveDFSMaze(mergedMaze, visited, rootX, rootY);
                if (mergedMaze[63][1] == 'P' || mergedMaze[62][0] == 'P' || mergedMaze[rootX][rootY] == 'P')
                    solved = true;

                while (!(q.empty()) && !(solved))
                {
                    q.pop();
                    rootX = q.front().first;
                    rootY = q.front().second;

                    MazeSolver::solveDFSMaze(mergedMaze, visited, rootX, rootY);
                    if (mergedMaze[63][1] == 'P' || mergedMaze[62][0] == 'P' || mergedMaze[rootX][rootY] == 'P')
                        solved = true;
                }
            }
        }

        // Access shared maze data
        if (solved == true)
        {

            int curX = rootX;
            int curY = rootY;

            mergedMaze[curX][curY] = 'P';
            while (!(predecessor[curX][curY].first == curX && predecessor[curX][curY].second == curY))
            {
                mergedMaze[curX][curY] = 'P';
                curX = predecessor[curX][curY].first;
                curY = predecessor[curX][curY].second;
                mergedMaze[curX][curY] = 'P';
            }
            mergedMaze[63][0] = 'E';
            mergedMaze[0][63] = 'S';
            for (const auto &row : mergedMaze)
            {
                for (char cell : row)
                {
                    std::cout << cell;
                }
                std::cout << std::endl;
            }
        }

    }
    else if (solveAlgorithm == "dijkstra")
    {
        MazeSolver::solveDijkstraMaze(mergedMaze, visited, 0, 63);
        if (rank == 0)
        {
            mergedMaze[63][0] = 'E';
            mergedMaze[0][63] = 'S';
            for (const auto &row : mergedMaze)
            {
                for (char cell : row)
                {
                    std::cout << cell;
                }
                std::cout << std::endl;
            }
        }
    }
   

    MPI_Finalize();
    return 0;
}
