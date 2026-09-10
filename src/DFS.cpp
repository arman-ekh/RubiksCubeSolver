#include "DFS.h"

DFS::DFS() {
    nodeCount = 0;
    isSolved = false;
}

inline int get_face(uint8_t move) {
    if (move == NONE) return -1;
    return (move - 1) / 3;
}

inline uint8_t inverse_move(uint8_t move) {
    if (move == NONE) return NONE;

    if (move % 3 == 1)
        return move + 2;

    if (move % 3 == 2)
        return move;

    return move - 2;
}

inline bool should_prune(uint8_t last_move, uint8_t next_move) {
    if (last_move == NONE) return false;

    int last_face = get_face(last_move);
    int next_face = get_face(next_move);

    if (last_face == next_face) return true;


    if ((last_face == 0 && next_face == 1) ||
        (last_face == 2 && next_face == 3) ||
        (last_face == 4 && next_face == 5)) {
        return true;
        }

    return false;
}

inline void apply_move(Cube& cube, uint8_t move) {
    switch (move) {
        case U1: cube.U(); break;
        case U2: cube.U2(); break;
        case U3: cube.U3(); break;

        case D1: cube.D(); break;
        case D2: cube.D2(); break;
        case D3: cube.D3(); break;

        case R1: cube.R(); break;
        case R2: cube.R2(); break;
        case R3: cube.R3(); break;

        case L1: cube.L(); break;
        case L2: cube.L2(); break;
        case L3: cube.L3(); break;

        case F1: cube.F(); break;
        case F2: cube.F2(); break;
        case F3: cube.F3();  break;

        case B1: cube.B(); break;
        case B2: cube.B2(); break;
        case B3: cube.B3(); break;
        default: break;
    }
}



void DFS::Search(Cube& cube, int depth, uint8_t last_move) {
    ++nodeCount;

    if (cube.isSolved()) {
        isSolved = true;
        // return;
    }

    if (depth == 0)
        return;

    for (uint8_t move = U1; move <= B3; ++move) {
        if (should_prune(last_move, move))
            continue;

        apply_move(cube, move);

        Search(cube, depth - 1, move);

        apply_move(cube, inverse_move(move));

        // if (isSolved)
        //     return;
    }
}


long int DFS::getNodeCount() {
    return nodeCount;
}
