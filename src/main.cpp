#include <unistd.h>
#include "sudoku.h"

using namespace std;

void printSuccessMessage(const string& operation) {
    printf("%s success.\n", operation.c_str());
}

void printErrorMessage(const string& message) {
    printf("%s\n", message.c_str());
    exit(-1);
}

enum class Mode {
    GenerateFinal,
    SolveSudoku,
    GenerateGame,
};

struct Options {
    Mode mode;
    int count;
    int difficulty;
    int blankMin;
    int blankMax;
    string filename;
    bool uniqueSolution;

    Options()
        : mode(Mode::GenerateFinal), count(0), difficulty(0), blankMin(0), blankMax(0), uniqueSolution(false) {}
};

Options parseOptions(int argc, char* argv[]) {
    Options options;
    int opt;
    string arg;

    while ((opt = getopt(argc, argv, "c:s:n:m:r:u")) != -1) {
        switch (opt) {
            case 'c':
                options.mode = Mode::GenerateFinal;
                options.count = stoi(optarg);
                if (options.count < 1 || options.count > 1000000) {
                    printErrorMessage("Parameters should range from 1 to 1000000");
                }
                break;
            case 's':
                options.mode = Mode::SolveSudoku;
                options.filename = optarg;
                break;
            case 'n':
                options.mode = Mode::GenerateGame;
                options.count = stoi(optarg);
                if (options.count < 1 || options.count > 10000) {
                    printErrorMessage("Parameters should range from 1 to 10000");
                }
                break;
            case 'm':
                options.difficulty = stoi(optarg);
                if (options.difficulty < 1 || options.difficulty > 3) {
                    printErrorMessage("Parameters should range from 1 to 3");
                }
                break;
            case 'r': {
                arg = optarg;
                auto pos = arg.find('~');
                options.blankMin = stoi(arg.substr(0, pos));
                options.blankMax = stoi(arg.substr(pos + 1));
                if (options.blankMin < 20 || options.blankMax > 55 || options.blankMin > options.blankMax) {
                    printErrorMessage("Parameters should range from 20 to 55");
                }
                break;
            }
            case 'u':
                options.uniqueSolution = true;
                break;
            default:
                printErrorMessage("The command is wrong!");
        }
    }

    if ((options.mode == Mode::GenerateGame) && !options.count) {
        printErrorMessage("Missing required parameter: count");
    }

    if ((options.mode == Mode::SolveSudoku) && !options.filename.length()) {
        printErrorMessage("Missing required parameter: filename");
    }

    return options;
}

int main(int argc, char* argv[]) {
    Options options = parseOptions(argc, argv);

    switch (options.mode) {
        case Mode::GenerateFinal:
            generateFinal(options.count);
            printSuccessMessage("generate_final");
            break;
        case Mode::SolveSudoku:
            generateAnswer(options.filename);
            printSuccessMessage("generate_answer");
            break;
        case Mode::GenerateGame:
            generateGame(options.count, options.uniqueSolution, options.difficulty, options.blankMin, options.blankMax);
            printSuccessMessage("generate_game");
            break;
    }

    return 0;
}
