#ifndef RUBIKSCUBESOLVER_BFS_H
#define RUBIKSCUBESOLVER_BFS_H

#include <queue>
#include <unordered_set>
#include <cstddef>
#include "Cube.h"

struct CubeHash {
    std::size_t operator()(const Cube& c) const {
        std::size_t h1 = std::hash<uint64_t>{}(c.Edges);
        std::size_t h2 = std::hash<uint64_t>{}(c.Corners & CORNERS_MASK);
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

class BFS {
private:
    std::queue<Cube> queue;
    std::unordered_set<Cube, CubeHash> visited;
    bool found;

public:
    void initialize(Cube cube);
    void step();
    void step_parallel();
    bool is_finished();
    bool is_found();
};

#endif