#include "Greedy.h"

#include <array>
#include <future>
#include <iostream>
#include <vector>

bool Greedy::PRUNE_TABLE[19][19];
bool Greedy::isTableInitialized = false;

Greedy::Greedy() {
    initPruneTable();
    node_count =0;
    is_solved = false;
}

void Greedy::initPruneTable() {
    if (isTableInitialized) return;
    for (uint8_t last = 0; last <= 18; ++last) {
        for (uint8_t next = 0; next <= 18; ++next) {
            PRUNE_TABLE[last][next] = calc_should_prune(last, next);
        }
    }
    isTableInitialized = true;
}

bool Greedy::isSolved() {
    return is_solved;
}

inline int evaluate_cube(const Cube& cube) {
    int correct_cubies = 0;
    constexpr uint64_t MASK = 31ULL;

    for (int i = 0; i < 12; ++i) {
        uint64_t solved_edge = (EdgesSolved >> (i * 5)) & MASK;
        uint64_t current_edge = (cube.Edges >> (i * 5)) & MASK;
        if (solved_edge == current_edge) {
            correct_cubies++;
        }
    }

    for (int i = 0; i < 8; ++i) {
        uint64_t solved_corner = (CornersSolved >> (i * 5)) & MASK;
        uint64_t current_corner = (cube.Corners >> (i * 5)) & MASK;
        if (solved_corner == current_corner) {
            correct_cubies++;
        }
    }

    int score = correct_cubies * 1000;
    return score;
}

Result Greedy::search(
    Cube cube,
    int depth,
    uint8_t last_move,
     std::atomic<bool>& global_solved,
    int eval
) {
    ++node_count;

    if (global_solved.load(std::memory_order_relaxed)) {
        return Result(false, cube, eval);
    }

    if (cube.isSolved()) {
        return Result(true, cube, eval);
    }

    if (depth == 0) {
        return Result(false, cube, eval);
    }

    Result best(false, cube, eval);

#define TRY_MOVE(MOVE, DO_MOVE)                                             \
    do {                                                                    \
        if (!PRUNE_TABLE[last_move][MOVE]) {                                \
            Cube next_cube = cube;                                          \
            next_cube.DO_MOVE();                                            \
                                                                            \
            int next_eval = evaluate_cube(next_cube);                      \
                                                                            \
            if (next_cube.isSolved()) {                                    \
                global_solved.store(true, std::memory_order_relaxed);      \
                                                                            \
                Result result(true, next_cube, next_eval);                  \
                result.moves.push_back(MOVE);                              \
                return result;                                             \
            }                                                               \
                                                                            \
            if (next_eval >= eval - 6000) {                                        \
                Result result = search(                                    \
                    next_cube,                                             \
                    depth - 1,                                             \
                    MOVE,                                                  \
                    global_solved,                                         \
                    next_eval                                              \
                );                                                          \
                                                                            \
                if (result.solved) {                                       \
                    result.moves.insert(result.moves.begin(), MOVE);       \
                    return result;                                          \
                }                                                           \
                                                                            \
                if (result.score > best.score) {                           \
                    result.moves.insert(result.moves.begin(), MOVE);       \
                    best = result;                                         \
                }                                                           \
            }                                                               \
        }                                                                   \
    } while (0)

    TRY_MOVE(1, U);
    TRY_MOVE(2, U2);
    TRY_MOVE(3, U3);

    TRY_MOVE(4, D);
    TRY_MOVE(5, D2);
    TRY_MOVE(6, D3);

    TRY_MOVE(7, F);
    TRY_MOVE(8, F2);
    TRY_MOVE(9, F3);

    TRY_MOVE(10, B);
    TRY_MOVE(11, B2);
    TRY_MOVE(12, B3);

    TRY_MOVE(13, L);
    TRY_MOVE(14, L2);
    TRY_MOVE(15, L3);

    TRY_MOVE(16, R);
    TRY_MOVE(17, R2);
    TRY_MOVE(18, R3);

#undef TRY_MOVE

    return best;
}

const inline char* move_name(uint8_t move) {
    switch (move) {
        case 1:  return "U";
        case 2:  return "U2";
        case 3:  return "U'";

        case 4:  return "D";
        case 5:  return "D2";
        case 6:  return "D'";

        case 7:  return "F";
        case 8:  return "F2";
        case 9:  return "F'";

        case 10: return "B";
        case 11: return "B2";
        case 12: return "B'";

        case 13: return "L";
        case 14: return "L2";
        case 15: return "L'";

        case 16: return "R";
        case 17: return "R2";
        case 18: return "R'";

        default: return "?";
    }
}

Result Greedy::ParallelSearch(
    const Cube& start_cube,
    std::atomic<bool>& global_solved,
    std::atomic<unsigned long int>& global_node_count
) {
    if (start_cube.isSolved()) {
        global_node_count.fetch_add(1, std::memory_order_relaxed);

        Result result(true, start_cube, 20 * 1000);
        return result;
    }

    if (global_solved.load(std::memory_order_relaxed)) {
        return Result(false, start_cube, evaluate_cube(start_cube));
    }

    std::array<Result, 18> results;
    std::vector<std::future<long int>> futures;

    futures.reserve(18);

    for (uint8_t move = 1; move <= 18; ++move) {
        int index = move - 1;

        futures.push_back(
            std::async(
                std::launch::async,
                [start_cube, move, index, &results, &global_solved]()
                -> long int {

                    Greedy local_greedy;

                    Cube branch_cube = start_cube;

                    switch (move) {
                        case 1:  branch_cube.U();  break;
                        case 2:  branch_cube.U2(); break;
                        case 3:  branch_cube.U3(); break;

                        case 4:  branch_cube.D();  break;
                        case 5:  branch_cube.D2(); break;
                        case 6:  branch_cube.D3(); break;

                        case 7:  branch_cube.F();  break;
                        case 8:  branch_cube.F2(); break;
                        case 9:  branch_cube.F3(); break;

                        case 10: branch_cube.B();  break;
                        case 11: branch_cube.B2(); break;
                        case 12: branch_cube.B3(); break;

                        case 13: branch_cube.L();  break;
                        case 14: branch_cube.L2(); break;
                        case 15: branch_cube.L3(); break;

                        case 16: branch_cube.R();  break;
                        case 17: branch_cube.R2(); break;
                        case 18: branch_cube.R3(); break;
                    }

                    if (branch_cube.isSolved()) {
                        global_solved.store(
                            true,
                            std::memory_order_relaxed
                        );

                        Result result(true, branch_cube, evaluate_cube(branch_cube));

                        result.moves.push_back(move);

                        results[index] = result;

                        return local_greedy.getNodeCount();
                    }

                    int initial_eval = evaluate_cube(branch_cube);

                    Result result = local_greedy.search(
                        branch_cube,
                        7,
                        move,
                        global_solved,
                        initial_eval
                    );

                    result.moves.insert(
                        result.moves.begin(),
                        move
                    );

                    results[index] = result;

                    if (result.solved) {
                        global_solved.store(
                            true,
                            std::memory_order_relaxed
                        );
                    }

                    return local_greedy.getNodeCount();
                }
            )
        );
    }

    long int batch_nodes = 0;

    for (auto& future : futures) {
        batch_nodes += future.get();
    }

    global_node_count.fetch_add(
        batch_nodes,
        std::memory_order_relaxed
    );

    if (global_solved.load(std::memory_order_relaxed)) {
        for (int i = 0; i < 18; ++i) {
            if (results[i].solved) {
                return results[i];
            }
        }
    }

    Result best_result = results[0];

    for (int i = 1; i < 18; ++i) {
        if (results[i].score > best_result.score) {
            best_result = results[i];
        }
    }

    int start_score = evaluate_cube(start_cube);

    if (best_result.score <= start_score) {
        std::cout << "No improvement found.\n";
        return best_result;
    }

    std::cout << "Current best score: "
              << best_result.score
              << '\n';

    std::cout << "Moves so far: ";

    for (uint8_t move : best_result.moves) {
        std::cout << move_name(move) << ' ';
    }

    std::cout << '\n';

    Result next_result = ParallelSearch(
        best_result.cube,
        global_solved,
        global_node_count
    );

    if (next_result.solved) {
        best_result.moves.insert(
            best_result.moves.end(),
            next_result.moves.begin(),
            next_result.moves.end()
        );

        best_result.cube = next_result.cube;
        best_result.score = next_result.score;
        best_result.solved = true;

        return best_result;
    }

    return best_result;
}