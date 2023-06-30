#include <gtest/gtest.h>
#include <fstream>
#include <vector>
#include <sstream>
#include "main.cpp"
#include "board.h"
#include "board.cpp"
#include "sudoku.h"
#include "sudoku.cpp"

//测试主函数
TEST(MainTest, GenerateFinalMode) {
    char* argv[] = { (char*)"./sudoku", (char*)"-c10000" };  //生成10000个终局;
    int argc = sizeof(argv) / sizeof(argv[0]);

    // 运行主函数
    ::testing::internal::CaptureStdout(); // 捕获标准输出
    MainTest(argc, argv);
    std::string output = ::testing::internal::GetCapturedStdout(); // 获取标准输出
    std::ifstream file("./final.txt");
    bool fileExists = file.good();
    file.close();


    // 进行断言，验证结果是否符合预期
    EXPECT_TRUE(output.find("success.") != std::string::npos);
    EXPECT_TRUE(fileExists);
}

TEST(MainTest, GenerateGameMode1) {
    char* argv[] = { (char*)"./sudoku", (char*)"-n100 -m3" }; //生成100局难度为3的游戏;
    int argc = sizeof(argv) / sizeof(argv[0]);

    ::testing::internal::CaptureStdout();
    MainTest(argc, argv);
    std::string output = ::testing::internal::GetCapturedStdout();
    std::ifstream file("./game.txt");
    bool fileExists = file.good();
    file.close();

    EXPECT_TRUE(output.find("success.") != std::string::npos);
    EXPECT_TRUE(fileExists);
}

TEST(MainTest, GenerateGameMode2) {
    char* argv[] = { (char*)"./sudoku", (char*)"-n100 -r20~45 -u" }; //生成100局挖空在20~45之间的游戏;
    int argc = sizeof(argv) / sizeof(argv[0]);

    ::testing::internal::CaptureStdout();
    MainTest(argc, argv);
    std::string output = ::testing::internal::GetCapturedStdout();
    std::ifstream file("./game.txt");
    bool fileExists = file.good();
    file.close();

    EXPECT_TRUE(output.find("success.") != std::string::npos);
    EXPECT_TRUE(fileExists);
}

TEST(MainTest, generateAnswerMode) {
    char* argv[] = { (char*)"./sudoku", (char*)"-s ./game.txt" }; //解决上述生成的100局游戏;
    int argc = sizeof(argv) / sizeof(argv[0]);

    ::testing::internal::CaptureStdout();
    MainTest(argc, argv);
    std::string output = ::testing::internal::GetCapturedStdout();
    std::ifstream file("./answer.txt");
    bool fileExists = file.good();
    file.close();

    EXPECT_TRUE(output.find("success.") != std::string::npos);
    EXPECT_TRUE(fileExists);
}

//边界测试:
TEST(MainTest, GenerateFinalBoarderTest) {
    char* argv[] = { (char*)"./sudoku", (char*)"-c0" };  //生成0个终局;
    int argc = sizeof(argv) / sizeof(argv[0]);

    // 运行主函数
    ::testing::internal::CaptureStdout(); // 捕获标准输出
    MainTest(argc, argv);
    std::string output = ::testing::internal::GetCapturedStdout(); // 获取标准输出
    std::ifstream file("./final.txt");
    bool fileExists = file.good();
    file.close();
    // std::cout << output;

    // 进行断言，验证结果是否符合预期
    EXPECT_TRUE(output.find("failed.") != std::string::npos);
    // EXPECT_FALSE(fileExists); //之前生成过了，这儿就不做判断了;
}

TEST(MainTest, GenerateGameBoarderTest1) {
    char* argv[] = { (char*)"./sudoku", (char*)"-n100 -m0" }; //生成100局难度为0的游戏;
    int argc = sizeof(argv) / sizeof(argv[0]);

    ::testing::internal::CaptureStdout();
    MainTest(argc, argv);
    std::string output = ::testing::internal::GetCapturedStdout();
    std::ifstream file("./game.txt");
    bool fileExists = file.good();
    file.close();

    EXPECT_TRUE(output.find("failed.") != std::string::npos);
    // EXPECT_FALSE(fileExists); //之前生成过了，这儿就不做判断了;
}

TEST(MainTest, GenerateGameBoarderTest2) {
    char* argv[] = { (char*)"./sudoku", (char*)"-n100 -r56~57 -u" }; //生成100局挖空在56~57之间的游戏;
    int argc = sizeof(argv) / sizeof(argv[0]);

    ::testing::internal::CaptureStdout();
    MainTest(argc, argv);
    std::string output = ::testing::internal::GetCapturedStdout();
    std::ifstream file("./game.txt");
    bool fileExists = file.good();
    file.close();

    EXPECT_TRUE(output.find("failed.") != std::string::npos);
    // EXPECT_FALSE(fileExists); //之前生成过了，这儿就不做判断了;
}

TEST(MainTest, generateAnswerBoarderTest) {
    char* argv[] = { (char*)"./sudoku", (char*)"-s ./none.txt" }; //解决不存在的游戏;
    int argc = sizeof(argv) / sizeof(argv[0]);

    ::testing::internal::CaptureStdout();
    MainTest(argc, argv);
    std::string output = ::testing::internal::GetCapturedStdout();
    std::ifstream file("./answer.txt");
    bool fileExists = file.good();
    file.close();

    EXPECT_TRUE(output.find("failed.") != std::string::npos);
    EXPECT_FALSE(fileExists);
}

//针对单个函数测试:
//测试generateFinal:
TEST(BoardTest, GenerateFinal) {
    int numGames = 10; // 生成10个最终解数独游戏

    generateFinal(numGames);

    // 检查生成的游戏文件是否存在
    std::ifstream file(filePathFinal);
    EXPECT_TRUE(file.good()); // 检查文件是否打开成功

    // 检查生成的游戏数量是否正确
    int count = 0;
    std::string line;
    while (std::getline(file, line)) {
        if (line.length() > 0) {
            count++;
        }
    }
    EXPECT_EQ(count, numGames); // 检查生成的游戏数量是否与期望值相等

    file.close();

    // 删除生成的游戏文件
    std::remove(filePathFinal.c_str());
}


//测试generateGame：
TEST(BoardTest, GenerateGame) {
    Board board;
    board.generateGame(40); // 生成一个有40个空格的数独游戏

    //进一步检查生成的游戏是否满足数独规则的要求
    std::string game = board.toString();
    EXPECT_EQ(game.length(), 198); // 检查生成的游戏字符串长度是否正确

    // 检查生成的游戏是否满足数独规则的要求
    for (int i = 0; i < Board::MaxRow; i++) {
        for (int j = 0; j < Board::MaxCol; j++) {
            int num = board.grid[i][j];
            if (num != 0) {
                // 检查同行是否有重复数字
                for (int col = 0; col < Board::MaxCol; col++) {
                    if (col != j && board.grid[i][col] == num) {
                        FAIL() << "Duplicate number " << num << " in the same row " << i;
                    }
                }

                // 检查同列是否有重复数字
                for (int row = 0; row < Board::MaxRow; row++) {
                    if (row != i && board.grid[row][j] == num) {
                        FAIL() << "Duplicate number " << num << " in the same column " << j;
                    }
                }

                // 检查同宫格是否有重复数字
                int startRow = i / 3 * 3;
                int startCol = j / 3 * 3;
                for (int row = startRow; row < startRow + 3; row++) {
                    for (int col = startCol; col < startCol + 3; col++) {
                        if (row != i && col != j && board.grid[row][col] == num) {
                            FAIL() << "Duplicate number " << num << " in the same block (" << startRow / 3 << ", " << startCol / 3 << ")";
                        }
                    }
                }
            }
        }
    }

    // 检查生成的游戏是否有空格
    int blankCount = 0;
    for (int i = 0; i < Board::MaxRow; i++) {
        for (int j = 0; j < Board::MaxCol; j++) {
            if (board.grid[i][j] == 0) {
                blankCount++;
            }
        }
    }
    EXPECT_EQ(blankCount, 40); // 检查生成的游戏中的空格数量是否正确
}
TEST(BoardTest, SolveDFS) {
    int input[9][9] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    Board board(input);
    int count = 0;
    board.solveDFS(0, 0, count);

    //检查解决数独的结果是否正确
    EXPECT_EQ(count, 1);  // 只能有一个解
    EXPECT_EQ(board.toString(), "534678912\n"
                                 "672195348\n"
                                 "198342567\n"
                                 "859761423\n"
                                 "426853791\n"
                                 "713924856\n"
                                 "961537284\n"
                                 "287419635\n"
                                 "345286179\n");
}

TEST(BoardTest, HasUniqueSolution) {
    // 创建一个数独游戏，该游戏有唯一解
    int array[9][9] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };
    Board board(array);

    // 检查数独游戏是否有唯一解
    bool hasUnique = board.hasUniqueSolution();

    EXPECT_TRUE(hasUnique);

    // 创建一个数独游戏，该游戏有多个解
    int array2[9][9] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 0, 0}
    };
    Board board2(array2);

    // 检查数独游戏是否有唯一解
    hasUnique = board2.hasUniqueSolution();
    EXPECT_FALSE(hasUnique);
}


// 执行所有测试用例
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
