#ifndef RUBIKSCUBESOLVER_CUBE_H
#define RUBIKSCUBESOLVER_CUBE_H

#include <cstdint>

constexpr uint64_t EdgesSolved =
    0b01011'01010'01001'01000'00111'00110'00101'00100'00011'00010'00001'00000;

constexpr uint64_t CornersSolved =
    0b00111'00110'00101'00100'00011'00010'00001'00000;

enum Move : uint8_t {
    NONE = 0,
    U1, U2, U3,
    D1, D2, D3,
    R1, R2, R3,
    L1, L2, L3,
    F1, F2, F3,
    B1, B2, B3
};

struct Cube {
    uint64_t Edges;
    uint64_t Corners;

    constexpr Cube(uint64_t edges, uint64_t corners)
        : Edges(edges), Corners(corners) {}

    bool isSolved() const {
        return Edges == EdgesSolved &&
               Corners == CornersSolved;
    }

private:

    static inline uint64_t cycle4(
        uint64_t value,
        int a,
        int b,
        int c,
        int d
    ) {
        constexpr uint64_t MASK = 31ULL;

        uint64_t va = (value >> (a * 5)) & MASK;
        uint64_t vb = (value >> (b * 5)) & MASK;
        uint64_t vc = (value >> (c * 5)) & MASK;
        uint64_t vd = (value >> (d * 5)) & MASK;

        uint64_t mask =
            (MASK << (a * 5)) |
            (MASK << (b * 5)) |
            (MASK << (c * 5)) |
            (MASK << (d * 5));

        value &= ~mask;

        value |= vd << (a * 5);
        value |= va << (b * 5);
        value |= vb << (c * 5);
        value |= vc << (d * 5);

        return value;
    }

    static inline uint64_t cycle4_inverse(
        uint64_t value,
        int a,
        int b,
        int c,
        int d
    ) {
        constexpr uint64_t MASK = 31ULL;

        uint64_t va = (value >> (a * 5)) & MASK;
        uint64_t vb = (value >> (b * 5)) & MASK;
        uint64_t vc = (value >> (c * 5)) & MASK;
        uint64_t vd = (value >> (d * 5)) & MASK;

        uint64_t mask =
            (MASK << (a * 5)) |
            (MASK << (b * 5)) |
            (MASK << (c * 5)) |
            (MASK << (d * 5));

        value &= ~mask;

        value |= vb << (a * 5);
        value |= vc << (b * 5);
        value |= vd << (c * 5);
        value |= va << (d * 5);

        return value;
    }

    static inline uint64_t cycle4_double(
        uint64_t value,
        int a,
        int b,
        int c,
        int d
    ) {
        constexpr uint64_t MASK = 31ULL;

        uint64_t va = (value >> (a * 5)) & MASK;
        uint64_t vb = (value >> (b * 5)) & MASK;
        uint64_t vc = (value >> (c * 5)) & MASK;
        uint64_t vd = (value >> (d * 5)) & MASK;

        uint64_t mask =
            (MASK << (a * 5)) |
            (MASK << (b * 5)) |
            (MASK << (c * 5)) |
            (MASK << (d * 5));

        value &= ~mask;

        value |= vc << (a * 5);
        value |= vd << (b * 5);
        value |= va << (c * 5);
        value |= vb << (d * 5);

        return value;
    }

    static inline uint64_t toggle_edges(
        uint64_t value,
        uint64_t mask
    ) {
        return value ^ mask;
    }

    static inline uint64_t toggle_corners(
        uint64_t value,
        uint64_t mask
    ) {
        return value ^ mask;
    }

public:

    inline void U() {
        Edges = cycle4(Edges, 0, 1, 2, 3);
        Corners = cycle4(Corners, 0, 1, 2, 3);

        Corners ^=
            (3ULL << 3)  |
            (3ULL << 8)  |
            (3ULL << 13) |
            (3ULL << 18);
    }

    inline void U2() {
        Edges = cycle4_double(Edges, 0, 1, 2, 3);
        Corners = cycle4_double(Corners, 0, 1, 2, 3);
    }

    inline void U3() {
        Edges = cycle4_inverse(Edges, 0, 1, 2, 3);
        Corners = cycle4_inverse(Corners, 0, 1, 2, 3);

        Corners ^=
            (3ULL << 3)  |
            (3ULL << 8)  |
            (3ULL << 13) |
            (3ULL << 18);
    }


    inline void D() {
        Edges = cycle4(Edges, 8, 9, 10, 11);
        Corners = cycle4(Corners, 4, 5, 6, 7);

        Corners ^=
            (3ULL << 23) |
            (3ULL << 28) |
            (3ULL << 33) |
            (3ULL << 38);
    }

    inline void D2() {
        Edges = cycle4_double(Edges, 8, 9, 10, 11);
        Corners = cycle4_double(Corners, 4, 5, 6, 7);
    }

    inline void D3() {
        Edges = cycle4_inverse(Edges, 8, 9, 10, 11);
        Corners = cycle4_inverse(Corners, 4, 5, 6, 7);

        Corners ^=
            (3ULL << 23) |
            (3ULL << 28) |
            (3ULL << 33) |
            (3ULL << 38);
    }


    inline void R() {
        Edges = cycle4(Edges, 3, 6, 11, 7);
        Corners = cycle4(Corners, 2, 6, 7, 3);

        Corners ^=
            (1ULL << 13) |
            (1ULL << 18) |
            (1ULL << 33) |
            (1ULL << 38);
    }

    inline void R2() {
        Edges = cycle4_double(Edges, 3, 6, 11, 7);
        Corners = cycle4_double(Corners, 2, 6, 7, 3);
    }

    inline void R3() {
        Edges = cycle4_inverse(Edges, 3, 6, 11, 7);
        Corners = cycle4_inverse(Corners, 2, 6, 7, 3);

        Corners ^=
            (1ULL << 13) |
            (1ULL << 18) |
            (1ULL << 33) |
            (1ULL << 38);
    }


    inline void L() {
        Edges = cycle4(Edges, 4, 9, 5, 1);
        Corners = cycle4(Corners, 0, 4, 5, 1);

        Corners ^=
            (1ULL << 3)  |
            (1ULL << 8)  |
            (1ULL << 23) |
            (1ULL << 28);
    }

    inline void L2() {
        Edges = cycle4_double(Edges, 4, 9, 5, 1);
        Corners = cycle4_double(Corners, 0, 4, 5, 1);
    }

    inline void L3() {
        Edges = cycle4_inverse(Edges, 4, 9, 5, 1);
        Corners = cycle4_inverse(Corners, 0, 4, 5, 1);

        Corners ^=
            (1ULL << 3)  |
            (1ULL << 8)  |
            (1ULL << 23) |
            (1ULL << 28);
    }


    inline void F() {
        Edges = cycle4(Edges, 0, 7, 8, 4);
        Corners = cycle4(Corners, 0, 3, 7, 4);

        Edges ^=
            (16ULL << 0)  |
            (16ULL << 35) |
            (16ULL << 40) |
            (16ULL << 20);

        Corners ^=
            (2ULL << 3)  |
            (2ULL << 18) |
            (2ULL << 38) |
            (2ULL << 23);
    }

    inline void F2() {
        Edges = cycle4_double(Edges, 0, 7, 8, 4);
        Corners = cycle4_double(Corners, 0, 3, 7, 4);
    }

    inline void F3() {
        Edges = cycle4_inverse(Edges, 0, 7, 8, 4);
        Corners = cycle4_inverse(Corners, 0, 3, 7, 4);

        Edges ^=
            (16ULL << 0)  |
            (16ULL << 35) |
            (16ULL << 40) |
            (16ULL << 20);

        Corners ^=
            (2ULL << 3)  |
            (2ULL << 18) |
            (2ULL << 38) |
            (2ULL << 23);
    }


    inline void B() {
        Edges = cycle4(Edges, 2, 6, 10, 5);
        Corners = cycle4(Corners, 1, 2, 7, 4);

        Edges ^=
            (16ULL << 10) |
            (16ULL << 30) |
            (16ULL << 50) |
            (16ULL << 25);

        Corners ^=
            (2ULL << 8)  |
            (2ULL << 13) |
            (2ULL << 38) |
            (2ULL << 23);
    }

    inline void B2() {
        Edges = cycle4_double(Edges, 2, 6, 10, 5);
        Corners = cycle4_double(Corners, 1, 2, 7, 4);
    }

    inline void B3() {
        Edges = cycle4_inverse(Edges, 2, 6, 10, 5);
        Corners = cycle4_inverse(Corners, 1, 2, 7, 4);

        Edges ^=
            (16ULL << 10) |
            (16ULL << 30) |
            (16ULL << 50) |
            (16ULL << 25);

        Corners ^=
            (2ULL << 8)  |
            (2ULL << 13) |
            (2ULL << 38) |
            (2ULL << 23);
    }
};

#endif