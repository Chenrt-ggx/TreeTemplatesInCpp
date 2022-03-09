#ifndef TREESTRUCTURE_FASTIO_H
#define TREESTRUCTURE_FASTIO_H

#include <cstdio>

inline int readInt() {
    int ch = getchar();
    int op = 1, value = 0;
    while (ch < '0' || ch > '9') {
        if (ch == '-') op = -1;
        ch = getchar();
    }
    while ('0' <= ch && ch <= '9') {
        value = value * 10 + ch - 48;
        ch = getchar();
    }
    return op * value;
}

void writeUnsignedInt(int value) {
    if (value > 9) writeUnsignedInt(value / 10);
    putchar(value % 10 + 48);
}

inline void writeInt(int value) {
    if (value < 0) putchar('-'), value = -value;
    writeUnsignedInt(value), putchar('\n');
}

void writeUnsignedLongLong(long long value) {
    if (value > 9) writeUnsignedLongLong(value / 10);
    putchar((int)(value % 10) + 48);
}

inline void writeLongLong(long long value) {
    if (value < 0) putchar('-'), value = -value;
    writeUnsignedLongLong(value), putchar('\n');
}

#endif //TREESTRUCTURE_FASTIO_H
