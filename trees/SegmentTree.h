#ifndef TREESTRUCTURE_SEGMENTTREE_H
#define TREESTRUCTURE_SEGMENTTREE_H

#include <functional>

template <typename T> class SegmentTree {
private:
    template <typename S> class Node {
    public:
        S tag, data;
        Node<S>* leftSon;
        Node<S>* rightSon;

        explicit Node(T zeroElement) {
            tag = data = zeroElement;
            leftSon = rightSon = nullptr;
        }
    };

    T zeroElement;
    int leftRange{}, rightRange{}, size{};

    Node<T>* root;
    std::function<T(T, T)> adder;
    std::function<T(T, int)> multipler;

    inline void updateNode(Node<T>* current, int leftBound, int rightBound, T value) {
        current -> data = adder(current -> data, multipler(value, rightBound - leftBound + 1));
        current -> tag = adder(current -> tag, value);
    }

    inline void spread(Node<T>* current, int leftBound, int rightBound) {
        if (current -> tag != zeroElement) {
            int mid = (leftBound + rightBound) / 2;
            if (current -> leftSon == nullptr) current -> leftSon = new Node<T>(zeroElement);
            updateNode(current -> leftSon, leftBound, mid, current -> tag);
            if (current -> rightSon == nullptr) current -> rightSon = new Node<T>(zeroElement);
            updateNode(current -> rightSon, mid + 1, rightBound, current -> tag);
            current -> tag = zeroElement;
        }
    }

    void insert(Node<T>*& current, int leftBound, int rightBound, T value) {
        if (current == nullptr) current = new Node<T>(zeroElement);
        current -> data = adder(current -> data, value);
        if (leftBound >= rightBound) return;
        int mid = (leftBound + rightBound) / 2;
        if (leftRange <= mid) insert(current -> leftSon, leftBound, mid, value);
        if (rightRange > mid) insert(current -> rightSon, mid + 1, rightBound, value);
    }

    void update(Node<T>* current, int leftBound, int rightBound, T value) {
        if (leftRange <= leftBound && rightBound <= rightRange) {
            updateNode(current, leftBound, rightBound, value);
            return;
        }
        spread(current, leftBound, rightBound);
        int mid = (leftBound + rightBound) / 2;
        if (leftRange <= mid) update(current -> leftSon, leftBound, mid, value);
        if (rightRange > mid) update(current -> rightSon, mid + 1, rightBound, value);
        current -> data = adder(current -> leftSon -> data, current -> rightSon -> data);
    }

    T query(Node<T>* current, int leftBound, int rightBound) {
        if (leftRange <= leftBound && rightBound <= rightRange) return current -> data;
        spread(current, leftBound, rightBound);
        T result = zeroElement;
        int mid = (leftBound + rightBound) / 2;
        if (leftRange <= mid) result = adder(result, query(current -> leftSon, leftBound, mid));
        if (rightRange > mid) result = adder(result, query(current -> rightSon, mid + 1, rightBound));
        return result;
    }

public:
    SegmentTree(int size, T zeroElement, std::function<T(T, T)> adder, std::function<T(T, int)> multipler) {
        this -> size = size;
        this -> adder = adder;
        this -> root = nullptr;
        this -> multipler = multipler;
        this -> zeroElement = zeroElement;
    }

    inline void insert(int index, T value) {
        this -> leftRange = this -> rightRange = index;
        insert(root, 1, size, value);
    }

    inline void update(int left, int right, T value) {
        this -> leftRange = left, this -> rightRange = right;
        update(root, 1, size, value);
    }

    inline T query(int index) {
        this -> leftRange = this -> rightRange = index;
        return query(root, 1, size);
    }

    inline T query(int left, int right) {
        this -> leftRange = left, this -> rightRange = right;
        return query(root, 1, size);
    }
};

#endif //TREESTRUCTURE_SEGMENTTREE_H
