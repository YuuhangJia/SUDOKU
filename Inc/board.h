#ifndef SUDOKU_BOARD_H
#define SUDOKU_BOARD_H

#include <string>
#include <cstring>
#include <vector>
#include <unordered_map>


class Board {
    static const int MaxRow = 9;
    static const int MaxCol = 9;

    int grid[MaxRow][MaxCol]{};

    int rowR[15],colR[15],pointR[15][15];//二进制压缩数组，用于记录哪些数字已经被使用过；行，列，每个九宫格的中心（表示这个九宫格） 

    bool isValid(int row, int col, int num) const;

    void solveDFS(int row, int col, int &count);

    void solveImproved(int row, int col, int &count);

    void myInit();

public:
    Board() = default;

    explicit Board(const std::string &str);

    explicit Board(const int array[MaxRow][MaxCol]);

    void solve();

    void generateGame(int blockNum);

    bool hasUniqueSolution();

    std::string toString() const;
};

#endif // SUDOKU_BOARD_H