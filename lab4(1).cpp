#include <iostream>
#include <cstring>  // �����ַ�������
#include <vector>
using namespace std;
const int MAX_SIZE = 100;  // ���崮����󳤶�
//����˳��洢 
// ��ʼ�����ĺ���
void initString(char* str, const char* initValue) {
    // ��鴫����ַ����Ƿ�Ϊ��ָ��
    if (!str) {
        cerr << "Error: Invalid string pointer." << endl;
        return;
    }

    // ʹ��strncpy������initValue�����ݸ��Ƶ�str��
    strncpy(str, initValue, MAX_SIZE - 1);

    // ȷ���ַ�����null��β
    str[MAX_SIZE - 1] = '\0';
}

// ��մ��ĺ���
void clearString(char* str) {
    // ��鴫����ַ����Ƿ�Ϊ��ָ��
    if (!str) {
        cerr << "Error: Invalid string pointer." << endl;
        return;
    }

    // ���ַ����ĵ�һ���ַ�����Ϊnull���Ӷ�����ַ���
    str[0] = '\0';
}

// �󴮳��ȵĺ���
int getStringLength(const char* str) {
    // ��鴫����ַ����Ƿ�Ϊ��ָ��
    if (!str) {
        cerr << "Error: Invalid string pointer." << endl;
        return -1;
    }
    int len = strlen(str);
    // ʹ��strlen������ȡ�ַ����ĳ���
    return len;
}

// ����next����ĺ���
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

// KMP�㷨ģʽƥ�亯������������ƥ���Ӵ��ĳ�ʼλ��
vector<int> kmpSearchAll(const char* text, const char* pattern) {
    int textLength = getStringLength(text);
    int patternLength = getStringLength(pattern);
    vector<int> positions;  // ���ڴ洢����ƥ���Ӵ��ĳ�ʼλ��

    if (patternLength == 0) return positions;  // ���ģʽ��Ϊ�գ�ֱ�ӷ��ؿ�����

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
            positions.push_back(i - j);  // �ҵ�һ��ƥ���Ӵ�����¼���ʼλ��
            j = next[j];  // ����������һ��ƥ���Ӵ�
        }
    }

    delete[] next;  // �ͷ�next������ڴ�
    return positions;
}

// ����ĳ����ʼλ�õ�ĳ���ȵ��Ӵ��ĺ���
bool getSubstring(const char* str, int start, int length, char* subStr) {
    int strLength = getStringLength(str);

    // ����������Ч��
    if (start < 0 || start >= strLength || length <= 0 || start + length > strLength) {
        cerr << "Error: Invalid start position or length." << endl;
        subStr[0] = '\0';
        return false;
    }

    // �����Ӵ�
    strncpy(subStr, str + start, length);
    subStr[length] = '\0';  // ȷ���Ӵ��� null ��β
    return true;
}

// �滻������Replace(S, T, V)
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

    // �����µ��ַ�������
    int newLength = SLength + positions.size() * (VLength - TLength);
    if (newLength >= MAX_SIZE) {
        cerr << "Error: Resulting string length exceeds maximum allowed size." << endl;
        return false;
    }

    char* newString = new char[newLength + 1];
    int newIndex = 0;
    int oldIndex = 0;

    for (size_t i = 0; i < positions.size(); ++i) {
        // ������ǰλ�õ���һ��ƥ���Ӵ�֮����ַ�
        while (oldIndex < positions[i]) {
            newString[newIndex++] = S[oldIndex++];
        }

        // �����滻�ַ��� V
        for (int j = 0; j < VLength; ++j) {
            newString[newIndex++] = V[j];
        }

        // ����ƥ����Ӵ� T
        oldIndex += TLength;
    }

    // ����ʣ����ַ�
    while (oldIndex < SLength) {
        newString[newIndex++] = S[oldIndex++];
    }

    newString[newIndex] = '\0';  // ȷ�����ַ����� null ��β
    strncpy(S, newString, newLength + 1);
    delete[] newString;
    return true;
}

// ƴ�Ӻ���: Concatenate(S1, S2)
bool concatenateStrings(char* S1, const char* S2) {
    int S1Length = getStringLength(S1);
    int S2Length = getStringLength(S2);

    if (S1Length + S2Length >= MAX_SIZE) {
        cerr << "Error: Resulting string length exceeds maximum allowed size." << endl;
        return false;
    }

    strncpy(S1 + S1Length, S2, MAX_SIZE - S1Length - 1);  // ��S2ƴ�ӵ�S1��ĩβ
    S1[S1Length + S2Length] = '\0';  // ȷ�����ַ����� null ��β
    return true;
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
    char myString[MAX_SIZE];  // ����һ�������ַ�����
    char string1[MAX_SIZE];  
    char string2[MAX_SIZE];  

    // ��ʼ���ַ���
    initString(string1, "Hello, ");
    initString(string2, "World!");

    // ��ʼ���ַ���
    initString(myString, "Hello, World!");
    
    // ��������
    traverseString(myString);
    
    // �����ʼ������ַ���
    cout << "Initialized String: " << myString << endl; 

      // ����ַ����ĳ���
    int length = getStringLength(myString);
    cout << "String Length: " << length << endl;
    
    
    // ģʽƥ�����
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
    
    
      // ��ȡ�Ӵ�����
    char subString[MAX_SIZE];
    if (getSubstring(myString, 7, 6, subString)) {
        cout << "Substring: " << subString << endl;
    }
    
    
    // �滻����
    const char* oldSubstring = "World";
    const char* newSubstring = "Universe";
    if (replaceAll(myString, oldSubstring, newSubstring)) {
        cout << "String after replacement: " << myString << endl;
    }
    
     // ����ַ���
    clearString(myString);
    
    // �����պ���ַ���
    cout << "Cleared String: " << myString << endl;
    
     // �����պ��ַ����ĳ���
    length = getStringLength(myString);
    cout << "Cleared String Length: " << length << endl;
    
    // ƴ�Ӳ���
    if (concatenateStrings(string1, string2)) {
        cout << "Concatenated String: " << string1 << endl;
    }
    
    return 0;
}
