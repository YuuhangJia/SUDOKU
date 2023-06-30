#include "board.h"
#include <cstdlib>
#include <ctime>

std::unordered_map<int,int> mmap;

Board::Board(const std::string &str)
{
    int pos = 0;
    for (auto &row : grid)
    {
        for (int &item : row)
        {
            item = str[pos++] - '0';
        }
        pos++;
    }
}

Board::Board(const int array[MaxRow][MaxCol])
{
    memcpy(grid, array, MaxRow * MaxCol * sizeof(int));
}

int deal(int x)
{
    int e = x % 3;
    return x - e + 1;
}

void Board::myInit()
{
    int tmp = 0;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            tmp = 1 << grid[i][j];
            rowR[i] += tmp;
            colR[j] += tmp;
            pointR[deal(i)][deal(j)] += tmp;
        }
    }
    for (int i = 0; i < 20; i++)
        mmap[1 << i] = i;
}

void Board::solve()
{
    myInit();
    int count = 0;
    // solveDFS(0, 0, count);
    solveImproved(0, 0, count);
}

void Board::generateGame(int blankNum)
{
    int base[9] = {0, 3, 6, 27, 30, 33, 54, 57, 60};
    int offset[9] = {0, 1, 2, 9, 10, 11, 18, 19, 20};

    // 挖空九宫格的格子
    for (int k : base)
    {
        int num = blankNum / 9;
        while (num--)
        {
            int dot;
            do
            {
                dot = k + offset[rand() % 9];
            } while (grid[dot / 9][dot % 9] == 0);
            grid[dot / 9][dot % 9] = 0;
        }
    }

    // 随机挖空剩余的格子
    int num = blankNum % 9;
    while (num--)
    {
        int dot;
        do
        {
            dot = rand() % 81;
        } while (grid[dot / 9][dot % 9] == 0);
        grid[dot / 9][dot % 9] = 0;
    }
}

bool Board::hasUniqueSolution()
{

    myInit();

    int count = 0;
    int temp[MaxRow][MaxCol]{};
    memcpy(temp, grid, MaxRow * MaxCol * sizeof(int));
    // solveDFS(0, 0, count);
    solveImproved(0, 0, count);
    memcpy(grid, temp, MaxRow * MaxCol * sizeof(int));
    return count == 1;
}

std::string Board::toString() const
{
    std::string str;
    for (const auto &row : grid)
    {
        for (const auto item : row)
        {
            if (item == 0)
            {
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

bool Board::isValid(int row, int col, int num) const
{
    for (int k = 0; k < 9; k++)
    {
        if (grid[row][k] == num || grid[k][col] == num || grid[(row / 3) * 3 + k / 3][(col / 3) * 3 + k % 3] == num)
        {
            return false;
        }
    }
    return true;
}

void Board::solveDFS(int row, int col, int &count)
{
    if (count >= 2)
    {
        return;
    }

    if (row == MaxRow)
    {
        count++;
        return;
    }

    if (col == MaxCol)
    {
        solveDFS(row + 1, 0, count);
        return;
    }

    if (grid[row][col] != 0)
    {
        solveDFS(row, col + 1, count);
        return;
    }

    for (int num = 1; num <= 9; num++)
    {
        if (isValid(row, col, num))
        {
            grid[row][col] = num;
            int temp = count;
            solveDFS(row, col + 1, count);
            if (count >= 2)
            {
                return;
            }
            else if (count == temp + 1)
            {
                continue;
            }
        }
    }
}

void Board::solveImproved(int row, int col, int &count)
{
    if (count >= 2)
    {
        return;
    }

    if (row == MaxRow)
    {
        count++;
        return;
    }

    if (col == MaxCol)
    {
        solveImproved(row + 1, 0, count);
        return;
    }

    if (grid[row][col] != 0)
    {
        solveImproved(row, col + 1, count);
        return;
    }

    int b = (rowR[row] | colR[col] | pointR[row][col]);
    b = b ^ ((1 << 10) - 1); // 可用的数字
    if ((b & (-b)) == 1)
        b--;
    while (b > 0)
    {
        int m = b & (-b);
        rowR[row] += m;
        colR[col] += m;
        pointR[deal(row)][deal(col)] += m;
        grid[row][col] = mmap[m];
        int temp = count;

        solveImproved(row, col + 1, count);

        rowR[row] -= m;
        colR[col] -= m;
        pointR[deal(row)][deal(col)] -= m;
        b-=m;

        if (count >= 2)
        {
            return;
        }
        else if (count == temp + 1)
        {
            continue;
        }
    }
}