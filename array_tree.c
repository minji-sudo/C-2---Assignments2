#include <stdio.h>
#include <string.h>

#define MAX_SIZE 1024

char tree[MAX_SIZE];
char input[256];
int pos = 0;

void parse(int idx) {
    while (input[pos] == ' ') pos++;
    if (input[pos] == '\0' || input[pos] == ')' || input[pos] == ',') return;

    if (idx < MAX_SIZE) {
        tree[idx] = input[pos];
    }
    pos++;

    while (input[pos] == ' ') pos++;
    if (input[pos] == '(') {
        pos++;
        
        while (input[pos] == ' ') pos++;
        if (input[pos] != ',' && input[pos] != ')') {
            parse(2 * idx);
        }
        
        while (input[pos] == ' ') pos++;
        if (input[pos] == ',') {
            pos++;
            while (input[pos] == ' ') pos++;
            if (input[pos] != ')') {
                parse(2 * idx + 1);
            }
        }
        
        while (input[pos] == ' ') pos++;
        if (input[pos] == ')') {
            pos++;
        }
    }
}

void printTree(int idx, int depth) {
    if (idx >= MAX_SIZE || tree[idx] == 0) return;

    printTree(2 * idx + 1, depth + 1);

    for (int i = 0; i < depth; i++) {
        printf("    ");
    }
    printf("%c\n", tree[idx]);

    printTree(2 * idx, depth + 1);
}

int countNodes() {
    int count = 0;
    for (int i = 1; i < MAX_SIZE; i++) {
        if (tree[i] != 0) count++;
    }
    return count;
}

int countLeafNodes() {
    int count = 0;
    for (int i = 1; i < MAX_SIZE; i++) {
        if (tree[i] != 0) {
            int left = 2 * i;
            int right = 2 * i + 1;
            int hasLeft = (left < MAX_SIZE && tree[left] != 0);
            int hasRight = (right < MAX_SIZE && tree[right] != 0);
            if (!hasLeft && !hasRight) {
                count++;
            }
        }
    }
    return count;
}

int getHeight(int idx) {
    if (idx >= MAX_SIZE || tree[idx] == 0) return 0;
    int lh = getHeight(2 * idx);
    int rh = getHeight(2 * idx + 1);
    if (lh > rh) return lh + 1;
    else return rh + 1;
}

int getDegree() {
    int maxDeg = 0;
    for (int i = 1; i < MAX_SIZE; i++) {
        if (tree[i] != 0) {
            int deg = 0;
            if (2 * i < MAX_SIZE && tree[2 * i] != 0) deg++;
            if (2 * i + 1 < MAX_SIZE && tree[2 * i + 1] != 0) deg++;
            if (deg > maxDeg) maxDeg = deg;
        }
    }
    return maxDeg;
}

int isComplete(int total) {
    for (int i = 1; i <= total; i++) {
        if (tree[i] == 0) return 0;
    }
    return 1;
}

int isFull(int total, int h) {
    int expected = 1;
    for (int i = 0; i < h; i++) {
        expected *= 2;
    }
    expected -= 1;
    return (total == expected);
}

int isSkewed(int idx) {
    if (idx >= MAX_SIZE || tree[idx] == 0) return 1;
    
    int left = 2 * idx;
    int right = 2 * idx + 1;
    int hasLeft = (left < MAX_SIZE && tree[left] != 0);
    int hasRight = (right < MAX_SIZE && tree[right] != 0);

    if (hasLeft && hasRight) return 0;

    return isSkewed(left) && isSkewed(right);
}

int main() {
    for (int i = 0; i < MAX_SIZE; i++) tree[i] = 0;

    printf("배열 이진트리 괄호식 입력: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\r\n")] = 0;

    parse(1);

    printf("\n[1] 이진트리 출력:\n");
    printTree(1, 0);

    int total = countNodes();
    int leaf = countLeafNodes();
    int nonLeaf = total - leaf;
    int height = getHeight(1);
    int degree = getDegree();

    printf("\n[2] 트리 정보 출력:\n");
    printf("- 전체 노드의 수: %d\n", total);
    printf("- 단말 노드의 수: %d\n", leaf);
    printf("- 비단말 노드의 수: %d\n", nonLeaf);
    printf("- 트리의 높이: %d\n", height);
    printf("- 트리의 차수: %d\n", degree);

    printf("\n[3] 이진트리의 형태 판별:\n");
    printf("- 완전 이진트리 여부: %s\n", isComplete(total) ? "O" : "X");
    printf("- 포화 이진트리 여부: %s\n", isFull(total, height) ? "O" : "X");
    printf("- 편향 이진트리 여부: %s\n", isSkewed(1) ? "O" : "X");

    return 0;
}
