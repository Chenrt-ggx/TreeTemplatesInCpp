#ifndef TREESTRUCTURE_BALANCEDTREE_H
#define TREESTRUCTURE_BALANCEDTREE_H

template <typename T> class  BalancedTree {
public:
    virtual void clear() = 0;

    virtual const char* implement() = 0;

    virtual void insert(T data) = 0;

    virtual void remove(T data) = 0;

    virtual int rank(T data) = 0;

    virtual T select(int rank) = 0;

    virtual T predecessor(T data) = 0;

    virtual T successor(T data) = 0;
};

#endif //TREESTRUCTURE_BALANCEDTREE_H
