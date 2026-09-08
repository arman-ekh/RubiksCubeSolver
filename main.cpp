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

uint64_t get_corner_state(uint64_t num, int index) {
    return (num >> (index * 5 + 3)) & 0b11;
}

void set_corner_state(uint64_t* num, int index, uint64_t state) {
    uint64_t mask = 0b11ULL << (index * 5 + 3);

    *num &= ~mask;
    *num |= state << (index * 5 + 3);
}

void change_corner_state_FB(int index, uint64_t* num) {
    uint64_t state = get_corner_state(*num, index);
    //case 0 -> 2
    //case 2 -> 0
    //case 1 -> 1
    if (state == 0)
        state = 2;
    else if (state == 2)
        state = 0;

    set_corner_state(num, index, state);
}

void change_corner_state_LR(int index, uint64_t* num) {
    // case 0 -> 1
    // case 1 -> 0
    // case 2 -> 2
    uint64_t state = get_corner_state(*num, index);
    if (state == 0) {
        state = 1;
    }else if (state == 1) {
        state = 0;
    }
    set_corner_state(num, index, state);
}

void change_corner_state_TB(int index, uint64_t* num) {
    // case 0 -> 0
    // case 1 -> 2
    // case 2 -> 1
    uint64_t state = get_corner_state(*num, index);
    if (state == 2) {
        state = 1;
    }else if (state == 1) {
        state = 2;
    }
    set_corner_state(num, index, state);
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
        change_corner_state_TB(0,&Corners);
        change_corner_state_TB(1,&Corners);
        change_corner_state_TB(2,&Corners);
        change_corner_state_TB(3,&Corners);
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
        change_corner_state_TB(4,&Corners);
        change_corner_state_TB(5,&Corners);
        change_corner_state_TB(6,&Corners);
        change_corner_state_TB(7,&Corners);

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
        change_corner_state_LR(2,&Corners);
        change_corner_state_LR(3,&Corners);
        change_corner_state_LR(6,&Corners);
        change_corner_state_LR(7,&Corners);
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
        change_corner_state_LR(2,&Corners);
        change_corner_state_LR(3,&Corners);
        change_corner_state_LR(6,&Corners);
        change_corner_state_LR(7,&Corners);
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

        //change orientation state
        change_corner_state_FB(0,&Corners);
        change_corner_state_FB(3,&Corners);
        change_corner_state_FB(4,&Corners);
        change_corner_state_FB(7,&Corners);
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

        //change orientation state
        change_corner_state_FB(1,&Corners);
        change_corner_state_FB(2,&Corners);
        change_corner_state_FB(4,&Corners);
        change_corner_state_FB(7,&Corners);
    }
};



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