#include <cstdio>
#include <cstring>

static void getCount(int preLeft, int preRight, int postLeft, int postRight,
        char preOrder[], char postOrder[], long long int& ans, int& times) {
    if (preLeft == preRight) return;
    if (preOrder[preLeft + 1] == postOrder[postRight - 1]) {
        ans *= 2, times++;
        getCount(preLeft + 1, preRight, postLeft,
                postRight - 1, preOrder, postOrder, ans, times);
        return;
    }
    int position = -1;
    for (int i = 0; preOrder[i]; i++)
        if (preOrder[i] == postOrder[postRight - 1]) {
            position = i;
            break;
        }
    getCount(preLeft + 1, position - 1, postLeft,
            postLeft + position - preLeft - 2, preOrder, postOrder, ans, times);
    getCount(position, preRight, postRight + position - preRight - 1,
            postRight - 1, preOrder, postOrder, ans, times);
}

static void solve(int preLeft, int preRight, int postLeft, int postRight,
        char preOrder[], char postOrder[], bool flag[], int& count) {
    if (preLeft == preRight) {
        putchar(preOrder[preLeft]);
        return;
    }
    if (preOrder[preLeft + 1] == postOrder[postRight - 1]) {
        if (flag[count++]) {
            solve(preLeft + 1, preRight, postLeft,
                    postRight - 1, preOrder, postOrder, flag, count);
            putchar(preOrder[preLeft]);
        }
        else {
            putchar(preOrder[preLeft]);
            solve(preLeft + 1, preRight, postLeft,
                    postRight - 1, preOrder, postOrder, flag, count);
        }
        return;
    }
    int position = -1;
    for (int i = 0; preOrder[i]; i++)
        if (preOrder[i] == postOrder[postRight - 1]) {
            position = i;
            break;
        }
    solve(preLeft + 1, position - 1, postLeft,
            postLeft + position - preLeft - 2, preOrder, postOrder, flag, count);
    putchar(preOrder[preLeft]);
    solve(position, preRight, postRight + position - preRight - 1,
            postRight - 1, preOrder, postOrder, flag, count);
}

int main() {
    bool flag[30];
    int times = 0;
    long long int ans = 1;
    char preOrder[30], postOrder[30];
    // ---------------------------------------
    scanf("%s", preOrder);
    scanf("%s", postOrder);
    int length = strlen(preOrder);
    getCount(0, length - 1, 0,
            length - 1, preOrder, postOrder, ans, times);
    printf("%lld\n", ans);
    // ---------------------------------------
    for (int i = 0; i < ans; i++) {
        int value = i, count = 0;
        for (int j = 0; j < times; j++) {
            flag[j] = (value % 2) == 1;
            value /= 2;
        }
        solve(0, length - 1, 0,
                length - 1, preOrder, postOrder, flag, count);
        putchar('\n');
    }
    return 0;
}
