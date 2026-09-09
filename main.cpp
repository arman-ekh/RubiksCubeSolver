#include <iostream>

#include "include/BFS.h"
#include "include/Cube.h"



int main() {
    std::cout << sizeof(Cube) << std::endl;


    Cube cube(EdgesSolved, CornersSolved);

    cube.F();
    cube.B();
    // cube.F();
    // cube.R();
    // cube.L();
    // cube.B();

    BFS bfs;
    bfs.initialize(cube);


        bfs.step();


    std::cout << bfs.is_found() << '\n';





    return 0;
}