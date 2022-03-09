#include <cstdio>
#include <cstring>

static void solve(int middleLeft, int middleRight, int postLeft, int postRight,
        char middleOrder[], char postOrder[]) {
    int position = -1;
    putchar(postOrder[postRight]);
    for (int i = 0; middleOrder[i]; i++)
        if (middleOrder[i] == postOrder[postRight]) {
            position = i;
            break;
        }
    if (position > middleLeft) {
        solve(middleLeft, position - 1, postLeft,
                postRight - middleRight + position - 1, middleOrder, postOrder);
    }
    if (position < middleRight) {
        solve(position + 1, middleRight, postLeft - middleLeft + position,
                postRight - 1, middleOrder, postOrder);
    }
}

int main() {
    char middleOrder[30], postOrder[30];
    scanf("%s", middleOrder);
    scanf("%s", postOrder);
    int length = strlen(middleOrder);
    solve(0, length - 1, 0, length - 1, middleOrder, postOrder);
    return 0;
}
