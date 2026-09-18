#include "../include/IDA.h"

#include <algorithm>
#include <future>
#include <vector>
#include <iostream>
#include <initializer_list>

bool IDA::PRUNE_TABLE[19][19];
bool IDA::isTableInitialized = false;


inline uint64_t build_mask(std::initializer_list<int> indices) {
    uint64_t mask = 0;
    for (int idx : indices) {
        mask |= (0x1FULL << (idx * 5));
    }
    return mask;
}

const SubGoal PHASES[4] = {

    { build_mask({8, 9, 10, 11}), build_mask({}) },


    { build_mask({8, 9, 10, 11}), build_mask({4, 5, 6, 7}) },


    { build_mask({4, 5, 6, 7, 8, 9, 10, 11}), build_mask({4, 5, 6, 7}) },


    { build_mask({0,1,2,3,4,5,6,7,8,9,10,11}), build_mask({0,1,2,3,4,5,6,7}) }
};

inline bool is_phase_solved(const Cube& cube, const SubGoal& phase) {
    if ((cube.Edges & phase.edge_mask) != (EdgesSolved & phase.edge_mask)) return false;
    if ((cube.Corners & phase.corner_mask) != (CornersSolved & phase.corner_mask)) return false;
    return true;
}


inline int evaluate_heuristic_masked(const Cube& cube, const SubGoal& phase) {
    constexpr uint64_t MASK = 0x1FULL;
    int incorrect_edges = 0;
    int incorrect_corners = 0;

#pragma GCC unroll 12
#pragma clang loop unroll(full)
    for (int i = 0; i < 12; ++i) {
        if ((phase.edge_mask & (MASK << (i * 5))) == 0) continue;
        uint64_t solved_edge = (EdgesSolved >> (i * 5)) & MASK;
        uint64_t current_edge = (cube.Edges >> (i * 5)) & MASK;
        incorrect_edges += (solved_edge != current_edge);
    }

#pragma GCC unroll 8
#pragma clang loop unroll(full)
    for (int i = 0; i < 8; ++i) {
        if ((phase.corner_mask & (MASK << (i * 5))) == 0) continue;
        uint64_t solved_corner = (CornersSolved >> (i * 5)) & MASK;
        uint64_t current_corner = (cube.Corners >> (i * 5)) & MASK;
        incorrect_corners += (solved_corner != current_corner);
    }

    int h_edges = (incorrect_edges + 3) >> 2;
    int h_corners = (incorrect_corners + 3) >> 2;

    return std::max(h_edges, h_corners);
}

IDAResult IDA::Search(Cube cube, int g, int threshold, uint8_t last_move, std::atomic<bool>& global_solved, const SubGoal& phase) {
    ++nodeCount;

    if (global_solved.load(std::memory_order_relaxed)) {
        return {false, threshold, {}};
    }

    if (is_phase_solved(cube, phase)) {
        global_solved.store(true, std::memory_order_relaxed);
        return {true, g, {}};
    }

    int h = evaluate_heuristic_masked(cube, phase);
    int f = g + h;

    if (f > threshold) {
        return {false, f, {}};
    }

    int min_next_threshold = 1000000;

#define TRY_MOVE(MOVE, DO_MOVE)                                             \
    do {                                                                    \
        if (!PRUNE_TABLE[last_move][MOVE]) {                                \
            Cube next_cube = cube;                                          \
            next_cube.DO_MOVE();                                            \
                                                                            \
            IDAResult res = Search(next_cube, g + 1, threshold, MOVE, global_solved, phase); \
                                                                            \
            if (res.solved) {                                               \
                res.path.push_back(MOVE); /* اضافه کردن حرکت به مسیر */      \
                return {true, res.next_threshold, res.path};                \
            }                                                               \
                                                                            \
            if (res.next_threshold < min_next_threshold) {                  \
                min_next_threshold = res.next_threshold;                    \
            }                                                               \
                                                                            \
            if (global_solved.load(std::memory_order_relaxed)) {            \
                return {false, min_next_threshold, {}};                     \
            }                                                               \
        }                                                                   \
    } while (0);

    TRY_MOVE(1, U)   TRY_MOVE(2, U2)   TRY_MOVE(3, U3)
    TRY_MOVE(4, D)   TRY_MOVE(5, D2)   TRY_MOVE(6, D3)
    TRY_MOVE(7, F)   TRY_MOVE(8, F2)   TRY_MOVE(9, F3)
    TRY_MOVE(10, B)  TRY_MOVE(11, B2)  TRY_MOVE(12, B3)
    TRY_MOVE(13, L)  TRY_MOVE(14, L2)  TRY_MOVE(15, L3)
    TRY_MOVE(16, R)  TRY_MOVE(17, R2)  TRY_MOVE(18, R3)

#undef TRY_MOVE

    return {false, min_next_threshold, {}};
}

void IDA::initPruneTable() {
    if (isTableInitialized) return;
    for (uint8_t last = 0; last <= 18; ++last) {
        for (uint8_t next = 0; next <= 18; ++next) {
            PRUNE_TABLE[last][next] = calc_should_prune(last, next);
        }
    }
    isTableInitialized = true;
}

IDA::IDA() {
    initPruneTable();
    nodeCount = 0;
    isSolved = false;
}

std::vector<uint8_t> IDA::ParallelSearch(const Cube &start_cube, std::atomic<unsigned long int> &global_node_count) {
    initPruneTable();

    Cube current_cube = start_cube;
    std::vector<uint8_t> full_solution;

    if (current_cube.isSolved()) {
        global_node_count.fetch_add(1, std::memory_order_relaxed);
        return full_solution;
    }

    std::vector<uint8_t> root_moves = {
        1, 2, 3, 4, 5, 6, 7, 8, 9,
        10, 11, 12, 13, 14, 15, 16, 17, 18
    };

    for (int p = 0; p < 4; ++p) {
        const SubGoal& current_phase = PHASES[p];

        if (is_phase_solved(current_cube, current_phase)) {
            continue;
        }

        int threshold = evaluate_heuristic_masked(current_cube, current_phase);
        std::atomic<bool> global_solved{false};
        std::vector<uint8_t> phase_solution;

        while (!global_solved.load(std::memory_order_relaxed)) {
            int next_global_threshold = 1000000;
            std::vector<std::future<IDAResult>> futures;
            futures.reserve(root_moves.size());

            for (uint8_t move : root_moves) {
                futures.push_back(std::async(std::launch::async, [=, &global_solved, &current_phase]() -> IDAResult {
                    if (global_solved.load(std::memory_order_relaxed)) {
                        return {false, 1000000, {}};
                    }

                    IDA local_ida;
                    Cube branch_cube = current_cube;

                    switch (move) {
                        case 1:  branch_cube.U();  break; case 2:  branch_cube.U2(); break; case 3:  branch_cube.U3(); break;
                        case 4:  branch_cube.D();  break; case 5:  branch_cube.D2(); break; case 6:  branch_cube.D3(); break;
                        case 7:  branch_cube.F();  break; case 8:  branch_cube.F2(); break; case 9:  branch_cube.F3(); break;
                        case 10: branch_cube.B();  break; case 11: branch_cube.B2(); break; case 12: branch_cube.B3(); break;
                        case 13: branch_cube.L();  break; case 14: branch_cube.L2(); break; case 15: branch_cube.L3(); break;
                        case 16: branch_cube.R();  break; case 17: branch_cube.R2(); break; case 18: branch_cube.R3(); break;
                    }

                    IDAResult res = local_ida.Search(branch_cube, 1, threshold, move, global_solved, current_phase);

                    if (res.solved) {
                        res.path.push_back(move);
                    }

                    return res;
                }));
            }

            bool solved_in_this_iteration = false;
            int min_threshold_this_iter = 1000000;

            for (auto& f : futures) {
                IDAResult res = f.get();
                if (res.solved && !solved_in_this_iteration) {
                    solved_in_this_iteration = true;
                    phase_solution = res.path;
                }
                if (res.next_threshold < min_threshold_this_iter) {
                    min_threshold_this_iter = res.next_threshold;
                }
            }

            if (solved_in_this_iteration) {
                global_solved.store(true, std::memory_order_relaxed);
                break;
            }

            if (min_threshold_this_iter >= 1000000 || min_threshold_this_iter <= threshold) {
                break;
            }

            threshold = min_threshold_this_iter;
        }


        std::reverse(phase_solution.begin(), phase_solution.end());


        for (uint8_t m : phase_solution) {
            full_solution.push_back(m);
            switch (m) {
                case 1:  current_cube.U();  break; case 2:  current_cube.U2(); break; case 3:  current_cube.U3(); break;
                case 4:  current_cube.D();  break; case 5:  current_cube.D2(); break; case 6:  current_cube.D3(); break;
                case 7:  current_cube.F();  break; case 8:  current_cube.F2(); break; case 9:  current_cube.F3(); break;
                case 10: current_cube.B();  break; case 11: current_cube.B2(); break; case 12: current_cube.B3(); break;
                case 13: current_cube.L();  break; case 14: current_cube.L2(); break; case 15: current_cube.L3(); break;
                case 16: current_cube.R();  break; case 17: current_cube.R2(); break; case 18: current_cube.R3(); break;
            }
        }
    }

    isSolved = current_cube.isSolved();
    return full_solution;
}

unsigned long int IDA::getNodeCount() {
    return nodeCount;
}

bool IDA::getIsSolved() {
    return isSolved;
}