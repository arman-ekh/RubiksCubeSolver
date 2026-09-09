#include "../include/BFS.h"

void BFS::initialize(Cube cube) {
    queue.push(cube);
    set.insert(cube);
    found = false;
}

bool BFS::is_finished() {
    if (queue.empty() || found) {
        return true;
    }else {
        return false;
    }
}

bool BFS::is_found() {
    return found;
}

void BFS::step() {

}
