#include <bitset>
#include <cstdint>
#include <iostream>

#include "include/Cube.h"



int main() {
    std::cout << sizeof(Cube) << std::endl;

    Cube cube = Cube(EdgesSolved, CornersSolved);

    for (int i = 0; i < 8; i++) {
        std::cout << std::bitset<5>((cube.Corners >> (i * 5))) << std::endl;
    }


    std::cout << std::bitset<40>(cube.Corners) << std::endl;

    for (int i =0 ; i < 12 ; i++ ) {
        std::cout << std::bitset<5>((cube.Edges >> (i * 5))) << std::endl;
    }

    std::cout << std::bitset<60>(cube.Edges) << std::endl;



    return 0;
}