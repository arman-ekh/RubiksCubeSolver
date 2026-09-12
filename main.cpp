#include <iostream>
#include <chrono>


#include "include/Cube.h"
#include "src/DFS.h"

int main() {


    Cube cube(EdgesSolved, CornersSolved);

    cube.F3();
    cube.B();
    cube.R();
    cube.U();
    cube.D();
    cube.L();
    cube.F();
    cube.L();
    cube.U2();
    cube.R();
    cube.U();
    cube.B3();

    std::atomic<long int> total_nodes{0};
    int target_depth = 8;

    auto start_time = std::chrono::high_resolution_clock::now();

    bool solved = DFS::ParallelSearch(cube, target_depth, total_nodes);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    std::cout << "Is Solved: " << (solved ? "Yes" : "No") << std::endl;
    std::cout << "Total Nodes Evaluated: " << total_nodes.load() << std::endl;
    std::cout << "Time Elapsed: " << duration.count() << " seconds" << std::endl;

    return 0;
}
