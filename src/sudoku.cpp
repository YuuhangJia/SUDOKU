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

// ���������
default_random_engine seed(time(nullptr));
// ���������������������
ranlux48 engine(seed());
// �����������Χ����Ϊ���ȷֲ�
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
        // ����
        int k = (index - curr) * 10;
        while (k-- && getline(file, temp))
            ;
        // ��ȡ
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
    // ���վ����õ��վ���
    int finalNum = countLines(filePathFinal) / 10;

    // ��ȡ���ɸ��վ�
    unordered_set<int> set;
    for (int i = 0; i < gameNum; ++i) {
        set.insert(getRand(0, finalNum));
    }

    vector<int> indexVec(set.begin(), set.end());
    sort(indexVec.begin(), indexVec.end());

    vector<Board> boardVec;
    readBoard(filePathFinal, indexVec, boardVec);

    // ������Ϸ
    vector<Board> gameVec;
    while (gameVec.size() < gameNum) {
        Board board = boardVec[getRand(0, (int)boardVec.size())];
        board.generateGame(getRand(blankMin, blankMax));
        // ��Ҫ��Ψһ�� ���� Ҫ��Ψһ�⵫����Ψһ��
        if (!isUnique || board.hasUniqueSolution()) {
            gameVec.emplace_back(board);
            continue;
        }
    }

    // ���� д�ļ�
    writeBoard(filePathGame, gameVec);
}

// ================================================================= //

void generateFinal(int num) {
    // ��������� ��һ��Ԫ��
    vector<int> head = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    random_shuffle(head.begin(), head.end());

    for (int i = 0; i < 10; i++) {
        swap(head[getRand(0, 9)], head[getRand(0, 9)]);
    }

    // ÿһ������ڵ�һ�е��ƶ�
    vector<int> shift = {0, 3, 6, 1, 4, 7, 2, 5, 8};
    // �����б�
    vector<Board> vec;
    // ����
    int array[9][9] = {};

    for (; next_permutation(head.begin(), head.end());) {
        for (; next_permutation(shift.begin() + 3, shift.begin() + 6);) {
            for (; next_permutation(shift.begin() + 6, shift.end());) {
                for (int i = 0; i < 9; i++) {
                    // ����ÿ�е�Ԫ��
                    for (int k = 0, j = shift[i]; j < shift[i] + 9; k++, j++) {
                        array[i][k] = head[j % 9];
                    }
                }
                // ��������
                vec.emplace_back(array);
                if (--num <= 0) {
                    // ���� д�ļ�
                    writeBoard(filePathFinal, vec);
                    return;
                }
            }
        }
    }
}

void generateAnswer(const string &filename) {
    // ��Ϸ��
    int gameNum = countLines(filename) / 10;

    // ��ȡ��Ϸ
    vector<int> indexVec(gameNum);
    for (int i = 0; i < gameNum; ++i)
        indexVec[i] = i;
    vector<Board> boardVec;
    readBoard(filename, indexVec, boardVec);

    // ���
    vector<Board> answerVec;
    for (auto board : boardVec) {
        board.solve();
        answerVec.push_back(board);
    }

    // �����
    writeBoard(filePathAnswer, answerVec);
}

void generateGame(int num, bool isUnique, int difficulty, int blankMin, int blankMax) {
    // �����Ѷȵ�������
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
