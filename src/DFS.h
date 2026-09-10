#ifndef RUBIKSCUBESOLVER_DFS_H
#define RUBIKSCUBESOLVER_DFS_H
#include <atomic>

#include "Cube.h"


class DFS {

    private:
        long int nodeCount;
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
        DFS();
        static void initPruneTable();
        void Search(Cube cube, int depth, uint8_t last_move, const std::atomic<bool>& global_solved);
        static bool ParallelSearch(const Cube& start_cube, int depth, std::atomic<long int>& global_node_count);
        long int getNodeCount();
        bool getIsSolved() {
            return isSolved;
        };
};


#endif