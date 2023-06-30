#include "board.h"
#include "sudoku.h"
#include <vector>
#include <fstream>
#include <algorithm>
#include <unordered_set>
#include <random>
#include <ctime>

using namespace std;

const string filePathGame = "game.txt";
const string filePathFinal = "final.txt";
const string filePathAnswer = "answer.txt";

// 随机数种子
default_random_engine seed(time(nullptr));
// 利用种子生成随机数引擎
ranlux48 engine(seed());
// 设置随机数范围，并为均匀分布
uniform_int_distribution<> distrib(0, 1000000);

int getRand(int min, int max) {
    return min == max ? min : distrib(engine) % (max - min) + min;
}

void readBoard(const string &filename, const vector<int> &indexVec, vector<Board> &boardVec) {
    ifstream file(filename);
    if (!file.is_open()) {
        printf("Error: Could not open\n");
        exit(-1);
    }
    int curr = 0;
    string temp;
    for (const auto index : indexVec) {
        // 跳过
        int k = (index - curr) * 10;
        while (k-- && getline(file, temp))
            ;
        // 读取
        string str;
        for (int i = 0; i < 10; i++) {
            getline(file, temp);
            std::replace(temp.begin(), temp.end(), '$', '0');
            str += temp;
            str += "\n";
        }
        boardVec.emplace_back(str);
        curr = index + 1;
    }
    file.close();
}

void writeBoard(const string &filename, const vector<Board> &vec) {
    ofstream file(filename, ios::app);
    for (const auto &board : vec) {
        file << board.toString();
    }
    file.close();
}

long countLines(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        printf("Error: Could not open\n");
        exit(-1);
    }

    int n = 0;
    string temp;
    while (getline(file, temp, '\n'))
        n++;
    file.close();
    return n;
}

// ================================================================= //

void generateGame(int gameNum, int blankMin, int blankMax, bool isUnique) {
    // 从终局里拿到终局数
    int finalNum = countLines(filePathFinal) / 10;

    // 读取若干个终局
    unordered_set<int> set;
    for (int i = 0; i < gameNum; ++i) {
        set.insert(getRand(0, finalNum));
    }

    vector<int> indexVec(set.begin(), set.end());
    sort(indexVec.begin(), indexVec.end());

    vector<Board> boardVec;
    readBoard(filePathFinal, indexVec, boardVec);

    // 生成游戏
    vector<Board> gameVec;
    while (gameVec.size() < gameNum) {
        Board board = boardVec[getRand(0, (int)boardVec.size())];
        board.generateGame(getRand(blankMin, blankMax));
        // 不要求唯一解 或者 要求唯一解但是有唯一解
        if (!isUnique || board.hasUniqueSolution()) {
            gameVec.emplace_back(board);
            continue;
        }
    }

    // 保存 写文件
    writeBoard(filePathGame, gameVec);
}

// ================================================================= //

void generateFinal(int num) {
    // 随机化处理 第一行元素
    vector<int> head = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    random_shuffle(head.begin(), head.end());

    for (int i = 0; i < 10; i++) {
        swap(head[getRand(0, 9)], head[getRand(0, 9)]);
    }

    // 每一行相对于第一行的移动
    vector<int> shift = {0, 3, 6, 1, 4, 7, 2, 5, 8};
    // 棋盘列表
    vector<Board> vec;
    // 数据
    int array[9][9] = {};

    for (; next_permutation(head.begin(), head.end());) {
        for (; next_permutation(shift.begin() + 3, shift.begin() + 6);) {
            for (; next_permutation(shift.begin() + 6, shift.end());) {
                for (int i = 0; i < 9; i++) {
                    // 处理每行的元素
                    for (int k = 0, j = shift[i]; j < shift[i] + 9; k++, j++) {
                        array[i][k] = head[j % 9];
                    }
                }
                // 构造棋盘
                vec.emplace_back(array);
                if (--num <= 0) {
                    // 保存 写文件
                    writeBoard(filePathFinal, vec);
                    return;
                }
            }
        }
    }
}

void generateAnswer(const string &filename) {
    // 游戏数
    int gameNum = countLines(filename) / 10;

    // 读取游戏
    vector<int> indexVec(gameNum);
    for (int i = 0; i < gameNum; ++i)
        indexVec[i] = i;
    vector<Board> boardVec;
    readBoard(filename, indexVec, boardVec);

    // 解答
    vector<Board> answerVec;
    for (auto board : boardVec) {
        board.solve();
        answerVec.push_back(board);
    }

    // 保存答案
    writeBoard(filePathAnswer, answerVec);
}

void generateGame(int num, bool isUnique, int difficulty, int blankMin, int blankMax) {
    // 根据难度调整块数
    switch (difficulty) {
        case 1:
            blankMin = 20;
            blankMax = 30;
            break;
        case 2:
            blankMin = 30;
            blankMax = 40;
            break;
        case 3:
            blankMin = 40;
            blankMax = 50;
            break;
        default:
            break;
    }
    generateGame(num, blankMin, blankMax, isUnique);
}
