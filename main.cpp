#include <map>
#include <vector>
#include <string>
#include <cstring>

#include "./trees/Treap.h"
#include "./trees/Splay.h"
#include "./trees/RBTree.h"
#include "./trees/SBTree.h"
#include "./trees/SGTree.h"
#include "./trees/AVLTree.h"
#include "./trees/FHQTreap.h"

#include "./trees/BITree.h"
#include "./trees/SegmentTree.h"

#include "./utils/FastIO.h"
#include "./utils/Comparator.h"

inline void P3374Solver() {
    int init = readInt(), total = readInt();
    auto biTree = new BITree<int>(init, 0, [](int x, int y) {
        return x + y;
    });
    for (int i = 1; i <= init; i++)  biTree -> update(i, readInt());
    for (int i = 0, value; i < total; i++) {
        switch (readInt()) {
            case 1:
                value = readInt(), biTree -> update(value, readInt());
                break;
            case 2:
                value = biTree -> query(readInt() - 1), writeInt(biTree -> query(readInt()) - value);
                break;
            default:
                break;
        }
    }
}

inline void P1908Solver() {
    long long ans = 0;
    int count = 0, total = readInt();
    std::map<int, int> map;
    std::vector<int> array(total);
    for (int i = 0; i < total; i++) array[i] = readInt(), map[array[i]] = 0;
    for (auto i : map) map[i.first] = ++count;
    auto biTree = new BITree<int>(count, 0, [](int x, int y) { return x + y; });
    for (int i = total - 1; i >= 0; i--) {
        ans += biTree -> query(map[array[i]] - 1);
        biTree -> update(map[array[i]], 1);
    }
    writeLongLong(ans);
}

inline void P3368Solver() {
    int init = readInt(), total = readInt();
    auto segmentTree = new SegmentTree<int>(init, 0,
            [](int x, int y) { return x + y; }, [](int x, int y) { return x * y; });
    for (int i = 1; i <= init; i++)  segmentTree -> insert(i, readInt());
    for (int i = 0, left, right; i < total; i++) {
        switch (readInt()) {
            case 1:
                left = readInt(), right = readInt();
                segmentTree -> update(left, right, readInt());
                break;
            case 2:
                writeInt(segmentTree -> query(readInt()));
                break;
            default:
                break;
        }
    }
}

inline void P3372Solver() {
    int init = readInt(), total = readInt();
    auto segmentTree = new SegmentTree<long long>(init, 0,
            [](long long x, long long y) { return x + y; }, [](long long x, int y) { return x * y; });
    for (int i = 1; i <= init; i++)  segmentTree -> insert(i, readInt());
    for (int i = 0, left, right; i < total; i++) {
        switch (readInt()) {
            case 1:
                left = readInt(), right = readInt();
                segmentTree -> update(left, right, readInt());
                break;
            case 2:
                left = readInt(), right = readInt();
                writeLongLong(segmentTree -> query(left, right));
                break;
            default:
                break;
        }
    }
}

inline void P3369Solver(BalancedTree<int>* tree) {
    int total = readInt();
    for (int i = 0; i < total; i++) {
        switch (readInt()) {
            case 1:
                tree -> insert(readInt());
                break;
            case 2:
                tree -> remove(readInt());
                break;
            case 3:
                writeInt(tree -> rank(readInt()));
                break;
            case 4:
                writeInt(tree -> select(readInt()));
                break;
            case 5:
                writeInt(tree -> predecessor(readInt()));
                break;
            case 6:
                writeInt(tree -> successor(readInt()));
                break;
            default:
                break;
        }
    }
}

inline void P6136Solver(BalancedTree<int>* tree) {
    unsigned ans = 0, last = 0;
    int init = readInt(), total = readInt();
    for (int i = 0; i < init; i++)  tree -> insert(readInt());
    for (int i = 0; i < total; i++) {
        switch (readInt()) {
            case 1:
                tree -> insert((unsigned) readInt() ^ last);
                break;
            case 2:
                tree -> remove((unsigned) readInt() ^ last);
                break;
            case 3:
                last = (unsigned)tree -> rank((unsigned) readInt() ^ last), ans ^= last;
                break;
            case 4:
                last = (unsigned)tree -> select((int)((unsigned) readInt() ^ last)), ans ^= last;
                break;
            case 5:
                last = (unsigned)tree -> predecessor((unsigned) readInt() ^ last), ans ^= last;
                break;
            case 6:
                last = (unsigned)tree -> successor((unsigned) readInt() ^ last), ans ^= last;
                break;
            default:
                break;
        }
    }
    writeInt((int) ans);
}

inline void stdoutBackToConsole() {
    fflush(stdout);
#ifdef _WIN64
    freopen("CON", "w", stdout);
#else
    freopen("/dev/console", "w", stdout);
#endif
}

inline void local() {
    std::vector<BalancedTree<int>*> trees;
    trees.push_back(new AVLTree<int>);
    trees.push_back(new SBTree<int>);
    trees.push_back(new Treap<int>);
    trees.push_back(new FHQTreap<int>);
    trees.push_back(new RBTree<int>);
    trees.push_back(new Splay<int>);
    trees.push_back(new SGTree<int>);
    for (BalancedTree<int>* i : trees) {
        for (int j = 1; j <= 10; j++) {
            printf("%s, test case %d : ", i -> implement(), j);
            freopen(("../test/test" + std::to_string(j) + ".in").c_str(), "r", stdin);
            freopen(("../test/test" + std::to_string(j) + ".out").c_str(), "w", stdout);
            i -> clear(), P3369Solver(i);
            stdoutBackToConsole();
            compare(("../test/test" + std::to_string(j) + ".std").c_str(),
                    ("../test/test" + std::to_string(j) + ".out").c_str());
        }
        putchar('\n');
    }
    for (int i = 1; i <= 10; i++) {
        system(("rm ../test/test" + std::to_string(i) + ".out").c_str());
    }
}

int main(int argc, char const *argv[]) {
    if (argc == 2 && strcmp(argv[1], "-Local") == 0) {
        local();
    }
    else if (argc == 2 && strcmp(argv[1], "-P3374") == 0) {
        P3374Solver();
    }
    else if (argc == 2 && strcmp(argv[1], "-P1908") == 0) {
        P1908Solver();
    }
    else if (argc == 2 && strcmp(argv[1], "-P3368") == 0) {
        P3368Solver();
    }
    else if (argc == 2 && strcmp(argv[1], "-P3372") == 0) {
        P3372Solver();
    }
    else if (argc == 2 && strcmp(argv[1], "-P3369") == 0) {
        P3369Solver(new AVLTree<int>());
    }
    else if (argc == 2 && strcmp(argv[1], "-P6136") == 0) {
        P6136Solver(new AVLTree<int>());
    }
    return 0;
}
