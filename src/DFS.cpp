#include "DFS.h"

bool DFS::PRUNE_TABLE[19][19];
bool DFS::isTableInitialized = false;

void DFS::initPruneTable() {
    if (isTableInitialized) return;

    for (uint8_t last = 0; last <= 18; ++last) {
        for (uint8_t next = 0; next <= 18; ++next) {
            PRUNE_TABLE[last][next] = calc_should_prune(last, next);
        }
    }
    isTableInitialized = true;
}

DFS::DFS() {
    nodeCount = 0;
    isSolved = false;
    initPruneTable();
}

void DFS::Search(Cube& cube, int depth, uint8_t last_move) {
    ++nodeCount;

    if (cube.isSolved()) {
        isSolved = true;
        return;
    }

    if (depth == 0) return;


    #define TRY_MOVE(MOVE, DO_MOVE, UNDO_MOVE)          \
        if (!PRUNE_TABLE[last_move][MOVE]) {             \
            DO_MOVE;                                     \
            Search(cube, depth - 1, MOVE);               \
            UNDO_MOVE;                                   \
            if (isSolved) return;                        \
        }


    TRY_MOVE(U1, cube.U(),  cube.U3())
    TRY_MOVE(U2, cube.U2(), cube.U2())
    TRY_MOVE(U3, cube.U3(), cube.U())


    TRY_MOVE(D1, cube.D(),  cube.D3())
    TRY_MOVE(D2, cube.D2(), cube.D2())
    TRY_MOVE(D3, cube.D3(), cube.D())


    TRY_MOVE(F1, cube.F(),  cube.F3())
    TRY_MOVE(F2, cube.F2(), cube.F2())
    TRY_MOVE(F3, cube.F3(), cube.F())

    TRY_MOVE(B1, cube.B(),  cube.B3())
    TRY_MOVE(B2, cube.B2(), cube.B2())
    TRY_MOVE(B3, cube.B3(), cube.B())

    TRY_MOVE(L1, cube.L(),  cube.L3())
    TRY_MOVE(L2, cube.L2(), cube.L2())
    TRY_MOVE(L3, cube.L3(), cube.L())

    TRY_MOVE(R1, cube.R(),  cube.R3())
    TRY_MOVE(R2, cube.R2(), cube.R2())
    TRY_MOVE(R3, cube.R3(), cube.R())

    #undef TRY_MOVE
}

long int DFS::getNodeCount() {
    return nodeCount;
}