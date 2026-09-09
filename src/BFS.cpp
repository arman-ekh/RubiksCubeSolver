#include "../include/BFS.h"
#include <vector>
#include <omp.h>


inline int get_face(uint8_t move) {
    if (move == NONE) return -1;
    return (move - 1) / 3;
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
        case U2: cube.U(); cube.U(); break;
        case U3: cube.U(); cube.U(); cube.U(); break;

        case D1: cube.D(); break;
        case D2: cube.D(); cube.D(); break;
        case D3: cube.D(); cube.D(); cube.D(); break;

        case R1: cube.R(); break;
        case R2: cube.R(); cube.R(); break;
        case R3: cube.R(); cube.R(); cube.R(); break;

        case L1: cube.L(); break;
        case L2: cube.L(); cube.L(); break;
        case L3: cube.L(); cube.L(); cube.L(); break;

        case F1: cube.F(); break;
        case F2: cube.F(); cube.F(); break;
        case F3: cube.F(); cube.F(); cube.F(); break;

        case B1: cube.B(); break;
        case B2: cube.B(); cube.B(); break;
        case B3: cube.B(); cube.B(); cube.B(); break;
        default: break;
    }
}

void BFS::initialize(Cube cube) {
    while (!queue.empty()) queue.pop();
    visited.clear();

    cube.setLastMove(NONE);
    queue.push(cube);
    visited.insert(cube);
    found = false;
}

bool BFS::is_finished() {
    return queue.empty() || found;
}

bool BFS::is_found() {
    return found;
}


void BFS::step() {
    size_t level_size = queue.size();

    for (size_t i = 0; i < level_size; ++i) {
        Cube current = queue.front();
        queue.pop();

        if (current.isSolved()) {
            found = true;
            return;
        }

        uint8_t last_move = current.getLastMove();

        for (uint8_t m = U1; m <= B3; ++m) {
            if (should_prune(last_move, m)) continue;

            Cube next_cube = current;
            apply_move(next_cube, m);
            next_cube.setLastMove(m);

            if (!visited.contains(next_cube)) {
                if (next_cube.isSolved()) {
                    found = true;
                    return;
                }
                visited.insert(next_cube);
                queue.push(next_cube);
            }
        }
    }
}

void BFS::step_parallel() {
    size_t level_size = queue.size();
    if (level_size == 0 || found) return;

    std::vector<Cube> current_level;
    current_level.reserve(level_size);
    while (!queue.empty()) {
        current_level.push_back(queue.front());
        queue.pop();
    }

    std::vector<Cube> next_level_nodes;
    bool global_found = false;

    #pragma omp parallel
    {
        std::vector<Cube> local_next;

        #pragma omp for schedule(dynamic)
        for (size_t i = 0; i < current_level.size(); ++i) {
            if (global_found) continue;

            Cube current = current_level[i];

            if (current.isSolved()) {
                #pragma omp atomic write
                global_found = true;
                continue;
            }

            uint8_t last_move = current.getLastMove();

            for (uint8_t m = U1; m <= B3; ++m) {
                if (should_prune(last_move, m)) continue;

                Cube next_cube = current;
                apply_move(next_cube, m);
                next_cube.setLastMove(m);

                bool is_new = false;
                #pragma omp critical(visited_lock)
                {
                    if (!visited.contains(next_cube)) {
                        visited.insert(next_cube);
                        is_new = true;
                    }
                }

                if (is_new) {
                    if (next_cube.isSolved()) {
                        #pragma omp atomic write
                        global_found = true;
                    }
                    local_next.push_back(next_cube);
                }
            }
        }

        #pragma omp critical(queue_merge_lock)
        {
            next_level_nodes.insert(next_level_nodes.end(), local_next.begin(), local_next.end());
        }
    }

    if (global_found) {
        found = true;
        return;
    }

    for (const auto& cube : next_level_nodes) {
        queue.push(cube);
    }
}