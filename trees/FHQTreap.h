#ifndef TREESTRUCTURE_FHQTREAP_H
#define TREESTRUCTURE_FHQTREAP_H

#include <random>
#include <chrono>
#include "../utils/BalancedTree.h"
#include "../utils/NoSuchElementException.h"

template <typename T> class FHQTreap : public BalancedTree<T> {
private:
    template <typename S> class Node {
    public:
        S data;
        Node<S>* leftSon;
        Node<S>* rightSon;
        int size, repeat, index{};

        explicit Node(S data, int index) {
            this -> data = data;
            this -> index = index;
            size = repeat = 1;
            leftSon = rightSon = nullptr;
        }

        inline void enlarge() {
            size++, repeat++;
        }

        inline void reduce() {
            size--, repeat--;
        }

        inline int countSmaller() {
            return leftSon == nullptr ? 0 : leftSon -> size;
        }

        inline void pushUp() {
            size = repeat;
            if (leftSon != nullptr) size += leftSon -> size;
            if (rightSon != nullptr) size += rightSon -> size;
        }
    };

    Node<T>* root;
    std::mt19937 generator;

    inline Node<T>* getMinNode(Node<T>* current) {
        if (current == nullptr) throw NoSuchElementException();
        while (current -> leftSon != nullptr) current = current -> leftSon;
        return current;
    }

    inline Node<T>* getMaxNode(Node<T>* current) {
        if (current == nullptr) throw NoSuchElementException();
        while (current -> rightSon != nullptr) current = current -> rightSon;
        return current;
    }

    void splitByKey(Node<T>*& left, Node<T>*& right, Node<T>* current, T data, bool close) {
        if (current == nullptr) {
            left = right = nullptr;
            return;
        }
        else if ((!close && data <= current -> data) || (close && data < current -> data)) {
            right = current;
            splitByKey(left, current -> leftSon, current -> leftSon, data, close);
        }
        else {
            left = current;
            splitByKey(current -> rightSon, right, current -> rightSon, data, close);
        }
        current -> pushUp();
    }

    void splitBySize(Node<T>*& left, Node<T>*& right, Node<T>* current, int size) {
        if (current == nullptr) {
            left = right = nullptr;
            return;
        }
        else if (size <= 0) {
            left = nullptr, right = current;
            return;
        }
        else if (size >= current -> size) {
            left = current, right = nullptr;
            return;
        }
        else if (size <= current -> countSmaller()) {
            right = current;
            splitBySize(left, current -> leftSon, current -> leftSon, size);
        }
        else {
            left = current;
            splitBySize(current -> rightSon, right, current -> rightSon,
                    size - current -> countSmaller() - current -> repeat);
        }
        current -> pushUp();
    }

    Node<T>* merge(Node<T>* leftRoot, Node<T>* rightRoot) {
        if (leftRoot == nullptr) return rightRoot;
        if (rightRoot == nullptr) return leftRoot;
        if (leftRoot -> index < rightRoot -> index) {
            leftRoot -> rightSon = merge(leftRoot -> rightSon, rightRoot);
            leftRoot -> pushUp();
            return leftRoot;
        }
        else {
            rightRoot -> leftSon = merge(leftRoot, rightRoot -> leftSon);
            rightRoot -> pushUp();
            return rightRoot;
        }
    }

    void clear(Node<T>*& current) {
        if (current == nullptr) return;
        clear(current -> leftSon);
        clear(current -> rightSon);
        delete current;
        current = nullptr;
    }

public:
    FHQTreap<T>() {
        generator = *(new std::mt19937(std::chrono::system_clock
                ::now().time_since_epoch().count()));
        root = nullptr;
    }

    void clear() {
        clear(root);
    }

    const char* implement() {
        return "FHQ Treap";
    }

    void insert(T data) {
        Node<T> *left, *mid, *right;
        splitByKey(left, mid, root, data, false);
        splitByKey(mid, right, mid, data, true);
        if (mid != nullptr) {
            mid -> enlarge(), root = merge(left, merge(mid, right));
        }
        else {
            root = merge(left, merge(new Node<T>(data, generator()), right));
        }
    }

    void remove(T data) {
        Node<T> *left, *mid, *right;
        splitByKey(left, mid, root, data, false);
        splitByKey(mid, right, mid, data, true);
        if (mid != nullptr && mid -> repeat > 1) {
            mid -> reduce(), root = merge(left, merge(mid, right));
        }
        else {
            if (mid != nullptr) delete mid;
            root = merge(left, right);
        }
    }

    int rank(T data) {
        Node<T> *left, *right;
        splitByKey(left, right, root, data, false);
        int rank = left == nullptr ? 1 : left -> size + 1;
        root = merge(left, right);
        return rank;
    }

    T select(int rank) {
        Node<T> *left, *right;
        splitBySize(left, right, root, rank);
        if (left == nullptr) {
            root = merge(left, right);
            throw NoSuchElementException();
        }
        else {
            T data = getMaxNode(left) -> data;
            root = merge(left, right);
            return data;
        }
    }

    T predecessor(T data) {
        Node<T> *left, *right;
        splitByKey(left, right, root, data, false);
        if (left == nullptr) {
            root = merge(left, right);
            throw NoSuchElementException();
        }
        else {
            T predecessor = getMaxNode(left) -> data;
            root = merge(left, right);
            return predecessor;
        }
    }

    T successor(T data) {
        Node<T> *left, *right;
        splitByKey(left, right, root, data, true);
        if (right == nullptr) {
            root = merge(left, right);
            throw NoSuchElementException();
        }
        else {
            T successor = getMinNode(right) -> data;
            root = merge(left, right);
            return successor;
        }
    }
};

#endif //TREESTRUCTURE_FHQTREAP_H
