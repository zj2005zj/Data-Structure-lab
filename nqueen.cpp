#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

vector<int> solution; // 保存每列皇后的位置
int len; // 皇后数量
int maxRestarts = 20; // 最大重启次数

void initSolution(); // 初始化解
int evaluate(); // 计算当前解的评价值（冲突数）
void performHillClimbing(); // 爬山算法
int evaluateDelta(int col, int newRow); // 增量评估函数

int main() {
    srand(time(0)); // 初始化随机种子
    cout << "请输入皇后个数: ";
    cin >> len;

    solution.resize(len);
    int bestEnergy = len * len; // 初始化最佳冲突数为一个较大的值
    vector<int> bestSolution = solution; // 初始化最佳解

    for (int restart = 0; restart < maxRestarts; ++restart) {
        initSolution(); // 初始化解
        performHillClimbing(); // 执行爬山算法

        int currentEnergy = evaluate(); // 当前冲突数
        if (currentEnergy < bestEnergy) {
            bestEnergy = currentEnergy;
            bestSolution = solution;
        }

        if (bestEnergy == 0) break; // 找到最优解，提前退出
    }

    solution = bestSolution; // 更新为最佳解

    cout << "得到的解为: ";
    for (int i = 0; i < len; ++i) {
        cout << solution[i] << " "; // 输出每列皇后所在的行
    }
    cout << endl;

    return 0;
}

/*** 初始化一个随机解 ***/
void initSolution() {
    for (int i = 0; i < len; ++i) {
        solution[i] = i; // 初始化每列的位置
    }
    // 随机打乱行的位置
    for (int i = 0; i < len; ++i) {
        swap(solution[i], solution[rand() % len]);
    }
}

/***
 *  计算当前解的评价函数（棋盘的价值: value=可相互攻击到的皇后对数）
 *  在同一行或同一列的皇后不会冲突，因此只需考虑对角线冲突。
 ***/
int evaluate() {
    int value = 0; // 初始化冲突数
    vector<int> cols(len, 0); // 列冲突数
    vector<int> diag1(2 * len - 1, 0); // 主对角线冲突数
    vector<int> diag2(2 * len - 1, 0); // 副对角线冲突数

    for (int i = 0; i < len; i++) {
        int row = solution[i];
        cols[row]++;
        diag1[row + i]++;
        diag2[row - i + (len - 1)]++;
    }

    for (int i = 0; i < len; i++) {
        if (cols[i] > 1) {
            value += cols[i] - 1; // 列冲突
        }
    }

    for (int i = 0; i < 2 * len - 1; i++) {
        if (diag1[i] > 1) {
            value += diag1[i] - 1; // 主对角线冲突
        }
        if (diag2[i] > 1) {
            value += diag2[i] - 1; // 副对角线冲突
        }
    }

    return value; // 返回冲突数
}

/***
 *  增量评估函数，计算移动某一列的皇后到新行后冲突数的变化
 ***/
int evaluateDelta(int col, int newRow) {
    int oldRow = solution[col];
    int delta = 0;
    int oldDiag1 = oldRow + col;                //主对角线
    int oldDiag2 = oldRow - col + (len - 1);//副对角线
    int newDiag1 = newRow + col;
    int newDiag2 = newRow - col + (len - 1);

    // 计算旧冲突
    for (int i = 0; i < len; ++i) {
        if (i != col && solution[i] == oldRow) delta--; // 旧行冲突减少
        if (i != col && solution[i] + i == oldDiag1) delta--; // 旧主对角线冲突减少
        if (i != col && solution[i] - i + (len - 1) == oldDiag2) delta--; // 旧副对角线冲突减少
    }

    // 计算新冲突
    for (int i = 0; i < len; ++i) {
        if (i != col && solution[i] == newRow) delta++; // 新行冲突增加
        if (i != col && solution[i] + i == newDiag1) delta++; // 新主对角线冲突增加
        if (i != col && solution[i] - i + (len - 1) == newDiag2) delta++; // 新副对角线冲突增加
    }

    return delta;
}
/***
 *  爬山算法
 ***/
void performHillClimbing() {
    int currentEnergy = evaluate(); // 当前冲突数
    bool found = true;

    while (currentEnergy > 0 && found) {
        found = false;

        // 尝试在所有列中移动每个皇后
        for (int col = 0; col < len; ++col) {
            int originalRow = solution[col];

            // 尝试将当前列皇后移动到不同的行
            for (int row = 0; row < len; ++row) {
                if (row != originalRow) {
                    // 计算新位置的增量评估
                    int delta = evaluateDelta(col, row);
                    if (delta < 0) {
                        solution[col] = row; // 移动皇后到新行
                        currentEnergy += delta; // 更新当前冲突数
                        found = true; // 找到更优解
                        break; // 不再继续尝试其他行
                    }
                }
            }
        }
    }
}


