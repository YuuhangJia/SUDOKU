#ifndef SUDOKU_SUDOKU_H
#define SUDOKU_SUDOKU_H

#include "board.h"
#include <string>

void generateFinal(int num);

void generateAnswer(const std::string &filename);

void generateGame(int num, bool isUnique, int difficulty, int minBlank, int maxBlank);

#endif //SUDOKU_SUDOKU_H