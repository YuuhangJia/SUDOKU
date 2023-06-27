#include "board.h"
#include <cstdlib>
#include <ctime>

Board::Board(const std::string &str) {
    int pos = 0;
    for (auto &row : grid) {
        for (int &item : row) {
            item = str[pos++] - '0';
        }
        pos++;
    }
}

Board::Board(const int array[MaxRow][MaxCol]) {
    memcpy(grid, array, MaxRow * MaxCol * sizeof(int));
}

void Board::solve() {
    int count = 0;
    solveDFS(0, 0, count);
}

void Board::generateGame(int blankNum) {
    int base[9] = {0, 3, 6, 27, 30, 33, 54, 57, 60};
    int offset[9] = {0, 1, 2, 9, 10, 11, 18, 19, 20};

    // 挖空九宫格的格子
    for (int k : base) {
        int num = blankNum / 9;
        while (num--) {
            int dot;
            do {
                dot = k + offset[rand() % 9];
            } while (grid[dot / 9][dot % 9] == 0);
            grid[dot / 9][dot % 9] = 0;
        }
    }

    // 随机挖空剩余的格子
    int num = blankNum % 9;
    while (num--) {
        int dot;
        do {
            dot = rand() % 81;
        } while (grid[dot / 9][dot % 9] == 0);
        grid[dot / 9][dot % 9] = 0;
    }
}

bool Board::hasUniqueSolution() {
    int count = 0;
    int temp[MaxRow][MaxCol]{};
    memcpy(temp, grid, MaxRow * MaxCol * sizeof(int));
    solveDFS(0, 0, count);
    memcpy(grid, temp, MaxRow * MaxCol * sizeof(int));
    return count == 1;
}

std::string Board::toString() const {
    std::string str;
    for (const auto &row : grid) {
        for (const auto item : row) {
            if(item == 0) {
                str += '$'; // 空格用'$'表示
                continue;
            }
            str += std::to_string(item); // 数字转为字符串
        }
        str += "\n";
    }
    str += "\n";
    return str;
}

bool Board::isValid(int row, int col, int num) const {
    for (int k = 0; k < 9; k++) {
        if (grid[row][k] == num || grid[k][col] == num || grid[(row / 3) * 3 + k / 3][(col / 3) * 3 + k % 3] == num) {
            return false;
        }
    }
    return true;
}

void Board::solveDFS(int row, int col, int &count) {
    if (count >= 2) {
        return;
    }

    if (row == MaxRow) {
        count++;
        return;
    }

    if (col == MaxCol) {
        solveDFS(row + 1, 0, count);
        return;
    }

    if (grid[row][col] != 0) {
        solveDFS(row, col + 1, count);
        return;
    }

    for (int num = 1; num <= 9; num++) {
        if (isValid(row, col, num)) {
            grid[row][col] = num;
            int temp = count;
            solveDFS(row, col + 1, count);
            if (count >= 2) {
                return;
            } else if (count == temp + 1) {
                continue;
            }
        }
    }
}
