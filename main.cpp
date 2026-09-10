#include <iostream>
#include <chrono>


#include "include/Cube.h"
#include "src/DFS.h"

int main() {


    Cube cube(EdgesSolved, CornersSolved);

    cube.F();
    cube.B();
    cube.R();
    cube.U();
    cube.D();
    cube.L();
    // cube.F();
    DFS dfs = DFS();

    auto start_time = std::chrono::high_resolution_clock::now();

    dfs.Search(cube , 8 ,NONE);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    std::cout << dfs.getNodeCount() << std::endl;
    std::cout << "Time taken: " << duration.count() << " seconds." << std::endl;

    return 0;
}
