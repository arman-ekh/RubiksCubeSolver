#ifndef RUBIKSCUBESOLVER_BFS_H
#define RUBIKSCUBESOLVER_BFS_H
#include <queue>
#include <set>

#include "Cube.h"


class BFS {
    private:
        std::queue<Cube> queue;
        std::set<Cube> set;
        bool found;
    public:
        void initialize(Cube cube);
        void step();
        bool is_finished();
        bool is_found();
};


#endif