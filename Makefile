all:
	mpic++ -o maze.out src/maze.cpp src/generator/mazegenerator.cpp src/generator/bfs.cpp src/generator/kruskal.cpp src/solver/mazesolver.cpp src/solver/dfs.cpp src/solver/dijkstra.cpp -std=c++11
