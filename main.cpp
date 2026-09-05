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

void change_edge_state(int index , uint64_t* num) {
    uint64_t mask = 0b10000;
    mask = (mask << (index * 5));
    *num = *num ^ mask;
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
        //move Edges
        move_index(3,0 , &Edges);
        move_index(3,2 , &Edges);
        move_index(2,1 , &Edges);
        //U move does not affect state of Edges


        //move corners
        move_index(0,1 , &Corners);
        move_index(0,2 , &Corners);
        move_index(0,3 , &Corners);
        //change orientation state
        //TODO
    }

    void D() {
        //move Edges
        move_index(11,8,&Edges);
        move_index(11,9,&Edges);
        move_index(11,10,&Edges);
        //D move does not affect state of Edges

        //move corners
        move_index(4,5 , &Corners);
        move_index(4,6 , &Corners);
        move_index(4,7 , &Corners);
        //change orientation state
        //TODO
    }

    void R() {
        //move Edges
        move_index(6,3,&Edges);
        move_index(11,3,&Edges);
        move_index(7,3,&Edges);
        //R move does not affect state of Edges

        //move corners
        move_index(2,6,&Corners);
        move_index(2,7,&Corners);
        move_index(2 , 3,&Corners);
        //change orientation state
        //TODO
    }

    void L() {
        //move Edges
        move_index(1,4,&Edges);
        move_index(1,9,&Edges);
        move_index(1,5,&Edges);

        //move corners
        move_index(0,4,&Corners);
        move_index(0,5,&Corners);
        move_index(0,1,&Corners);
        //change orientation state
        //TODO
    }

    void F() {
        //move Edges
        move_index(0,7,&Edges);
        move_index(0,8,&Edges);
        move_index(0,4,&Edges);

        change_edge_state(0,&Edges);
        change_edge_state(7,&Edges);
        change_edge_state(8,&Edges);
        change_edge_state(4,&Edges);

        //move corners
        move_index(0,3,&Corners);
        move_index(0,7,&Corners);
        move_index(0,4,&Corners);
        //TODO
    }

    void B() {
        //move Edges
        move_index(2,6,&Edges);
        move_index(2,10,&Edges);
        move_index(2,5,&Edges);

        change_edge_state(2,&Edges);
        change_edge_state(6,&Edges);
        change_edge_state(5,&Edges);
        change_edge_state(10,&Edges);

        //move corners
        move_index(1,2,&Corners);
        move_index(1,7,&Corners);
        move_index(1,4,&Corners);
        //TODO
    }
};



int main() {


    std::cout << sizeof(Cube) << std::endl;
    //
    // std::cout <<std::bitset<60>(EdgesSolved) << std::endl;

    Cube cube = Cube(EdgesSolved, CornersSolved);
    cube.F();

    change_edge_state(11,&cube.Edges);

    std::cout << std::bitset<60>(cube.Edges) << std::endl;





    return 0;
}