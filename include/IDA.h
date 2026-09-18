#ifndef RUBIKSCUBESOLVER_IDA_H
#define RUBIKSCUBESOLVER_IDA_H
#include <atomic>

#include "Cube.h"
#include <cstdint>
#include <vector>

struct SubGoal {
    uint64_t edge_mask;
    uint64_t corner_mask;
};

struct IDAResult {
    bool solved;
    int next_threshold;
    std::vector<uint8_t> path;
};

class IDA {
private:
    unsigned long int nodeCount;
    bool isSolved;

    static bool PRUNE_TABLE[19][19];
    static bool isTableInitialized;

    static inline int get_face(uint8_t move) {
        if (move == 0) return -1; // NONE
        return (move - 1) / 3;
    }

    static inline bool calc_should_prune(uint8_t last_move, uint8_t next_move) {
        if (last_move == 0) return false; // NONE

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

public:
    IDA();
    static void initPruneTable();
    std::vector<uint8_t> ParallelSearch(const Cube &start_cube, std::atomic<unsigned long int> &global_node_count);
    IDAResult Search(Cube cube, int g, int threshold, uint8_t last_move, std::atomic<bool>& global_solved, const SubGoal& phase);
    unsigned long int getNodeCount();
    bool getIsSolved();
};


#endif