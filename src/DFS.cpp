#include "DFS.h"

#include <algorithm>

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


    if (!should_prune(last_move , U1)) {
        cube.U();
        Search(cube, depth-1, U1);
        cube.U3();
    }
    if (!should_prune(last_move , U2)) {
        cube.U2();
        Search(cube, depth-1, U2);
        cube.U2();
    }
    if (!should_prune(last_move , U3)) {
        cube.U3();
        Search(cube, depth-1, U3);
        cube.U();
    }

    if (!should_prune(last_move , D1)) {
        cube.D();
        Search(cube, depth-1, D1);
        cube.D3();
    }
    if (!should_prune(last_move , D2)) {
        cube.D2();
        Search(cube, depth-1, D2);
        cube.D2();
    }
    if (!should_prune(last_move , D3)) {
        cube.D3();
        Search(cube, depth-1, D3);
        cube.D();
    }

    if (!should_prune(last_move , F1)) {
        cube.F();
        Search(cube, depth-1, F1);
        cube.F3();
    }
    if (!should_prune(last_move , F2)) {
        cube.F2();
        Search(cube, depth-1, F2);
        cube.F2();
    }
    if (!should_prune(last_move , F3)) {
        cube.F3();
        Search(cube, depth-1, F3);
        cube.F();
    }

    if (!should_prune(last_move , B1)) {
        cube.B();
        Search(cube, depth-1, B1);
        cube.B3();
    }
    if (!should_prune(last_move , B2)) {
        cube.B2();
        Search(cube, depth-1, B2);
        cube.B2();
    }
    if (!should_prune(last_move , B3)) {
        cube.B3();
        Search(cube, depth-1, B3);
        cube.B();
    }
    if (!should_prune(last_move , L1)) {
        cube.L();
        Search(cube, depth-1, L1);
        cube.L3();
    }
    if (!should_prune(last_move , L2)) {
        cube.L2();
        Search(cube, depth-1, L2);
        cube.L2();
    }
    if (!should_prune(last_move , L3)) {
        cube.L3();
        Search(cube, depth-1, L3);
        cube.L();
    }

    if (!should_prune(last_move , R1)) {
        cube.R();
        Search(cube, depth-1, R1);
        cube.R3();
    }
    if (!should_prune(last_move , R2)) {
        cube.R2();
        Search(cube, depth-1, R2);
        cube.R2();
    }
    if (!should_prune(last_move , R3)) {
        cube.R3();
        Search(cube, depth-1, R3);
        cube.R();
    }

    // for (uint8_t move = U1; move <= B3; ++move) {
    //     if (should_prune(last_move, move))
    //         continue;
    //
    //     apply_move(cube, move);
    //
    //     Search(cube, depth - 1, move);
    //
    //     apply_move(cube, inverse_move(move));
    //
    //     // if (isSolved)
    //     //     return;
    // }
}


long int DFS::getNodeCount() {
    return nodeCount;
}
