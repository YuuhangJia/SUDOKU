#ifndef SUDOKU_BOARD_H
#define SUDOKU_BOARD_H

#include <string>
#include <cstring>
#include <vector>

using namespace std;


class Board {
    static const int MaxRow = 9;
    static const int MaxCol = 9;

    int grid[MaxRow][MaxCol]{};

    bool isValid(int row, int col, int num) const;

    void solveDFS(int row, int col, int &count);

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