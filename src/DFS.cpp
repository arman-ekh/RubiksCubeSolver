#include "DFS.h"

#include <future>
#include <vector>

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

DFS::DFS() : nodeCount(0), isSolved(false) {
    initPruneTable();
    nodeCount = 0;
    isSolved = false;
}

long int DFS::getNodeCount() {
    return nodeCount;
}

void DFS::Search(Cube cube, int depth, uint8_t last_move, const std::atomic<bool>& global_solved) {
    ++nodeCount;


    if (global_solved.load(std::memory_order_relaxed)) {
        return;
    }

    if (depth == 0) return;

    #define TRY_MOVE(MOVE, DO_MOVE)                                      \
        if (!PRUNE_TABLE[last_move][MOVE]) {                             \
            Cube next_cube = cube;                                       \
            next_cube.DO_MOVE();                                         \
        if (next_cube.isSolved()) {                                  \
            isSolved = true;                                         \
            return;                                                  \
        }                                                            \
        Search(next_cube, depth - 1, MOVE, global_solved);           \
        if (isSolved || global_solved.load(std::memory_order_relaxed)) return; \
    }

    TRY_MOVE(1, U)   TRY_MOVE(2, U2)   TRY_MOVE(3, U3)
    TRY_MOVE(4, D)   TRY_MOVE(5, D2)   TRY_MOVE(6, D3)
    TRY_MOVE(7, F)   TRY_MOVE(8, F2)   TRY_MOVE(9, F3)
    TRY_MOVE(10, B)  TRY_MOVE(11, B2)  TRY_MOVE(12, B3)
    TRY_MOVE(13, L)  TRY_MOVE(14, L2)  TRY_MOVE(15, L3)
    TRY_MOVE(16, R)  TRY_MOVE(17, R2)  TRY_MOVE(18, R3)

    #undef TRY_MOVE
}

bool DFS::ParallelSearch(const Cube& start_cube, int depth, std::atomic<long int>& global_node_count) {
    initPruneTable();

    if (start_cube.isSolved()) {
        global_node_count.fetch_add(1, std::memory_order_relaxed);
        return true;
    }

    std::atomic<bool> global_solved{false};
    std::vector<std::future<long int>> futures;

    for (uint8_t move = 1; move <= 18; ++move) {
        futures.push_back(std::async(std::launch::async, [start_cube, depth, move, &global_solved]() -> long int {
            DFS local_dfs;
            Cube branch_cube = start_cube;

            switch (move) {
                case 1: branch_cube.U(); break;   case 2: branch_cube.U2(); break;  case 3: branch_cube.U3(); break;
                case 4: branch_cube.D(); break;   case 5: branch_cube.D2(); break;  case 6: branch_cube.D3(); break;
                case 7: branch_cube.F(); break;   case 8: branch_cube.F2(); break;  case 9: branch_cube.F3(); break;
                case 10: branch_cube.B(); break;  case 11: branch_cube.B2(); break; case 12: branch_cube.B3(); break;
                case 13: branch_cube.L(); break;  case 14: branch_cube.L2(); break; case 15: branch_cube.L3(); break;
                case 16: branch_cube.R(); break;  case 17: branch_cube.R2(); break; case 18: branch_cube.R3(); break;
            }


            local_dfs.Search(branch_cube, depth - 1, move, global_solved);

            if (local_dfs.getIsSolved()) {
                global_solved.store(true, std::memory_order_relaxed);
            }


            return local_dfs.getNodeCount();
        }));
    }


    for (auto& f : futures) {
        long int branch_nodes = f.get();

        global_node_count.fetch_add(branch_nodes, std::memory_order_relaxed);
    }

    global_node_count.fetch_add(1, std::memory_order_relaxed);

    return global_solved.load();
}