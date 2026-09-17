#ifndef RUBIKSCUBESOLVER_RESULT_H
#define RUBIKSCUBESOLVER_RESULT_H

struct result {
    private:
        bool isSolved;
        int eval;
    public:
        result(bool isSolved, int eval) {
            this->isSolved = isSolved;
            this->eval = eval;
        };
    bool IsSolved() {
        return isSolved;
    }
    int get_eval() {
        return eval;
    }

};
#endif
