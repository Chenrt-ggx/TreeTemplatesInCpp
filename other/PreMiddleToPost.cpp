#include <cstdio>
#include <cstring>

static void solve(int preLeft, int preRight, int middleLeft, int middleRight,
        char preOrder[], char middleOrder[]) {
    int position = -1;
    for (int i = 0; middleOrder[i]; i++)
        if (middleOrder[i] == preOrder[preLeft]) {
            position = i;
            break;
        }
    if (position > middleLeft) {
        solve(preLeft + 1, preLeft + position - middleLeft, middleLeft,
                position - 1, preOrder, middleOrder);
    }
    if (position < middleRight) {
        solve(preRight + position - middleRight + 1, preRight,
                position + 1, middleRight, preOrder, middleOrder);
    }
    putchar(preOrder[preLeft]);
}

int main() {
    char preOrder[30], middleOrder[30];
    scanf("%s", preOrder);
    scanf("%s", middleOrder);
    int length = strlen(preOrder);
    solve(0, length - 1, 0, length - 1, preOrder, middleOrder);
    return 0;
}
