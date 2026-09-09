#include <iostream>
#include <chrono>
#include <omp.h>

#include "include/BFS.h"
#include "include/Cube.h"

int main() {
    std::cout << "Running with " << omp_get_max_threads() << " OpenMP threads." << std::endl;


    Cube cube(EdgesSolved, CornersSolved);

    cube.F();
    cube.B();
    cube.R();
    cube.U();
    cube.D();
    cube.L();
    // cube.F();


    BFS bfs;
    bfs.initialize(cube);

    std::cout << "Starting Parallel BFS..." << std::endl;

    auto start_time = std::chrono::high_resolution_clock::now();
    int depth = 0;


    while (!bfs.is_finished()) {
        depth++;
        std::cout << "Processing Depth: " << depth << "..." << std::endl;

        bfs.step_parallel();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    if (bfs.is_found()) {
        std::cout << "\nCube Solved successfully at depth " << depth << "!" << std::endl;
    } else {
        std::cout << "\nFailed to find solution." << std::endl;
    }

    std::cout << "Time taken: " << duration.count() << " seconds." << std::endl;

    return 0;
}