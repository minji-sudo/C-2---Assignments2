/*
[3번 과제 비교 정리]
1. 메모리 사용량:
- 완전 이진트리는 빈 공간이 없어서 포인터 오버헤드가 없는 배열 방식이 더 효율적임.
- 편향 이진트리는 배열로 구현하면 낭비되는 빈칸이 너무 커져서, 필요한 노드만 할당하는 포인터 방식이 훨씬 효율적임.

2. 노드 탐색 효율성:
- 배열은 인덱스 계산(i/2, 2*i)으로 부모와 자식을 바로 찾을 수 있어서(O(1)) 더 빠름.
- 포인터는 자식은 바로 찾지만, 부모나 형제를 찾으려면 루트부터 전체 트리를 순회해야 해서(O(N)) 배열보다 비효율적임.
*/

#define MAX_SIZE 1024

char arrTree[MAX_SIZE];
char input[256];
int pos = 0;

typedef struct TreeNode {
    char data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

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
        if (input[pos] != ',' && input[pos] != ')') root->left = parse();
        
        while (input[pos] == ' ') pos++;
        if (input[pos] == ',') {
            pos++;
            while (input[pos] == ' ') pos++;
            if (input[pos] != ')') root->right = parse();
        }
        while (input[pos] == ' ') pos++;
        if (input[pos] == ')') pos++;
    }
    return root;
}

void copyToArray(TreeNode *root, int idx) {
    if (root == NULL || idx >= MAX_SIZE) return;
    arrTree[idx] = root->data;
    copyToArray(root->left, 2 * idx);
    copyToArray(root->right, 2 * idx + 1);
}

void searchInArray(char target) {
    int idx = -1;
    for (int i = 1; i < MAX_SIZE; i++) {
        if (arrTree[i] == target) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("[배열] 노드 %c를 찾을 수 없습니다.\n", target);
        return;
    }

    printf("\n[배열 구현 탐색 결과: %c]\n", target);

    if (idx == 1) {
        printf("- 부모 노드: 없음 (루트)\n");
    } else {
        printf("- 부모 노드: %c\n", arrTree[idx / 2]);
    }

    int left = 2 * idx;
    int right = 2 * idx + 1;
    if (left < MAX_SIZE && arrTree[left] != 0) {
        printf("- 왼쪽 자식: %c\n", arrTree[left]);
    } else {
        printf("- 왼쪽 자식: 없음\n");
    }

    if (right < MAX_SIZE && arrTree[right] != 0) {
        printf("- 오른쪽 자식: %c\n", arrTree[right]);
    } else {
        printf("- 오른쪽 자식: 없음\n");
    }

    if (idx == 1) {
        printf("- 형제 노드: 없음\n");
    } else {
        int sibIdx;
        if (idx % 2 == 0) sibIdx = idx + 1;
        else sibIdx = idx - 1;

        if (sibIdx < MAX_SIZE && arrTree[sibIdx] != 0) {
            printf("- 형제 노드: %c\n", arrTree[sibIdx]);
        } else {
            printf("- 형제 노드: 없음\n");
        }
    }
}

TreeNode* findNode(TreeNode *curr, char target) {
    if (curr == NULL) return NULL;
    if (curr->data == target) return curr;

    TreeNode *found = findNode(curr->left, target);
    if (found != NULL) return found;
    return findNode(curr->right, target);
}

TreeNode* findParent(TreeNode *curr, char target) {
    if (curr == NULL) return NULL;
    if ((curr->left && curr->left->data == target) || 
        (curr->right && curr->right->data == target)) {
        return curr;
    }
    TreeNode *found = findParent(curr->left, target);
    if (found != NULL) return found;
    return findParent(curr->right, target);
}

void searchInPointer(TreeNode *root, char target) {
    TreeNode *node = findNode(root, target);
    if (node == NULL) {
        printf("[연결 구조] 노드 %c를 찾을 수 없습니다.\n", target);
        return;
    }

    printf("\n[연결 구조 구현 탐색 결과: %c]\n", target);

    TreeNode *parent = findParent(root, target);
    if (parent != NULL) {
        printf("- 부모 노드: %c\n", parent->data);
        if (parent->left && parent->left->data == target) {
            if (parent->right != NULL) printf("- 형제 노드: %c\n", parent->right->data);
            else printf("- 형제 노드: 없음\n");
        } else {
            if (parent->left != NULL) printf("- 형제 노드: %c\n", parent->left->data);
            else printf("- 형제 노드: 없음\n");
        }
    } else {
        printf("- 부모 노드: 없음 (루트)\n");
        printf("- 형제 노드: 없음\n");
    }

    if (node->left != NULL) {
        printf("- 왼쪽 자식: %c\n", node->left->data);
    } else {
        printf("- 왼쪽 자식: 없음\n");
    }

    if (node->right != NULL) {
        printf("- 오른쪽 자식: %c\n", node->right->data);
    } else {
        printf("- 오른쪽 자식: 없음\n");
    }
}

int main() {
    for (int i = 0; i < MAX_SIZE; i++) arrTree[i] = 0;

    printf("이진트리 괄호식 입력: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\r\n")] = 0;

    TreeNode *root = parse();
    copyToArray(root, 1);

    char target;
    printf("탐색할 노드 입력: ");
    scanf(" %c", &target);

    searchInArray(target);
    searchInPointer(root, target);

    return 0;
}
