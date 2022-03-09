#ifndef TREESTRUCTURE_BITREE_H
#define TREESTRUCTURE_BITREE_H

#include <vector>
#include <functional>

template <typename T> class BITree {
private:
    T zeroElement;
    std::vector<T> tree;
    std::function<T(T, T)> adder;

public:
    BITree(int size, T zeroElement, std::function<T(T, T)> adder) {
        this -> adder = adder;
        this -> zeroElement = zeroElement;
        tree = *(new std::vector<T>(size, zeroElement));
    }

    inline void update(int index, T value) {
        for (unsigned i = index; i <= tree.size(); i += i & -i)
            tree[i - 1] = adder(tree[i - 1], value);
    }

    inline T query(int index) {
        T result = zeroElement;
        for (unsigned i = index; i > 0; i -= i & -i)
            result = adder(result, tree[i - 1]);
        return result;
    }
};

#endif //TREESTRUCTURE_BITREE_H
