#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
using namespace std;
//�ѷ��� 
// ��ʼ�����ĺ���
char* initString(const char* initValue) {
    // ����ʼֵ�Ƿ�Ϊ��
    if (initValue == NULL) {
        cerr << "Error: Invalid initial value." << endl;
        return NULL;
    }

    // �����ʼֵ�ĳ���
    int length = strlen(initValue);

    // ��̬�����ڴ棬��СΪ length + 1��������ֹ�� '\0'��
    char* str = (char*)malloc((length + 1) * sizeof(char));
    if (str == NULL) {
        cerr << "Error: Memory allocation failed." << endl;
        return NULL;
    }

    // ����ʼֵ���Ƶ��·�����ڴ���
    strcpy(str, initValue);

    // �����·���Ķ��ڴ��ַ
    return str;
}
// ���ٴ��ĺ���
void destroyString(char* str) {
    // ���ָ���Ƿ�Ϊ��
    if (str != NULL) {
        // �ͷ��ڴ�
        free(str);
    }
}

// ��մ��ĺ���
void clearString(char* str) {
    // ���ָ���Ƿ�Ϊ��
    if (str != NULL) {
        // ���ַ����������Ϊ�մ�
        str[0] = '\0';
    }
}

// �󴮳��ȵĺ���
int getStringLength(const char* str) {
    // ���ָ���Ƿ�Ϊ��
    if (str == NULL) {
        cerr << "Error: Invalid string pointer." << endl;
        return -1;
    }

    // �����ַ����ĳ���
    return strlen(str);
}
// ��������ƥ���ǰ׺��
void buildPrefixTable(const char* pattern, int* prefixTable, int patternLength) {
    int len = 0;  // ��ǰ�ǰ׺�ĳ���
    prefixTable[0] = 0;  // ��һ���ַ���ǰ׺����Ϊ0

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

// KMP�㷨ʵ���Ӵ�ƥ��
std::vector<int> kmpSearch(const char* text, const char* pattern) {
    int textLength = getStringLength(text);
    int patternLength = getStringLength(pattern);

    if (textLength == -1 || patternLength == -1) {
        return {};
    }

    std::vector<int> positions;
    int* prefixTable = new int[patternLength];
    buildPrefixTable(pattern, prefixTable, patternLength);

    int i = 0;  // text������
    int j = 0;  // pattern������

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
// ���Ӵ��ĺ���
char* getSubstring(const char* str, int start, int length) {
    int strLength = getStringLength(str);
    if (start < 0 || start >= strLength || length <= 0 || start + length > strLength) {
        cerr << "Error: Invalid start position or length." << endl;
        return NULL;
    }

    // �����ڴ��Դ洢�Ӵ�
    char* substring = (char*)std::malloc((length + 1) * sizeof(char));
    if (substring == NULL) {
        cerr << "Error: Memory allocation failed." << endl;
        return NULL;
    }

    // �����Ӵ�
    strncpy(substring, str + start, length);
    substring[length] = '\0';  // �����ֹ��

    return substring;
}

// �滻����
char* Replace(char* S, const char* T, const char* V) {
    int SLength = getStringLength(S);
    int TLength = getStringLength(T);
    int VLength = getStringLength(V);

    if (SLength == -1 || TLength == -1 || VLength == -1) {
        return NULL;
    }

    // ʹ��KMP�㷨�ҵ�����ƥ���λ��
    vector<int> positions = kmpSearch(S, T);

    // �����滻����ַ�������
    int newLength = SLength + positions.size() * (VLength - TLength);

    // �����µ��ڴ��Դ洢�滻����ַ���
    char* newStr = (char*)malloc((newLength + 1) * sizeof(char));
    if (newStr == NULL) {
        cerr << "Error: Memory allocation failed." << endl;
        return NULL;
    }

    int i = 0;  // S������
    int j = 0;  // newStr������

    // ʹ��һ��� for ѭ��
    for (size_t k = 0; k < positions.size(); k++) {
        int pos = positions[k];

        // ����ƥ��λ��֮ǰ���ַ�
        strncpy(newStr + j, S + i, pos - i);
        j += pos - i;
        i = pos;

        // �����滻�ַ���
        strncpy(newStr + j, V, VLength);
        j += VLength;
        i += TLength;
    }

    // ����ʣ����ַ�
    strcpy(newStr + j, S + i);

    return newStr;
}

// ƴ���������ĺ���
char* concatStrings(const char* str1, const char* str2) {
    int len1 = getStringLength(str1);
    int len2 = getStringLength(str2);

    if (len1 == -1 || len2 == -1) {
        return NULL;
    }

    // �����ڴ��Դ洢ƴ�Ӻ���ַ���
    char* result = (char*)std::malloc((len1 + len2 + 1) * sizeof(char));
    if (result == NULL) {
        std::cerr << "Error: Memory allocation failed." << std::endl;
        return NULL;
    }

    // ���Ƶ�һ���ַ���
    std::strcpy(result, str1);

    // ���Ƶڶ����ַ���
    std::strcpy(result + len1, str2);

    return result;
}
// ��������: ����������������ַ�
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
    // ����һ���ѷ�����ַ���ָ��
    char* myString = NULL;

    // ��ʼ���ַ���
    myString = initString("Hello, World!");

    // ����ʼ���Ƿ�ɹ�
    if (myString != NULL) 
        cout << "Initialized string: " << myString << endl;
    // ��������
    traverseString(myString);
    
    // ���ַ����ĳ���
    int length = getStringLength(myString);
    if (length != -1) {
        cout << "String length: " << length << endl;
    }
        
    // ����ַ���
    clearString(myString);
    cout << "Cleared string: " << myString << endl;
    
     // �ٴ�����պ��ַ����ĳ���
    int clearedLength = getStringLength(myString);
    if (clearedLength != -1) {
        cout << "Cleared string length: " << clearedLength << endl;
    }
    
    // ʹ����Ϻ������ַ���
    destroyString(myString);
    myString = NULL;  // ��ָ����Ϊ�գ���������ָ��
    
   
   // ����һ���ѷ�����ַ���ָ��
    char* text = NULL;
    char* pattern = NULL;

    // ��ʼ���ַ���
    text = initString("Hello,World!World");
    pattern = initString("World");

   
    // ʹ��KMP�㷨�����Ӵ�ƥ��
    vector<int> positions = kmpSearch(text, pattern);

    // ���ƥ��λ��
    for (size_t i = 0; i < positions.size(); i++) {
    	positions[i]++;
        cout << positions[i] << " ";
    }
    cout << std::endl;

     
      // ���Ӵ�ʾ��
    char* substring = getSubstring(text, 4, 6);
    if (substring != NULL) {
        cout << "Substring: " << substring << endl;
        destroyString(substring);  // �ͷ��Ӵ��ڴ�
    }
    // �滻ʾ��
    char* newText = Replace(text, "World", "XYZ");
    if (newText != NULL) {
        cout << "New text: " << newText << endl;
        destroyString(newText);  // �ͷ��滻����ַ����ڴ�
    }
    
    
    
    // ƴ��ʾ��
    char* str1 = initString("Hello, ");
    char* str2 = initString("world!");
    char* concatenatedStr = concatStrings(str1, str2);
    if (concatenatedStr != NULL) {
        std::cout << "Concatenated string: " << concatenatedStr << std::endl;
        destroyString(concatenatedStr);  // �ͷ�ƴ�Ӻ���ַ����ڴ�
    }
    
    // ʹ����Ϻ������ַ���
    destroyString(text);
    destroyString(pattern);
    text = NULL;  // ��ָ����Ϊ�գ���������ָ��
    pattern = NULL;
   
    return 0;

}
