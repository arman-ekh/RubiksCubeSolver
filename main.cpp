#include <cstdint>
#include <iostream>

const uint64_t EdgesSolved = 0b01011'01010'01000'00110'00101'00100'00011'00010'00001'00000;
const uint64_t CornersSolved = 0b00111'00110'00101'00100'00011'00010'00001'00000;

struct Cube {

    uint64_t Edges ;
    uint64_t Corners ;

    bool isSolved() const {
        return Edges == EdgesSolved && Corners == CornersSolved;
    }

    Cube(uint64_t Edges, uint64_t Corners) {
        this->Edges = Edges;
        this->Corners = Corners;
    }

};

int main() {


    std::cout << sizeof(Cube) << std::endl;

    return 0;
}