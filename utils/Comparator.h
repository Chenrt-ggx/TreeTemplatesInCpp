#ifndef TREESTRUCTURE_COMPARATOR_H
#define TREESTRUCTURE_COMPARATOR_H

#include <cstdio>
#include <cstdlib>
#include <cassert>

static inline void fixTail(char* str) {
    if (str != nullptr) {
        for (int i = 0; str[i]; i++)
            if (str[i] == '\r' || str[i] == '\n') {
                str[i] = 0;
                return;
            }
    }
}

inline void compare(const char* std, const char* out) {
    char* buffer1 = (char*)malloc((unsigned)1 << (unsigned)16);
    char* buffer2 = (char*)malloc((unsigned)1 << (unsigned)16);
    FILE* stdFile = fopen(std, "r");
    FILE* outFile = fopen(out, "r");
    assert(stdFile), assert(outFile);
    assert(buffer1), assert(buffer2);
    for (int *stdLine, *outLine, lines = 1; true; lines++) {
        while (true) {
            stdLine = (int*)fgets(buffer1, (unsigned)1 << (unsigned)16, stdFile);
            fixTail(buffer1);
            if (stdLine == nullptr || (char)stdLine[0]) break;
        }
        while (true) {
            outLine = (int*)fgets(buffer2, (unsigned)1 << (unsigned)16, outFile);
            fixTail(buffer2);
            if (outLine == nullptr || (char)outLine[0]) break;
        }
        if (stdLine == nullptr && outLine == nullptr) {
            puts("accepted");
            break;
        }
        if (stdLine == nullptr || outLine == nullptr) {
            printf("different at line %d, ", lines);
            if (stdLine == nullptr) puts("std is too short");
            if (outLine == nullptr) puts("out is too short");
            break;
        }
        if (strcmp(buffer1, buffer2) != 0) {
            printf("different at line %d, ", lines);
            printf("answer in std : %s, ", buffer1);
            printf("answer in out : %s\n", buffer2);
            break;
        }
    }
    fclose(stdFile), fclose(outFile);
}

#endif //TREESTRUCTURE_COMPARATOR_H
