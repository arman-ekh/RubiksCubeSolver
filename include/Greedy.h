#ifndef RUBIKSCUBESOLVER_IDA_STAR_H
#define RUBIKSCUBESOLVER_IDA_STAR_H
#include <atomic>

#include "Cube.h"


class Greedy {
    private:
        long int max_evaluate;
        bool is_solved;
        long int node_count;
    public:
        Greedy();
        void search(Cube cube , int depth , uint8_t last_move ,  const std::atomic<bool>& global_solved);
        bool isSolved();
};


#endif