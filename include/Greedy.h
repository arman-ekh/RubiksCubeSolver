#ifndef RUBIKSCUBESOLVER_GREEDY_H
#define RUBIKSCUBESOLVER_GREEDY_H

#include <atomic>
#include <cstdint>
#include <vector>

#include "Cube.h"

struct Result {
    bool solved;
    Cube cube;
    int score;
    std::vector<uint8_t> moves;

    Result()
        : solved(false), cube(EdgesSolved, CornersSolved), score(0) {}

    Result(bool solved, const Cube& cube, int score = 0)
        : solved(solved), cube(cube), score(score) {}
};

class Greedy {
private:
    long int max_evaluate;
    bool is_solved;
    long int node_count;

    static bool PRUNE_TABLE[19][19];
    static bool isTableInitialized;

    static inline int get_face(uint8_t move) {
        if (move == 0)
            return -1;
        return (move - 1) / 3;
    }

    static inline bool calc_should_prune(uint8_t last_move,uint8_t next_move) {
        if (last_move == 0)
            return false;

        int last_face = get_face(last_move);
        int next_face = get_face(next_move);

        if (last_face == next_face)
            return true;
        if ((last_face == 0 && next_face == 1) ||
            (last_face == 2 && next_face == 3) ||
            (last_face == 4 && next_face == 5)) {
            return true;
        }
        return false;
    }
public:
    Greedy();
    Result search(Cube cube,int depth,uint8_t last_move, std::atomic<bool>& global_solved,int eval);
    bool isSolved();
    static void initPruneTable();
    static Result ParallelSearch(const Cube& start_cube,std::atomic<bool>& global_solved,std::atomic<long int>& global_node_count);
    long int getNodeCount() {
        return node_count;
    };
};

#endif