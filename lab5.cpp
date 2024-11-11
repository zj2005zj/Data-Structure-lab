#include <iostream>
#include <stdlib.h>
#include <vector>
#include <queue>
using namespace std;

//定义存放数据的类型,假设是可以存放多个数据的结构体类型 
typedef struct{
    int id;     //序号 
    int value;  //值 
} ElemType; 

//定义二叉树的结点 
typedef struct tnode{
    ElemType data;         //该节点的值 
    struct tnode *lchild;  //左孩子 
    struct tnode *rchild;  //右孩子 
} bNode;

//递归算法(先序遍历)：构造二叉树 ，输入先序序列pre和中序序列in，以及两个序列各自的上界pu,iu和下界pl,il
bNode *buildBTreeByPI(int *pre, int pl, int pu, int *in, int il, int iu){
    printf("\n%d-%d,%d-%d",pl,pu,il,iu);
    int il1,iu1,il2,iu2,pl1,pl2,pu1,pu2; //根节点把中序序列分成两个子序列，同样得到先徐序列的两个子序列，这是子序列的下标 
    if (pu-pl!=iu-il) exit(0); //检查输入参数 
    if (pu-pl<0) return NULL;  //若序列为空，返回空结点/空树 
    bNode *t = (bNode *)malloc(sizeof(bNode));
    if (!t) exit(0);
    t->data.value = pre[pl];
    t->data.id = pl;
    for(int i=il;i<=iu;++i){
        if (in[i]==pre[pl]){
            iu1 = i-1;
            il1 = il;
            il2 = i+1;
            iu2 = iu;  //完成中序序列in的切割
            pl1 = pl+1;
            pu1 = pl+(iu1-il1+1);
            pl2 = pu1+1;
            pu2 = pu; //完成先徐序列pre的切割 
        }
    }
    t->lchild = buildBTreeByPI(pre,pl1,pu1,in,il1,iu1);
    t->rchild = buildBTreeByPI(pre,pl2,pu2,in,il2,iu2);
    return t; 
} 
// 层序遍历打印二叉树，并打印每个节点的值和id，并重新赋值id
void printTree(bNode *root) {
    if (root == NULL) return;

    queue<bNode*> q;
    q.push(root);
    int level = 0;
    int idCounter = 0; // id 计数器，从 0 开始
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
                current->data.id = idCounter++; // 重新赋值 id
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


// 递归删除所有值为特定值的节点及其子树
bNode* deleteNodesWithValue(bNode* root, int value) {
    if (root == NULL) return NULL;

    // 先递归处理左右子树
    root->lchild = deleteNodesWithValue(root->lchild, value);
    root->rchild = deleteNodesWithValue(root->rchild, value);

    // 如果当前节点的值等于要删除的值，返回NULL（删除该节点及其子树）
    if (root->data.value == value) {
        free(root);  // 释放内存
        return NULL;
    }

    // 否则返回当前节点
    return root;
}

// 辅助函数，用于在子树中查找具有特定id的节点
bNode* findInSubtree(bNode* root, int id) {
    if (root == NULL) return NULL;
    if (root->data.id == id) return root;
    bNode* left = findInSubtree(root->lchild, id);
    if (left) return left;
    return findInSubtree(root->rchild, id);
}

bNode* findLCA(bNode* root, int id1, int id2) {
    if (root == NULL) return NULL;

    // 分别在左右子树中查找id1和id2
    bNode* left = findInSubtree(root->lchild, id1);
    bNode* right = findInSubtree(root->lchild, id2);

    if (left && right) {
        // 如果id1和id2都在左子树中找到，则在左子树中继续寻找LCA
        return findLCA(root->lchild, id1, id2);
    }

    left = findInSubtree(root->rchild, id1);
    right = findInSubtree(root->rchild, id2);

    if (left && right) {
        // 如果id1和id2都在右子树中找到，则在右子树中继续寻找LCA
        return findLCA(root->rchild, id1, id2);
    }

    // 如果一个节点在当前节点的左子树，另一个在右子树，或者当前节点就是其中一个节点，则当前节点是LCA
    if ((findInSubtree(root->lchild, id1) || root->data.id == id1) && 
        (findInSubtree(root->rchild, id2) || root->data.id == id2)) {
        return root;
    }
    if ((findInSubtree(root->lchild, id2) || root->data.id == id2) && 
        (findInSubtree(root->rchild, id1) || root->data.id == id1)) {
        return root;
    }

    // 如果id1和id2都不在当前节点的子树中，则向上回溯
    return NULL;
}



// 辅助函数，递归打印路径
bool printPathToNode(bNode* root, int id, string path) {
    if (root == NULL) return false;

    // 如果当前节点是目标节点，打印路径
    if (root->data.id == id) {
        cout << path << endl;
        return true;
    }

    // 递归查找左子树
    if (printPathToNode(root->lchild, id, path + "左")) return true;

    // 递归查找右子树
    if (printPathToNode(root->rchild, id, path + "右")) return true;

    return false;
}

// 调用辅助函数打印路径
void printPathToNode(bNode* root, int id) {
    if (root == NULL) {
        cout << "空树" << endl;
        return;
    }

    if (findInSubtree(root, id) == NULL) {
        cout << "id为" << id << "未找到" << endl;
        return;
    }

    printPathToNode(root, id, "");
}
// 辅助函数，用于递归查找二叉树中的最大值和最小值
void findMinMaxValue(bNode* root, int& minValue, int& maxValue) {
    if (root == NULL) return;

    // 更新最大值和最小值
    if (root->data.value < minValue) minValue = root->data.value;
    if (root->data.value > maxValue) maxValue = root->data.value;

    // 递归查找左右子树
    findMinMaxValue(root->lchild, minValue, maxValue);
    findMinMaxValue(root->rchild, minValue, maxValue);
}

// 计算二叉树节点中最大的value和最小的value之差
int calculateValueDifference(bNode* root) {
    if (root == NULL) return 0;

    int minValue = root->data.value;
    int maxValue = root->data.value;

    // 递归查找最大值和最小值
    findMinMaxValue(root, minValue, maxValue);

    // 返回最大值和最小值之差
    return maxValue - minValue;
}

int main() {
    // 先序遍历序列
    int pre[] = {23, 7, 13, 10, 8, 14, 17, 18, 33, 76, 42, 89};
    // 中序遍历序列
    int in[] = {7, 8, 10, 13, 14, 17, 18, 23, 33, 42, 76, 89};

    // 构建二叉树
    bNode *root = buildBTreeByPI(pre, 0, 11, in, 0, 11);
    cout << "\n";
    // 打印二叉树
    printTree(root);
    
    
     // 查找id1和id2的最近共同祖先
    int id1 = 9;
    int id2 = 10;
    bNode* lca = findLCA(root, id1, id2);
    if (lca) {
        cout <<  id1 << " 和 " << id2 << "的最近共同祖先" << " 是 " << lca->data.value << endl;
    } else {
        cout << "未找到." << endl;
    }
    
    // 示例：打印从根节点到id为10的节点的路径
    int targetId = 7;
    cout << "路径" << targetId << ":" << endl;
    printPathToNode(root, targetId);
    
     // 计算二叉树节点中最大的value和最小的value之差
    int valueDifference = calculateValueDifference(root);
    cout << "最大值最小值之差为: " << valueDifference << endl;
    // 删除值为17的节点及其子树
    int deleteValue = 17;
    root = deleteNodesWithValue(root, deleteValue);
  
    // 打印删除后的二叉树
    cout << "删除节点后的二叉树 " << deleteValue << ":" << endl;
    printTree(root);
    
    // 释放树的内存（如果有必要）
    // 这里省略了释放内存的代码

    return 0;
}








