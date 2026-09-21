#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeNode {
    char data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

char input[256];
int pos = 0;

TreeNode* createNode(char data) {
    TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

TreeNode* parse() {
    while (input[pos] == ' ') pos++;
    if (input[pos] == '\0' || input[pos] == ')' || input[pos] == ',') return NULL;

    char val = input[pos];
    pos++;
    TreeNode *root = createNode(val);

    while (input[pos] == ' ') pos++;
    if (input[pos] == '(') {
        pos++;
        
        while (input[pos] == ' ') pos++;
        if (input[pos] != ',' && input[pos] != ')') {
            root->left = parse();
        }
        
        while (input[pos] == ' ') pos++;
        if (input[pos] == ',') {
            pos++;
            while (input[pos] == ' ') pos++;
            if (input[pos] != ')') {
                root->right = parse();
            }
        }
        
        while (input[pos] == ' ') pos++;
        if (input[pos] == ')') {
            pos++;
        }
    }
    return root;
}

void printTree(TreeNode *root, int depth) {
    if (root == NULL) return;

    printTree(root->right, depth + 1);

    for (int i = 0; i < depth; i++) {
        printf("    ");
    }
    printf("%c\n", root->data);

    printTree(root->left, depth + 1);
}

int countNodes(TreeNode *root) {
    if (root == NULL) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

int countLeafNodes(TreeNode *root) {
    if (root == NULL) return 0;
    if (root->left == NULL && root->right == NULL) return 1;
    return countLeafNodes(root->left) + countLeafNodes(root->right);
}

int getHeight(TreeNode *root) {
    if (root == NULL) return 0;
    int lh = getHeight(root->left);
    int rh = getHeight(root->right);
    if (lh > rh) return lh + 1;
    else return rh + 1;
}

int getDegree(TreeNode *root) {
    if (root == NULL) return 0;
    int deg = 0;
    if (root->left != NULL) deg++;
    if (root->right != NULL) deg++;

    int ld = getDegree(root->left);
    int rd = getDegree(root->right);
    int maxChild = ld;
    if (rd > maxChild) maxChild = rd;

    if (deg > maxChild) return deg;
    return maxChild;
}

int checkComplete(TreeNode *root, int index, int nodeCount) {
    if (root == NULL) return 1;
    if (index > nodeCount) return 0;

    if (!checkComplete(root->left, 2 * index, nodeCount)) return 0;
    if (!checkComplete(root->right, 2 * index + 1, nodeCount)) return 0;
    return 1;
}

int isComplete(TreeNode *root) {
    int total = countNodes(root);
    return checkComplete(root, 1, total);
}

int isFull(TreeNode *root) {
    int h = getHeight(root);
    int total = countNodes(root);
    int expected = 1;
    for (int i = 0; i < h; i++) {
        expected *= 2;
    }
    expected -= 1;
    return (total == expected);
}

int isSkewed(TreeNode *root) {
    if (root == NULL) return 1;
    if (root->left != NULL && root->right != NULL) return 0;
    return isSkewed(root->left) && isSkewed(root->right);
}

int main() {
    printf("연결 자료구조 이진트리 괄호식 입력: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\r\n")] = 0;

    TreeNode *root = parse();

    printf("\n[1] 이진트리 출력:\n");
    printTree(root, 0);

    int total = countNodes(root);
    int leaf = countLeafNodes(root);
    int nonLeaf = total - leaf;
    int height = getHeight(root);
    int degree = getDegree(root);

    printf("\n[2] 트리 정보 출력:\n");
    printf("- 전체 노드의 수: %d\n", total);
    printf("- 단말 노드의 수: %d\n", leaf);
    printf("- 비단말 노드의 수: %d\n", nonLeaf);
    printf("- 트리의 높이: %d\n", height);
    printf("- 트리의 차수: %d\n", degree);

    printf("\n[3] 이진트리의 형태 판별:\n");
    printf("- 완전 이진트리 여부: %s\n", isComplete(root) ? "O" : "X");
    printf("- 포화 이진트리 여부: %s\n", isFull(root) ? "O" : "X");
    printf("- 편향 이진트리 여부: %s\n", isSkewed(root) ? "O" : "X");

    return 0;
}
