#include "../include/BFS.h"

#include <iostream>

void BFS::initialize(Cube cube) {
    queue.push(cube);
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
    for (int i =0; i < queue.size(); i++) {
        std::cout << queue.size() << "\n";
        Cube cube = queue.front();
        if (cube.isSolved() ) {
            found = true;
            return;
        }

        queue.pop();
        if (set.contains(cube)) {
            continue;
        }
        set.insert(cube);
        Cube cube1 = Cube(cube.Edges , cube.Corners);
        cube1.U();
        queue.push(cube1);

        Cube cube2 = Cube(cube.Edges , cube.Corners);
        cube2.U();
        cube2.U();
        queue.push(cube2);

        Cube cube3 = Cube(cube.Edges , cube.Corners);
        cube3.U();
        cube3.U();
        cube3.U();
        queue.push(cube3);

        Cube cube4 = Cube(cube.Edges , cube.Corners);
        cube4.D();
        queue.push(cube4);

        Cube cube5 = Cube(cube.Edges , cube.Corners);
        cube5.D();
        cube5.D();
        queue.push(cube5);


        Cube cube6 = Cube(cube.Edges , cube.Corners);
        cube6.D();
        cube6.D();
        cube6.D();
        queue.push(cube6);

        Cube cube7 = Cube(cube.Edges , cube.Corners);
        cube7.R();
        queue.push(cube7);

        Cube cube8 = Cube(cube.Edges , cube.Corners);
        cube8.R();
        cube8.R();
        queue.push(cube8);


        Cube cube9 = Cube(cube.Edges , cube.Corners);
        cube9.R();
        cube9.R();
        cube9.R();
        queue.push(cube9);

        Cube cube10 = Cube(cube.Edges , cube.Corners);
        cube10.L();
        queue.push(cube10);

        Cube cube11 = Cube(cube.Edges , cube.Corners);
        cube11.L();
        cube11.L();
        queue.push(cube11);

        Cube cube12 = Cube(cube.Edges , cube.Corners);
        cube12.L();
        cube12.L();
        cube12.L();
        queue.push(cube12);

        Cube cube13 = Cube(cube.Edges , cube.Corners);
        cube13.F();
        queue.push(cube13);

        Cube cube14 = Cube(cube.Edges , cube.Corners);
        cube14.F();
        cube14.F();
        queue.push(cube14);

        Cube cube15 = Cube(cube.Edges , cube.Corners);
        cube15.F();
        cube15.F();
        cube15.F();
        queue.push(cube15);

        Cube cube16 = Cube(cube.Edges , cube.Corners);
        cube16.B();
        queue.push(cube16);

        Cube cube17 = Cube(cube.Edges , cube.Corners);
        cube17.B();
        cube17.B();
        queue.push(cube17);

        Cube cube18 = Cube(cube.Edges , cube.Corners);
        cube18.B();
        cube18.B();
        cube18.B();
        queue.push(cube18);
    }
}