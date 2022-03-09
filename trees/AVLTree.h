#ifndef TREESTRUCTURE_AVLTREE_H
#define TREESTRUCTURE_AVLTREE_H

#include <algorithm>
#include "../utils/BalancedTree.h"
#include "../utils/NoSuchElementException.h"

template <typename T> class AVLTree : public BalancedTree<T> {
private:
    template <typename S> class Node {
    public:
        S data;
        Node<S>* leftSon;
        Node<S>* rightSon;
        int size, height, repeat;

        explicit Node(S data) {
            this -> data = data;
            size = height = repeat = 1;
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

        inline int leftHeight() {
            return leftSon == nullptr ? 0 : leftSon -> height;
        }

        inline int rightHeight() {
            return rightSon == nullptr ? 0 : rightSon -> height;
        }

        inline void pushUp() {
            height = 1, size = repeat;
            if (leftSon != nullptr)
                size += leftSon -> size, height = std::max(height, leftSon -> height + 1);
            if (rightSon != nullptr)
                size += rightSon -> size, height = std::max(height, rightSon -> height + 1);
        }
    };

    Node<T>* root;

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

    inline void leftRotate(Node<T>*& current) {
        if (current == nullptr) return;
        Node<T>* node = current -> rightSon;
        current -> rightSon = node -> leftSon;
        node -> leftSon = current;
        current -> pushUp(), node -> pushUp();
        current = node;
    }

    inline void rightRotate(Node<T>*& current) {
        if (current == nullptr) return;
        Node<T>* node = current -> leftSon;
        current -> leftSon = node -> rightSon;
        node -> rightSon = current;
        current -> pushUp(), node -> pushUp();
        current = node;
    }

    void maintain(Node<T>*& current) {
        if (current == nullptr) return;
        if (current -> leftHeight() - current -> rightHeight() >= 2) {
            if (current -> leftSon -> leftHeight() <= current -> leftSon -> rightHeight())
                leftRotate(current -> leftSon);
            rightRotate(current);
        }
        if (current -> leftHeight() - current -> rightHeight() <= -2) {
            if (current -> rightSon -> rightHeight() <= current -> rightSon -> leftHeight())
                rightRotate(current -> rightSon);
            leftRotate(current);
        }
    }

    void clear(Node<T>*& current) {
        if (current == nullptr) return;
        clear(current -> leftSon);
        clear(current -> rightSon);
        delete current;
        current = nullptr;
    }

    void insert(Node<T>*& current, T data) {
        if (current == nullptr) current = new Node<T>(data);
        else if (data < current -> data)
            insert(current -> leftSon, data), current -> pushUp(), maintain(current);
        else if (data > current -> data)
            insert(current -> rightSon, data), current -> pushUp(), maintain(current);
        else current -> enlarge();
    }

    void remove(Node<T>*& current, T data) {
        if (current == nullptr) return;
        else if (data < current -> data)
            remove(current -> leftSon, data), current -> pushUp();
        else if (data > current -> data)
            remove(current -> rightSon, data), current -> pushUp();
        else if (current -> repeat > 1) current -> reduce();
        else if (current -> leftSon == nullptr) {
            Node<T>* temp = current;
            current = current -> rightSon;
            delete temp;
        }
        else if (current -> rightSon == nullptr) {
            Node<T>* temp = current;
            current = current -> leftSon;
            delete temp;
        }
        else {
            Node<T>* replace = getMinNode(current -> rightSon);
            current -> data = replace -> data;
            current -> repeat = replace -> repeat, replace -> repeat = 1;
            remove(current -> rightSon, current -> data);
            current -> pushUp(), maintain(current);
        }
    }

public:
    AVLTree<T>() {
        root = nullptr;
    }

    void clear() {
        clear(root);
    }

    const char* implement() {
        return "AVL Tree";
    }

    void insert(T data) {
        insert(root, data);
    }

    void remove(T data) {
        remove(root, data);
    }

    int rank(T data) {
        int rank = 1;
        Node<T>* current = root;
        while (current != nullptr) {
            if (data == current -> data) {
                rank += current -> countSmaller();
                break;
            }
            else if (data < current -> data) {
                current = current -> leftSon;
            }
            else {
                rank += current -> countSmaller() + current -> repeat;
                current = current -> rightSon;
            }
        }
        return rank;
    }

    T select(int rank) {
        int value = rank;
        Node<T>* current = root;
        while (current != nullptr) {
            if (current -> countSmaller() + 1 <= value &&
                value <= current -> countSmaller() + current -> repeat) {
                return current -> data;
            }
            else if (value < current -> countSmaller() + 1) {
                current = current -> leftSon;
            }
            else {
                value -= current -> countSmaller() + current -> repeat;
                current = current -> rightSon;
            }
        }
        throw NoSuchElementException();
    }

    T predecessor(T data) {
        Node<T>* current = root;
        Node<T>* node = getMinNode(root);
        if (data <= node -> data) throw NoSuchElementException();
        while (current != nullptr) {
            if (data > current -> data) {
                if (node -> data < current -> data) node = current;
                current = current -> rightSon;
            }
            else current = current -> leftSon;
        }
        return node -> data;
    }

    T successor(T data) {
        Node<T>* current = root;
        Node<T>* node = getMaxNode(root);
        if (data >= node -> data) throw NoSuchElementException();
        while (current != nullptr) {
            if (data < current -> data) {
                if (node -> data > current -> data) node = current;
                current = current -> leftSon;
            }
            else current = current -> rightSon;
        }
        return node -> data;
    }
};

#endif //TREESTRUCTURE_AVLTREE_H
