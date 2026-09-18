#include <bitset>
#include <iostream>
#include <chrono>
#include <random>


#include "Greedy.h"
#include "IDA.h"
#include "include/Cube.h"
#include "include/DFS.h"



const inline char* move_name(uint8_t move) {
    switch (move) {
        case 1:  return "U";
        case 2:  return "U2";
        case 3:  return "U'";

        case 4:  return "D";
        case 5:  return "D2";
        case 6:  return "D'";

        case 7:  return "F";
        case 8:  return "F2";
        case 9:  return "F'";

        case 10: return "B";
        case 11: return "B2";
        case 12: return "B'";

        case 13: return "L";
        case 14: return "L2";
        case 15: return "L'";

        case 16: return "R";
        case 17: return "R2";
        case 18: return "R'";

        default: return "?";
    }
}
void scrambleCube(Cube& cube, int moveCount = 12) {

    std::mt19937 rng(static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> dist(1, 18);

    for (int i = 0; i < moveCount; ++i) {
        int move = dist(rng);
        switch (move) {
            case 1:  cube.U();  break;
            case 2:  cube.U2(); break;
            case 3:  cube.U3(); break;
            case 4:  cube.D();  break;
            case 5:  cube.D2(); break;
            case 6:  cube.D3(); break;
            case 7:  cube.F();  break;
            case 8:  cube.F2(); break;
            case 9:  cube.F3(); break;
            case 10: cube.B();  break;
            case 11: cube.B2(); break;
            case 12: cube.B3(); break;
            case 13: cube.L();  break;
            case 14: cube.L2(); break;
            case 15: cube.L3(); break;
            case 16: cube.R();  break;
            case 17: cube.R2(); break;
            case 18: cube.R3(); break;
        }
    }
}

int main() {


    Cube cube(EdgesSolved, CornersSolved);

    scrambleCube(cube);

    std::atomic<unsigned long int> total_nodes{0};
    std::atomic<bool> atomic_solved{false};
    int target_depth = 8;

    IDA solver;
    std::atomic<unsigned long int> global_nodes{0};

    auto start_time = std::chrono::high_resolution_clock::now();


    std::vector<uint8_t> solution_path = solver.ParallelSearch(cube, global_nodes);

    // Result result = Greedy::ParallelSearch(cube, atomic_solved, total_nodes);

    auto end_time = std::chrono::high_resolution_clock::now();

    if (solver.getIsSolved()) {
        std::cout << "Cube Solved Successfully!\n";
        std::cout << "Total Moves: " << solution_path.size() << "\n";
        std::cout << "Nodes Evaluated: " << global_nodes.load() << "\n\n";

        std::cout << "Solution Path:\n";
        for (uint8_t m : solution_path) {
            std::cout << move_name(m) << " ";
        }
        std::cout << "\n";
    } else {
        std::cout << "Failed to solve the cube.\n";
    }


    std::chrono::duration<double> duration = end_time - start_time;
    std::cout << "Time Elapsed: " << duration.count() << " seconds" << std::endl;

    return 0;
}
