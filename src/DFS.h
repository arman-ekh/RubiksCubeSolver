#ifndef RUBIKSCUBESOLVER_DFS_H
#define RUBIKSCUBESOLVER_DFS_H
#include "Cube.h"


class DFS {

    private:
        long int nodeCount;
        bool isSolved;
    public:
        DFS();
        void Search(Cube& cube, int depth, uint8_t last_move);
        long int getNodeCount();
};


#endif