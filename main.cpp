#include <unistd.h>  // 导入unistd.h头文件
#include "sudoku.h"  // 导入自定义的sudoku.h头文件

using namespace std;

void printSuccessMessage(const string& operation) {
    printf("%s success.\n", operation.c_str());  // 打印成功消息
}

void printErrorMessage(const string& message) {
    printf("%s\n", message.c_str());  // 打印错误消息
    exit(-1);  // 退出程序
}

enum class Mode {
    GenerateFinal,  // 生成数独最终结果
    SolveSudoku,  // 解决数独
    GenerateGame,  // 生成数独游戏
};

struct Options {
    Mode mode;  // 模式选项
    int count;  // 数量选项
    int difficulty;  // 难度选项
    int blankMin;  // 空白格最小数量选项
    int blankMax;  // 空白格最大数量选项
    string filename;  // 文件名选项
    bool uniqueSolution;  // 唯一解选项

    Options()
        : mode(Mode::GenerateFinal), count(0), difficulty(0), blankMin(0), blankMax(0), uniqueSolution(false) {}
};

Options parseOptions(int argc, char* argv[]) {
    Options options;
    int opt;
    string arg;

    // 解析命令行参数
    while ((opt = getopt(argc, argv, "c:s:n:m:r:u")) != -1) {
        switch (opt) {
            case 'c':
                options.mode = Mode::GenerateFinal;
                options.count = stoi(optarg);  // 解析数独最终结果生成的数量选项
                if (options.count < 1 || options.count > 1000000) {
                    printErrorMessage("Parameters should range from 1 to 1000000");  // 数量选项错误提示
                }
                break;
            case 's':
                options.mode = Mode::SolveSudoku;
                options.filename = optarg;  // 解析要解决的数独文件名选项
                break;
            case 'n':
                options.mode = Mode::GenerateGame;
                options.count = stoi(optarg);  // 解析数独游戏生成的数量选项
                if (options.count < 1 || options.count > 10000) {
                    printErrorMessage("Parameters should range from 1 to 10000");  // 数量选项错误提示
                }
                break;
            case 'm':
                options.difficulty = stoi(optarg);  // 解析数独游戏的难度选项
                if (options.difficulty < 1 || options.difficulty > 3) {
                    printErrorMessage("Parameters should range from 1 to 3");  // 难度选项错误提示
                }
                break;
            case 'r': {
                arg = optarg;
                auto pos = arg.find('~');
                options.blankMin = stoi(arg.substr(0, pos));  // 解析空白格最小数量选项
                options.blankMax = stoi(arg.substr(pos + 1));  // 解析空白格最大数量选项
                if (options.blankMin < 20 || options.blankMax > 55 || options.blankMin > options.blankMax) {
                    printErrorMessage("Parameters should range from 20 to 55");  // 空白格数量选项错误提示
                }
                break;
            }
            case 'u':
                options.uniqueSolution = true;  // 解析唯一解选项
                break;
            default:
                printErrorMessage("The command is wrong!");  // 命令错误提示
        }
    }

    // 验证参数的完整性
    if ((options.mode == Mode::GenerateGame) && !options.count) {
        printErrorMessage("Missing required parameter: count");  // 缺少数量选项错误提示
    }

    if ((options.mode == Mode::SolveSudoku) && !options.filename.length()) {
        printErrorMessage("Missing required parameter: filename");  // 缺少文件名选项错误提示
    }

    return options;  // 返回解析后的选项
}

int main(int argc, char* argv[]) {
    Options options = parseOptions(argc, argv);  // 解析命令行选项

    // 根据选项执行不同的操作
    switch (options.mode) {
        case Mode::GenerateFinal:
            generateFinal(options.count);  // 生成数独最终结果
            printSuccessMessage("generate_final"); 
            break;
        case Mode::SolveSudoku:
            generateAnswer(options.filename);  // 解决数独
            printSuccessMessage("generate_answer");  
            break;
        case Mode::GenerateGame:
            generateGame(options.count, options.uniqueSolution, options.difficulty, options.blankMin, options.blankMax);
            // 生成数独游戏
            printSuccessMessage("generate_game"); 
            break;
    }

    return 0;
}
