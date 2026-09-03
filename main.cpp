#include <bitset>
#include <cstdint>
#include <iostream>

const uint64_t EdgesSolved = 0b01011'01010'01001'01000'00111'00110'00101'00100'00011'00010'00001'00000;
const uint64_t CornersSolved = 0b00111'00110'00101'00100'00011'00010'00001'00000;

uint64_t get_index(uint64_t num, int index) {
    return (num >> (index * 5)) & 0b11111;
}

void set_index(uint64_t* num, int index, uint64_t value) {
    uint64_t mask = 0b11111ULL << (index * 5);

    *num &= ~mask;
    *num |= value << (index * 5);
}

void move_index(int index_one, int index_two, uint64_t* num) {
    uint64_t value_one = get_index(*num, index_one);
    uint64_t value_two = get_index(*num, index_two);

    set_index(num, index_one, value_two);
    set_index(num, index_two, value_one);
}



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

    void U() {
        move_index(3,0 , &Edges);
        move_index(3,2 , &Edges);
        move_index(2,1 , &Edges);


        //move corners
    }

};



int main() {


    // std::cout << sizeof(Cube) << std::endl;
    //
    // std::cout <<std::bitset<60>(EdgesSolved) << std::endl;
    //
    //
    //
    //
    //

    Cube cube = Cube(EdgesSolved, CornersSolved);
    cube.U();
    std::cout << std::bitset<20>(cube.Edges) << std::endl;
    cube.U();
    std::cout << std::bitset<20>(cube.Edges) << std::endl;
    cube.U();
    std::cout << std::bitset<20>(cube.Edges) << std::endl;
    cube.U();
    std::cout << std::bitset<20>(cube.Edges) << std::endl;
    cube.U();
    std::cout << std::bitset<20>(cube.Edges) << std::endl;

    return 0;
}