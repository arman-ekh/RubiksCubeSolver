#include <cstdint>
#include <iostream>

struct Cube {
    const uint64_t EdgesSolved = 0b01011'01010'01000'00110'00101'00100'00011'00010'00001'00000;
    const uint64_t CornersSolved = 0b00111'00110'00101'00100'00011'00010'00001'00000;
    uint64_t Edges ;
    uint64_t Corners ;

    bool isSolved() const {
        return Edges == EdgesSolved && Corners == CornersSolved;
    }

};

int main() {


    return 0;
}