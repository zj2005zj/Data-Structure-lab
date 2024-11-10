#include <iostream>
#include <cstring>  // 用于字符串操作
#include <vector>
using namespace std;
const int MAX_SIZE = 100;  // 定义串的最大长度
//定长顺序存储 
// 初始化串的函数
void initString(char* str, const char* initValue) {
    // 检查传入的字符串是否为空指针
    if (!str) {
        cerr << "Error: Invalid string pointer." << endl;
        return;
    }

    // 使用strncpy函数将initValue的内容复制到str中
    strncpy(str, initValue, MAX_SIZE - 1);

    // 确保字符串以null结尾
    str[MAX_SIZE - 1] = '\0';
}

// 清空串的函数
void clearString(char* str) {
    // 检查传入的字符串是否为空指针
    if (!str) {
        cerr << "Error: Invalid string pointer." << endl;
        return;
    }

    // 将字符串的第一个字符设置为null，从而清空字符串
    str[0] = '\0';
}

// 求串长度的函数
int getStringLength(const char* str) {
    // 检查传入的字符串是否为空指针
    if (!str) {
        cerr << "Error: Invalid string pointer." << endl;
        return -1;
    }
    int len = strlen(str);
    // 使用strlen函数获取字符串的长度
    return len;
}

// 构建next数组的函数
void buildNextArray(const char* pattern, int* next) {
    int patternLength = getStringLength(pattern);
    next[0] = -1;
    int i = 0, j = -1;

    while (i < patternLength) {
        if (j == -1 || pattern[i] == pattern[j]) {
            i++;
            j++;
            next[i] = j;
        } else {
            j = next[j];
        }
    }
}

// KMP算法模式匹配函数，返回所有匹配子串的初始位置
vector<int> kmpSearchAll(const char* text, const char* pattern) {
    int textLength = getStringLength(text);
    int patternLength = getStringLength(pattern);
    vector<int> positions;  // 用于存储所有匹配子串的初始位置

    if (patternLength == 0) return positions;  // 如果模式串为空，直接返回空向量

    int* next = new int[patternLength];
    buildNextArray(pattern, next);

    int i = 0, j = 0;
    while (i < textLength) {
        if (j == -1 || text[i] == pattern[j]) {
            i++;
            j++;
        } else {
            j = next[j];
        }

        if (j == patternLength) {
            positions.push_back(i - j);  // 找到一个匹配子串，记录其初始位置
            j = next[j];  // 继续查找下一个匹配子串
        }
    }

    delete[] next;  // 释放next数组的内存
    return positions;
}

// 返回某个起始位置的某长度的子串的函数
bool getSubstring(const char* str, int start, int length, char* subStr) {
    int strLength = getStringLength(str);

    // 检查参数的有效性
    if (start < 0 || start >= strLength || length <= 0 || start + length > strLength) {
        cerr << "Error: Invalid start position or length." << endl;
        subStr[0] = '\0';
        return false;
    }

    // 拷贝子串
    strncpy(subStr, str + start, length);
    subStr[length] = '\0';  // 确保子串以 null 结尾
    return true;
}

// 替换函数：Replace(S, T, V)
bool replaceAll(char* S, const char* T, const char* V) {
    int SLength = getStringLength(S);
    int TLength = getStringLength(T);
    int VLength = getStringLength(V);

    if (TLength == 0) {
        cerr << "Error: Pattern string T is empty." << endl;
        return false;
    }

    vector<int> positions = kmpSearchAll(S, T);
    if (positions.empty()) {
        cout << "Pattern '" << T << "' not found in the string '" << S << "'." << endl;
        return false;
    }

    // 计算新的字符串长度
    int newLength = SLength + positions.size() * (VLength - TLength);
    if (newLength >= MAX_SIZE) {
        cerr << "Error: Resulting string length exceeds maximum allowed size." << endl;
        return false;
    }

    char* newString = new char[newLength + 1];
    int newIndex = 0;
    int oldIndex = 0;

    for (size_t i = 0; i < positions.size(); ++i) {
        // 拷贝当前位置到下一个匹配子串之间的字符
        while (oldIndex < positions[i]) {
            newString[newIndex++] = S[oldIndex++];
        }

        // 拷贝替换字符串 V
        for (int j = 0; j < VLength; ++j) {
            newString[newIndex++] = V[j];
        }

        // 跳过匹配的子串 T
        oldIndex += TLength;
    }

    // 拷贝剩余的字符
    while (oldIndex < SLength) {
        newString[newIndex++] = S[oldIndex++];
    }

    newString[newIndex] = '\0';  // 确保新字符串以 null 结尾
    strncpy(S, newString, newLength + 1);
    delete[] newString;
    return true;
}

// 拼接函数: Concatenate(S1, S2)
bool concatenateStrings(char* S1, const char* S2) {
    int S1Length = getStringLength(S1);
    int S2Length = getStringLength(S2);

    if (S1Length + S2Length >= MAX_SIZE) {
        cerr << "Error: Resulting string length exceeds maximum allowed size." << endl;
        return false;
    }

    strncpy(S1 + S1Length, S2, MAX_SIZE - S1Length - 1);  // 将S2拼接到S1的末尾
    S1[S1Length + S2Length] = '\0';  // 确保新字符串以 null 结尾
    return true;
}

// 遍历函数: 依次输出串中所有字符
void traverseString(const char* str) {
    if (!str) {
        cerr << "Error: Invalid string pointer." << endl;
        return;
    }

    cout << "Traversing characters in the string: ";
    for (int i = 0; str[i] != '\0'; ++i) {
        cout << str[i] << endl;
        
    }
}

int main() {
    char myString[MAX_SIZE];  // 定义一个定长字符数组
    char string1[MAX_SIZE];  
    char string2[MAX_SIZE];  

    // 初始化字符串
    initString(string1, "Hello, ");
    initString(string2, "World!");

    // 初始化字符串
    initString(myString, "Hello, World!");
    
    // 遍历测试
    traverseString(myString);
    
    // 输出初始化后的字符串
    cout << "Initialized String: " << myString << endl; 

      // 输出字符串的长度
    int length = getStringLength(myString);
    cout << "String Length: " << length << endl;
    
    
    // 模式匹配测试
    const char* pattern = "World";
    vector<int> positions = kmpSearchAll(myString, pattern);
    if (!positions.empty()) {
        cout << "Pattern '" << pattern << "' found at positions: ";
        for (size_t i = 0; i < positions.size(); ++i) {
            cout << positions[i] << " ";
        }
        cout << endl;
    } else {
        cout << "Pattern '" << pattern << "' not found." << endl;
    }
    
    
      // 获取子串测试
    char subString[MAX_SIZE];
    if (getSubstring(myString, 7, 6, subString)) {
        cout << "Substring: " << subString << endl;
    }
    
    
    // 替换测试
    const char* oldSubstring = "World";
    const char* newSubstring = "Universe";
    if (replaceAll(myString, oldSubstring, newSubstring)) {
        cout << "String after replacement: " << myString << endl;
    }
    
     // 清空字符串
    clearString(myString);
    
    // 输出清空后的字符串
    cout << "Cleared String: " << myString << endl;
    
     // 输出清空后字符串的长度
    length = getStringLength(myString);
    cout << "Cleared String Length: " << length << endl;
    
    // 拼接测试
    if (concatenateStrings(string1, string2)) {
        cout << "Concatenated String: " << string1 << endl;
    }
    
    return 0;
}
