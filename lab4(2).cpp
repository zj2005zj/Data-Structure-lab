#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
using namespace std;
//堆分配 
// 初始化串的函数
char* initString(const char* initValue) {
    // 检查初始值是否为空
    if (initValue == NULL) {
        cerr << "Error: Invalid initial value." << endl;
        return NULL;
    }

    // 计算初始值的长度
    int length = strlen(initValue);

    // 动态分配内存，大小为 length + 1（包含终止符 '\0'）
    char* str = (char*)malloc((length + 1) * sizeof(char));
    if (str == NULL) {
        cerr << "Error: Memory allocation failed." << endl;
        return NULL;
    }

    // 将初始值复制到新分配的内存中
    strcpy(str, initValue);

    // 返回新分配的堆内存地址
    return str;
}
// 销毁串的函数
void destroyString(char* str) {
    // 检查指针是否为空
    if (str != NULL) {
        // 释放内存
        free(str);
    }
}

// 清空串的函数
void clearString(char* str) {
    // 检查指针是否为空
    if (str != NULL) {
        // 将字符串内容清空为空串
        str[0] = '\0';
    }
}

// 求串长度的函数
int getStringLength(const char* str) {
    // 检查指针是否为空
    if (str == NULL) {
        cerr << "Error: Invalid string pointer." << endl;
        return -1;
    }

    // 计算字符串的长度
    return strlen(str);
}
// 构建部分匹配表（前缀表）
void buildPrefixTable(const char* pattern, int* prefixTable, int patternLength) {
    int len = 0;  // 当前最长前缀的长度
    prefixTable[0] = 0;  // 第一个字符的前缀长度为0

    int i = 1;
    while (i < patternLength) {
        if (pattern[i] == pattern[len]) {
            len++;
            prefixTable[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = prefixTable[len - 1];
            } else {
                prefixTable[i] = 0;
                i++;
            }
        }
    }
}

// KMP算法实现子串匹配
std::vector<int> kmpSearch(const char* text, const char* pattern) {
    int textLength = getStringLength(text);
    int patternLength = getStringLength(pattern);

    if (textLength == -1 || patternLength == -1) {
        return {};
    }

    std::vector<int> positions;
    int* prefixTable = new int[patternLength];
    buildPrefixTable(pattern, prefixTable, patternLength);

    int i = 0;  // text的索引
    int j = 0;  // pattern的索引

    while (i < textLength) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }

        if (j == patternLength) {
            positions.push_back(i - j);
            j = prefixTable[j - 1];
        } else if (i < textLength && pattern[j] != text[i]) {
            if (j != 0) {
                j = prefixTable[j - 1];
            } else {
                i++;
            }
        }
    }

    delete[] prefixTable;
    return positions;
}
// 求子串的函数
char* getSubstring(const char* str, int start, int length) {
    int strLength = getStringLength(str);
    if (start < 0 || start >= strLength || length <= 0 || start + length > strLength) {
        cerr << "Error: Invalid start position or length." << endl;
        return NULL;
    }

    // 分配内存以存储子串
    char* substring = (char*)std::malloc((length + 1) * sizeof(char));
    if (substring == NULL) {
        cerr << "Error: Memory allocation failed." << endl;
        return NULL;
    }

    // 复制子串
    strncpy(substring, str + start, length);
    substring[length] = '\0';  // 添加终止符

    return substring;
}

// 替换函数
char* Replace(char* S, const char* T, const char* V) {
    int SLength = getStringLength(S);
    int TLength = getStringLength(T);
    int VLength = getStringLength(V);

    if (SLength == -1 || TLength == -1 || VLength == -1) {
        return NULL;
    }

    // 使用KMP算法找到所有匹配的位置
    vector<int> positions = kmpSearch(S, T);

    // 计算替换后的字符串长度
    int newLength = SLength + positions.size() * (VLength - TLength);

    // 分配新的内存以存储替换后的字符串
    char* newStr = (char*)malloc((newLength + 1) * sizeof(char));
    if (newStr == NULL) {
        cerr << "Error: Memory allocation failed." << endl;
        return NULL;
    }

    int i = 0;  // S的索引
    int j = 0;  // newStr的索引

    // 使用一般的 for 循环
    for (size_t k = 0; k < positions.size(); k++) {
        int pos = positions[k];

        // 复制匹配位置之前的字符
        strncpy(newStr + j, S + i, pos - i);
        j += pos - i;
        i = pos;

        // 复制替换字符串
        strncpy(newStr + j, V, VLength);
        j += VLength;
        i += TLength;
    }

    // 复制剩余的字符
    strcpy(newStr + j, S + i);

    return newStr;
}

// 拼接两个串的函数
char* concatStrings(const char* str1, const char* str2) {
    int len1 = getStringLength(str1);
    int len2 = getStringLength(str2);

    if (len1 == -1 || len2 == -1) {
        return NULL;
    }

    // 分配内存以存储拼接后的字符串
    char* result = (char*)std::malloc((len1 + len2 + 1) * sizeof(char));
    if (result == NULL) {
        std::cerr << "Error: Memory allocation failed." << std::endl;
        return NULL;
    }

    // 复制第一个字符串
    std::strcpy(result, str1);

    // 复制第二个字符串
    std::strcpy(result + len1, str2);

    return result;
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
    // 定义一个堆分配的字符串指针
    char* myString = NULL;

    // 初始化字符串
    myString = initString("Hello, World!");

    // 检查初始化是否成功
    if (myString != NULL) 
        cout << "Initialized string: " << myString << endl;
    // 遍历测试
    traverseString(myString);
    
    // 求字符串的长度
    int length = getStringLength(myString);
    if (length != -1) {
        cout << "String length: " << length << endl;
    }
        
    // 清空字符串
    clearString(myString);
    cout << "Cleared string: " << myString << endl;
    
     // 再次求清空后字符串的长度
    int clearedLength = getStringLength(myString);
    if (clearedLength != -1) {
        cout << "Cleared string length: " << clearedLength << endl;
    }
    
    // 使用完毕后销毁字符串
    destroyString(myString);
    myString = NULL;  // 将指针设为空，避免悬空指针
    
   
   // 定义一个堆分配的字符串指针
    char* text = NULL;
    char* pattern = NULL;

    // 初始化字符串
    text = initString("Hello,World!World");
    pattern = initString("World");

   
    // 使用KMP算法进行子串匹配
    vector<int> positions = kmpSearch(text, pattern);

    // 输出匹配位置
    for (size_t i = 0; i < positions.size(); i++) {
    	positions[i]++;
        cout << positions[i] << " ";
    }
    cout << std::endl;

     
      // 求子串示例
    char* substring = getSubstring(text, 4, 6);
    if (substring != NULL) {
        cout << "Substring: " << substring << endl;
        destroyString(substring);  // 释放子串内存
    }
    // 替换示例
    char* newText = Replace(text, "World", "XYZ");
    if (newText != NULL) {
        cout << "New text: " << newText << endl;
        destroyString(newText);  // 释放替换后的字符串内存
    }
    
    
    
    // 拼接示例
    char* str1 = initString("Hello, ");
    char* str2 = initString("world!");
    char* concatenatedStr = concatStrings(str1, str2);
    if (concatenatedStr != NULL) {
        std::cout << "Concatenated string: " << concatenatedStr << std::endl;
        destroyString(concatenatedStr);  // 释放拼接后的字符串内存
    }
    
    // 使用完毕后销毁字符串
    destroyString(text);
    destroyString(pattern);
    text = NULL;  // 将指针设为空，避免悬空指针
    pattern = NULL;
   
    return 0;

}
