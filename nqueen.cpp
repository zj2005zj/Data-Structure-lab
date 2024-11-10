#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

vector<int> solution; // ����ÿ�лʺ��λ��
int len; // �ʺ�����
int maxRestarts = 20; // �����������

void initSolution(); // ��ʼ����
int evaluate(); // ���㵱ǰ�������ֵ����ͻ����
void performHillClimbing(); // ��ɽ�㷨
int evaluateDelta(int col, int newRow); // ������������

int main() {
    srand(time(0)); // ��ʼ���������
    cout << "������ʺ����: ";
    cin >> len;

    solution.resize(len);
    int bestEnergy = len * len; // ��ʼ����ѳ�ͻ��Ϊһ���ϴ��ֵ
    vector<int> bestSolution = solution; // ��ʼ����ѽ�

    for (int restart = 0; restart < maxRestarts; ++restart) {
        initSolution(); // ��ʼ����
        performHillClimbing(); // ִ����ɽ�㷨

        int currentEnergy = evaluate(); // ��ǰ��ͻ��
        if (currentEnergy < bestEnergy) {
            bestEnergy = currentEnergy;
            bestSolution = solution;
        }

        if (bestEnergy == 0) break; // �ҵ����Ž⣬��ǰ�˳�
    }

    solution = bestSolution; // ����Ϊ��ѽ�

    cout << "�õ��Ľ�Ϊ: ";
    for (int i = 0; i < len; ++i) {
        cout << solution[i] << " "; // ���ÿ�лʺ����ڵ���
    }
    cout << endl;

    return 0;
}

/*** ��ʼ��һ������� ***/
void initSolution() {
    for (int i = 0; i < len; ++i) {
        solution[i] = i; // ��ʼ��ÿ�е�λ��
    }
    // ��������е�λ��
    for (int i = 0; i < len; ++i) {
        swap(solution[i], solution[rand() % len]);
    }
}

/***
 *  ���㵱ǰ������ۺ��������̵ļ�ֵ: value=���໥�������Ļʺ������
 *  ��ͬһ�л�ͬһ�еĻʺ󲻻��ͻ�����ֻ�迼�ǶԽ��߳�ͻ��
 ***/
int evaluate() {
    int value = 0; // ��ʼ����ͻ��
    vector<int> cols(len, 0); // �г�ͻ��
    vector<int> diag1(2 * len - 1, 0); // ���Խ��߳�ͻ��
    vector<int> diag2(2 * len - 1, 0); // ���Խ��߳�ͻ��

    for (int i = 0; i < len; i++) {
        int row = solution[i];
        cols[row]++;
        diag1[row + i]++;
        diag2[row - i + (len - 1)]++;
    }

    for (int i = 0; i < len; i++) {
        if (cols[i] > 1) {
            value += cols[i] - 1; // �г�ͻ
        }
    }

    for (int i = 0; i < 2 * len - 1; i++) {
        if (diag1[i] > 1) {
            value += diag1[i] - 1; // ���Խ��߳�ͻ
        }
        if (diag2[i] > 1) {
            value += diag2[i] - 1; // ���Խ��߳�ͻ
        }
    }

    return value; // ���س�ͻ��
}

/***
 *  �������������������ƶ�ĳһ�еĻʺ����к��ͻ���ı仯
 ***/
int evaluateDelta(int col, int newRow) {
    int oldRow = solution[col];
    int delta = 0;
    int oldDiag1 = oldRow + col;                //���Խ���
    int oldDiag2 = oldRow - col + (len - 1);//���Խ���
    int newDiag1 = newRow + col;
    int newDiag2 = newRow - col + (len - 1);

    // ����ɳ�ͻ
    for (int i = 0; i < len; ++i) {
        if (i != col && solution[i] == oldRow) delta--; // ���г�ͻ����
        if (i != col && solution[i] + i == oldDiag1) delta--; // �����Խ��߳�ͻ����
        if (i != col && solution[i] - i + (len - 1) == oldDiag2) delta--; // �ɸ��Խ��߳�ͻ����
    }

    // �����³�ͻ
    for (int i = 0; i < len; ++i) {
        if (i != col && solution[i] == newRow) delta++; // ���г�ͻ����
        if (i != col && solution[i] + i == newDiag1) delta++; // �����Խ��߳�ͻ����
        if (i != col && solution[i] - i + (len - 1) == newDiag2) delta++; // �¸��Խ��߳�ͻ����
    }

    return delta;
}
/***
 *  ��ɽ�㷨
 ***/
void performHillClimbing() {
    int currentEnergy = evaluate(); // ��ǰ��ͻ��
    bool found = true;

    while (currentEnergy > 0 && found) {
        found = false;

        // ���������������ƶ�ÿ���ʺ�
        for (int col = 0; col < len; ++col) {
            int originalRow = solution[col];

            // ���Խ���ǰ�лʺ��ƶ�����ͬ����
            for (int row = 0; row < len; ++row) {
                if (row != originalRow) {
                    // ������λ�õ���������
                    int delta = evaluateDelta(col, row);
                    if (delta < 0) {
                        solution[col] = row; // �ƶ��ʺ�����
                        currentEnergy += delta; // ���µ�ǰ��ͻ��
                        found = true; // �ҵ����Ž�
                        break; // ���ټ�������������
                    }
                }
            }
        }
    }
}


