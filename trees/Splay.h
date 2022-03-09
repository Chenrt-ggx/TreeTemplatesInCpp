#ifndef TREESTRUCTURE_SPLAY_H
#define TREESTRUCTURE_SPLAY_H

#include "../utils/BalancedTree.h"
#include "../utils/NoSuchElementException.h"

template <typename T> class Splay : public BalancedTree<T> {
private:
    template <typename S> class Node {
    public:
        S data;
        int size, repeat;

        Node<S>* father;
        Node<S>* leftSon;
        Node<S>* rightSon;

        explicit Node(S data, Node<S>* father) {
            this -> data = data;
            this -> father = father;
            size = repeat = 1;
            leftSon = rightSon = nullptr;
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

    inline void getNode(Node<T>*& father, Node<T>*& current, T data) {
        father = nullptr;
        while (current != nullptr) {
            father = current;
            if (data < current -> data) current = current -> leftSon;
            else if (data > current -> data) current = current -> rightSon;
            else return;
        }
    }

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

    inline void relink(Node<T>* current, Node<T>* relink) {
        if (relink != nullptr) relink -> father = current -> father;
        if (current -> father == nullptr) root = relink;
        else if (current == current -> father -> leftSon) current -> father -> leftSon = relink;
        else current -> father -> rightSon = relink;
    }

    inline void leftRotate(Node<T>* current) {
        if (current != nullptr) {
            Node<T>* node = current -> rightSon;
            current -> rightSon = node -> leftSon;
            if (node -> leftSon != nullptr) node -> leftSon -> father = current;
            relink(current, node);
            node -> leftSon = current;
            current -> father = node;
            current -> pushUp(), node -> pushUp();
        }
    }

    inline void rightRotate(Node<T>* current) {
        if (current != nullptr) {
            Node<T>* node = current -> leftSon;
            current -> leftSon = node -> rightSon;
            if (node -> rightSon != nullptr) node -> rightSon -> father = current;
            relink(current, node);
            node -> rightSon = current;
            current -> father = node;
            current -> pushUp(), node -> pushUp();
        }
    }

    void splay(Node<T>* src, Node<T>* dst) {
        while (src != nullptr && src -> father != nullptr && src -> father != dst) {
            if (src -> father -> father != dst) {
                if (src -> father == src -> father -> father -> leftSon) {
                    if (src == src -> father -> leftSon) rightRotate(src -> father -> father);
                    else leftRotate(src -> father);
                }
                else {
                    if (src == src -> father -> leftSon) rightRotate(src -> father);
                    else leftRotate(src -> father -> father);
                }
            }
            if (src == src -> father -> leftSon) rightRotate(src -> father);
            else leftRotate(src -> father);
        }
        if (dst == nullptr) root = src;
    }

    void clear(Node<T>*& current) {
        if (current == nullptr) return;
        clear(current -> leftSon);
        clear(current -> rightSon);
        delete current;
        current = nullptr;
    }

public:
    Splay() {
        root = nullptr;
    }

    void clear() {
        clear(root);
    }

    const char* implement() {
        return "Splay";
    }

    void insert(T data) {
        Node<T>* father = nullptr;
        Node<T>* current = root;
        while (current != nullptr) {
            father = current, father -> size++;
            if (data < current -> data) current = current -> leftSon;
            else if (data > current -> data) current = current -> rightSon;
            else {
                splay(current, root -> father);
                current -> repeat++;
                return;
            }
        }
        current = new Node<T>(data, father);
        if (father == nullptr) root = current;
        else if (data < father -> data) father -> leftSon = current;
        else father -> rightSon = current;
        splay(current, root -> father);
    }

    void remove(T data) {
        Node<T> *father, *current = root;
        getNode(father, current, data);
        if (current == nullptr) splay(father, root -> father);
        else {
            splay(current, root -> father);
            if (root -> repeat > 1) {
                root -> repeat--;
                return;
            }
        }
        current = root;
        if (root -> leftSon == nullptr) relink(root, root -> rightSon);
        else if (root -> rightSon == nullptr) relink(root, root -> leftSon);
        else {
            Node<T>* node = getMinNode(current -> rightSon);
            if (node -> father != current) {
                for (Node<T>* i = node; i != current; i = i -> father)
                    i -> size -= node -> repeat;
                relink(node, node -> rightSon);
                node -> rightSon = current -> rightSon;
                node -> rightSon -> father = node;
            }
            relink(current, node);
            node -> leftSon = current -> leftSon;
            node -> leftSon -> father = node;
            node -> pushUp();
        }
        delete current;
    }

    int rank(T data) {
        Node<T> *father, *current = root;
        getNode(father, current, data);
        if (current != nullptr) {
            splay(current, root -> father);
            return root -> countSmaller() + 1;
        }
        else if (father != nullptr) {
            splay(father, root -> father);
            if (data > root -> data) return root -> countSmaller() + root -> repeat + 1;
            return root -> countSmaller() + 1;
        }
        else return 1;
    }

    T select(int rank) {
        int value = rank;
        Node<T>* father = nullptr;
        Node<T>* current = root;
        while (current != nullptr) {
            father = current;
            if (current -> countSmaller() + 1 <= value &&
                value <= current -> countSmaller() + current -> repeat) {
                splay(current, root -> father);
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
        if (father != nullptr) splay(father, root -> father);
        throw NoSuchElementException();
    }

    T predecessor(T data) {
        Node<T> *father, *current = root;
        getNode(father, current, data);
        if (current != nullptr) {
            splay(current, root -> father);
            return getMaxNode(root -> leftSon) -> data;
        }
        else if (father != nullptr) {
            splay(father, root -> father);
            if (data > root -> data) return root -> data;
            return getMaxNode(root -> leftSon) -> data;
        }
        else throw NoSuchElementException();
    }

    T successor(T data) {
        Node<T> *father, *current = root;
        getNode(father, current, data);
        if (current != nullptr) {
            splay(current, root -> father);
            return getMinNode(root -> rightSon) -> data;
        }
        else if (father != nullptr) {
            splay(father, root -> father);
            if (data < root -> data) return root -> data;
            return getMinNode(root -> rightSon) -> data;
        }
        else throw NoSuchElementException();
    }
};

#endif //TREESTRUCTURE_SPLAY_H
