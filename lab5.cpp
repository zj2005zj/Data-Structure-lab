#include <iostream>
#include <stdlib.h>
#include <vector>
#include <queue>
using namespace std;

//���������ݵ�����,�����ǿ��Դ�Ŷ�����ݵĽṹ������ 
typedef struct{
    int id;     //��� 
    int value;  //ֵ 
} ElemType; 

//����������Ľ�� 
typedef struct tnode{
    ElemType data;         //�ýڵ��ֵ 
    struct tnode *lchild;  //���� 
    struct tnode *rchild;  //�Һ��� 
} bNode;

//�ݹ��㷨(�������)����������� ��������������pre����������in���Լ��������и��Ե��Ͻ�pu,iu���½�pl,il
bNode *buildBTreeByPI(int *pre, int pl, int pu, int *in, int il, int iu){
    printf("\n%d-%d,%d-%d",pl,pu,il,iu);
    int il1,iu1,il2,iu2,pl1,pl2,pu1,pu2; //���ڵ���������зֳ����������У�ͬ���õ��������е����������У����������е��±� 
    if (pu-pl!=iu-il) exit(0); //���������� 
    if (pu-pl<0) return NULL;  //������Ϊ�գ����ؿս��/���� 
    bNode *t = (bNode *)malloc(sizeof(bNode));
    if (!t) exit(0);
    t->data.value = pre[pl];
    t->data.id = pl;
    for(int i=il;i<=iu;++i){
        if (in[i]==pre[pl]){
            iu1 = i-1;
            il1 = il;
            il2 = i+1;
            iu2 = iu;  //�����������in���и�
            pl1 = pl+1;
            pu1 = pl+(iu1-il1+1);
            pl2 = pu1+1;
            pu2 = pu; //�����������pre���и� 
        }
    }
    t->lchild = buildBTreeByPI(pre,pl1,pu1,in,il1,iu1);
    t->rchild = buildBTreeByPI(pre,pl2,pu2,in,il2,iu2);
    return t; 
} 
// ���������ӡ������������ӡÿ���ڵ��ֵ��id�������¸�ֵid
void printTree(bNode *root) {
    if (root == NULL) return;

    queue<bNode*> q;
    q.push(root);
    int level = 0;
    int idCounter = 0; // id ���������� 0 ��ʼ
    bool hasNodes = true;

    while (hasNodes) {
        int size = q.size();
        vector<bNode*> currentLevel(size, NULL);
        hasNodes = false;

        for (int i = 0; i < size; ++i) {
            bNode *current = q.front();
            q.pop();
            currentLevel[i] = current;
            if (current) {
                current->data.id = idCounter++; // ���¸�ֵ id
                q.push(current->lchild);
                q.push(current->rchild);
                hasNodes = hasNodes || current->lchild || current->rchild;
            } else {
                q.push(NULL);
                q.push(NULL);
            }
        }

        cout << "Level " << level << ": ";
        for (int i = 0; i < currentLevel.size(); ++i) {
            if (currentLevel[i]) {
                cout << "(" << currentLevel[i]->data.value << ", " << currentLevel[i]->data.id << ") ";
            } else {
                cout << "N ";
            }
        }
        cout << endl;
        level++;
    }
}


// �ݹ�ɾ������ֵΪ�ض�ֵ�Ľڵ㼰������
bNode* deleteNodesWithValue(bNode* root, int value) {
    if (root == NULL) return NULL;

    // �ȵݹ鴦����������
    root->lchild = deleteNodesWithValue(root->lchild, value);
    root->rchild = deleteNodesWithValue(root->rchild, value);

    // �����ǰ�ڵ��ֵ����Ҫɾ����ֵ������NULL��ɾ���ýڵ㼰��������
    if (root->data.value == value) {
        free(root);  // �ͷ��ڴ�
        return NULL;
    }

    // ���򷵻ص�ǰ�ڵ�
    return root;
}

// ���������������������в��Ҿ����ض�id�Ľڵ�
bNode* findInSubtree(bNode* root, int id) {
    if (root == NULL) return NULL;
    if (root->data.id == id) return root;
    bNode* left = findInSubtree(root->lchild, id);
    if (left) return left;
    return findInSubtree(root->rchild, id);
}

bNode* findLCA(bNode* root, int id1, int id2) {
    if (root == NULL) return NULL;

    // �ֱ������������в���id1��id2
    bNode* left = findInSubtree(root->lchild, id1);
    bNode* right = findInSubtree(root->lchild, id2);

    if (left && right) {
        // ���id1��id2�������������ҵ��������������м���Ѱ��LCA
        return findLCA(root->lchild, id1, id2);
    }

    left = findInSubtree(root->rchild, id1);
    right = findInSubtree(root->rchild, id2);

    if (left && right) {
        // ���id1��id2�������������ҵ��������������м���Ѱ��LCA
        return findLCA(root->rchild, id1, id2);
    }

    // ���һ���ڵ��ڵ�ǰ�ڵ������������һ���������������ߵ�ǰ�ڵ��������һ���ڵ㣬��ǰ�ڵ���LCA
    if ((findInSubtree(root->lchild, id1) || root->data.id == id1) && 
        (findInSubtree(root->rchild, id2) || root->data.id == id2)) {
        return root;
    }
    if ((findInSubtree(root->lchild, id2) || root->data.id == id2) && 
        (findInSubtree(root->rchild, id1) || root->data.id == id1)) {
        return root;
    }

    // ���id1��id2�����ڵ�ǰ�ڵ�������У������ϻ���
    return NULL;
}



// �����������ݹ��ӡ·��
bool printPathToNode(bNode* root, int id, string path) {
    if (root == NULL) return false;

    // �����ǰ�ڵ���Ŀ��ڵ㣬��ӡ·��
    if (root->data.id == id) {
        cout << path << endl;
        return true;
    }

    // �ݹ����������
    if (printPathToNode(root->lchild, id, path + "��")) return true;

    // �ݹ����������
    if (printPathToNode(root->rchild, id, path + "��")) return true;

    return false;
}

// ���ø���������ӡ·��
void printPathToNode(bNode* root, int id) {
    if (root == NULL) {
        cout << "����" << endl;
        return;
    }

    if (findInSubtree(root, id) == NULL) {
        cout << "idΪ" << id << "δ�ҵ�" << endl;
        return;
    }

    printPathToNode(root, id, "");
}
// �������������ڵݹ���Ҷ������е����ֵ����Сֵ
void findMinMaxValue(bNode* root, int& minValue, int& maxValue) {
    if (root == NULL) return;

    // �������ֵ����Сֵ
    if (root->data.value < minValue) minValue = root->data.value;
    if (root->data.value > maxValue) maxValue = root->data.value;

    // �ݹ������������
    findMinMaxValue(root->lchild, minValue, maxValue);
    findMinMaxValue(root->rchild, minValue, maxValue);
}

// ����������ڵ�������value����С��value֮��
int calculateValueDifference(bNode* root) {
    if (root == NULL) return 0;

    int minValue = root->data.value;
    int maxValue = root->data.value;

    // �ݹ�������ֵ����Сֵ
    findMinMaxValue(root, minValue, maxValue);

    // �������ֵ����Сֵ֮��
    return maxValue - minValue;
}

int main() {
    // �����������
    int pre[] = {23, 7, 13, 10, 8, 14, 17, 18, 33, 76, 42, 89};
    // �����������
    int in[] = {7, 8, 10, 13, 14, 17, 18, 23, 33, 42, 76, 89};

    // ����������
    bNode *root = buildBTreeByPI(pre, 0, 11, in, 0, 11);
    cout << "\n";
    // ��ӡ������
    printTree(root);
    
    
     // ����id1��id2�������ͬ����
    int id1 = 9;
    int id2 = 10;
    bNode* lca = findLCA(root, id1, id2);
    if (lca) {
        cout <<  id1 << " �� " << id2 << "�������ͬ����" << " �� " << lca->data.value << endl;
    } else {
        cout << "δ�ҵ�." << endl;
    }
    
    // ʾ������ӡ�Ӹ��ڵ㵽idΪ10�Ľڵ��·��
    int targetId = 7;
    cout << "·��" << targetId << ":" << endl;
    printPathToNode(root, targetId);
    
     // ����������ڵ�������value����С��value֮��
    int valueDifference = calculateValueDifference(root);
    cout << "���ֵ��Сֵ֮��Ϊ: " << valueDifference << endl;
    // ɾ��ֵΪ17�Ľڵ㼰������
    int deleteValue = 17;
    root = deleteNodesWithValue(root, deleteValue);
  
    // ��ӡɾ����Ķ�����
    cout << "ɾ���ڵ��Ķ����� " << deleteValue << ":" << endl;
    printTree(root);
    
    // �ͷ������ڴ棨����б�Ҫ��
    // ����ʡ�����ͷ��ڴ�Ĵ���

    return 0;
}








