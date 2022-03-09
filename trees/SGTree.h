#ifndef TREESTRUCTURE_SGTREE_H
#define TREESTRUCTURE_SGTREE_H

#include <vector>
#include "../utils/BalancedTree.h"
#include "../utils/NoSuchElementException.h"

template <typename T> class SGTree : public BalancedTree<T> {
private:
    constexpr static double ALPHA = 0.8;
    constexpr static double BETA = 0.6;

    template <typename S> class Node {
    public:
        S data;
        Node<S>* leftSon;
        Node<S>* rightSon;
        int size, real, unique, repeat;

        explicit Node(S data) {
            this -> data = data;
            leftSon = rightSon = nullptr;
            size = real = unique = repeat = 1;
        }

        inline void enlarge() {
            size++, unique++, repeat++;
        }

        inline void reduce() {
            if (repeat > 0) size--, unique--, repeat--;
        }

        inline int countSmaller() {
            return leftSon == nullptr ? 0 : leftSon -> size;
        }

        inline bool unbalance() {
            return (leftSon != nullptr && leftSon -> real > (int)(real * ALPHA) + 10)
                   || (rightSon != nullptr && rightSon -> real > (int)(real * ALPHA) + 10)
                   || (unique < (int)(real * BETA) - 10);
        }

        inline void pushUp() {
            size = repeat, real = 1, unique = repeat != 0;
            if (leftSon != nullptr)
                size += leftSon -> size, real += leftSon -> real, unique += leftSon -> unique;
            if (rightSon != nullptr)
                size += rightSon -> size, real += rightSon -> real, unique += rightSon -> unique;
        }
    };

    Node<T>* root;
    Node<T>** rebuildNode;

    void serialize(Node<T>* current, std::vector<Node<T>*>& buffer) {
        if (current == nullptr) return;
        serialize(current -> leftSon, buffer);
        if (current -> repeat) buffer.push_back(current);
        else delete current;
        serialize(current -> rightSon, buffer);
    }

    Node<T>* unSerialize(int left, int right, std::vector<Node<T>*>& buffer) {
        if (left >= right) return nullptr;
        int mid = (left + right) / 2;
        Node<T>* current = buffer[mid];
        current -> leftSon = unSerialize(left, mid, buffer);
        current -> rightSon = unSerialize(mid + 1, right, buffer);
        current -> pushUp();
        return current;
    }

    inline void rebuild() {
        std::vector<Node<T>*> buffer;
        serialize(*rebuildNode, buffer);
        *rebuildNode = unSerialize(0, buffer.size(), buffer);
        rebuildNode = nullptr, buffer.clear();
    }

    int count(T data, int close) {
        int rank = 0;
        Node<T>* current = root;
        while (current != nullptr) {
            if (data == current -> data) {
                rank += current -> countSmaller() + (close ? current -> repeat : 0);
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

    void modify(Node<T>*& current, T data, bool insert) {
        if (current == nullptr) {
            if (insert) current = new Node<T>(data);
            return;
        }
        else if (data < current -> data)
            modify(current -> leftSon, data, insert), current -> pushUp();
        else if (data > current -> data)
            modify(current -> rightSon, data, insert), current -> pushUp();
        else if (insert) current -> enlarge();
        else current -> reduce();
        if (current -> unbalance()) rebuildNode = &current;
    }

    void clear(Node<T>*& current) {
        if (current == nullptr) return;
        clear(current -> leftSon);
        clear(current -> rightSon);
        delete current;
        current = nullptr;
    }

public:
    SGTree<T>() {
        root = nullptr, rebuildNode = nullptr;
    }

    void clear() {
        clear(root);
    }

    const char* implement() {
        return "Scape Goat Tree";
    }

    void insert(T data) {
        modify(root, data, true);
        if (rebuildNode != nullptr) rebuild();
    }

    void remove(T data) {
        modify(root, data, false);
        if (rebuildNode != nullptr) rebuild();
    }

    int rank(T data) {
        return count(data, false) + 1;
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
        return select(count(data, false));
    }

    T successor(T data) {
        return select(count(data, true) + 1);
    }
};

#endif //TREESTRUCTURE_SGTREE_H
