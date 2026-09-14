#ifndef RUBIKSCUBESOLVER_IDA_STAR_H
#define RUBIKSCUBESOLVER_IDA_STAR_H
#include <atomic>

#include "Cube.h"


class IDA_star {
    private:
        long int max_heuristic;
        bool is_solved;
        long int node_count;
    public:
        IDA_star();
        void search(Cube cube , int depth , uint8_t last_move ,  const std::atomic<bool>& global_solved);
        bool isSolved();
};


#endif