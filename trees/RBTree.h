#ifndef TREESTRUCTURE_RBTREE_H
#define TREESTRUCTURE_RBTREE_H

#include "../utils/BalancedTree.h"
#include "../utils/NoSuchElementException.h"

template <typename T> class RBTree : public BalancedTree<T> {
private:
    template <typename S> class Node {
    public:
        S data;
        bool color;
        int size, repeat;

        Node<S>* father;
        Node<S>* leftSon;
        Node<S>* rightSon;

        explicit Node(S data, Node<S>* father) {
            this -> data = data;
            this -> father = father;
            color = false;
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

    inline Node<T>* parentOf(Node<T>* current) {
        return current == nullptr ? nullptr : current -> father;
    }

    inline Node<T>* leftOf(Node<T>* current) {
        return current == nullptr ? nullptr : current -> leftSon;
    }

    inline Node<T>* rightOf(Node<T>* current) {
        return current == nullptr ? nullptr : current -> rightSon;
    }

    inline bool getColor(Node<T>* current) {
        return current == nullptr ? false : current -> color;
    }

    inline void setColor(Node<T>* current, bool color) {
        if (current != nullptr) current -> color = color;
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

    void fixInsert(Node<T>* current) {
        current -> color = true;
        while (current != nullptr && current != root && current -> father -> color) {
            if (parentOf(current) == leftOf(parentOf(parentOf(current)))) {
                Node<T>* uncle = rightOf(parentOf(parentOf(current)));
                if (getColor(uncle)) {
                    setColor(parentOf(current), false);
                    setColor(uncle, false);
                    setColor(parentOf(parentOf(current)), true);
                    current = parentOf(parentOf(current));
                }
                else {
                    if (current == rightOf(parentOf(current)))
                        current = parentOf(current), leftRotate(current);
                    setColor(parentOf(current), false);
                    setColor(parentOf(parentOf(current)), true);
                    rightRotate(parentOf(parentOf(current)));
                }
            }
            else {
                Node<T>* uncle = leftOf(parentOf(parentOf(current)));
                if (getColor(uncle)) {
                    setColor(parentOf(current), false);
                    setColor(uncle, false);
                    setColor(parentOf(parentOf(current)), true);
                    current = parentOf(parentOf(current));
                }
                else {
                    if (current == leftOf(parentOf(current)))
                        current = parentOf(current), rightRotate(current);
                    setColor(parentOf(current), false);
                    setColor(parentOf(parentOf(current)), true);
                    leftRotate(parentOf(parentOf(current)));
                }
            }
        }
        root -> color = false;
    }

    void fixRemove(Node<T>* current) {
        while (current != nullptr && current != root && !getColor(current)) {
            if (current == leftOf(parentOf(current))) {
                Node<T>* cousin = rightOf(parentOf(current));
                if (getColor(cousin)) {
                    setColor(parentOf(current), true);
                    setColor(cousin, false);
                    leftRotate(parentOf(current));
                    cousin = rightOf(parentOf(current));
                }
                if (!getColor(leftOf(cousin)) && !getColor(rightOf(cousin))) {
                    setColor(cousin, true), current = parentOf(current);
                }
                else {
                    if (!getColor(rightOf(cousin))) {
                        setColor(leftOf(cousin), false);
                        setColor(cousin, true);
                        rightRotate(cousin);
                        cousin = rightOf(parentOf(current));
                    }
                    setColor(cousin, getColor(parentOf(current)));
                    setColor(parentOf(current), false);
                    setColor(rightOf(cousin), false);
                    leftRotate(parentOf(current));
                    current = root;
                }
            }
            else {
                Node<T>* cousin = leftOf(parentOf(current));
                if (getColor(cousin)) {
                    setColor(parentOf(current), true);
                    setColor(cousin, false);
                    rightRotate(parentOf(current));
                    cousin = leftOf(parentOf(current));
                }
                if (!getColor(rightOf(cousin)) && !getColor(leftOf(cousin))) {
                    setColor(cousin, true), current = parentOf(current);
                }
                else {
                    if (!getColor(leftOf(cousin))) {
                        setColor(rightOf(cousin), false);
                        setColor(cousin, true);
                        leftRotate(cousin);
                        cousin = leftOf(parentOf(current));
                    }
                    setColor(cousin, getColor(parentOf(current)));
                    setColor(parentOf(current), false);
                    setColor(leftOf(cousin), false);
                    rightRotate(parentOf(current));
                    current = root;
                }
            }
        }
        setColor(current, false);
    }

    void clear(Node<T>*& current) {
        if (current == nullptr) return;
        clear(current -> leftSon);
        clear(current -> rightSon);
        delete current;
        current = nullptr;
    }

public:
    RBTree() {
        root = nullptr;
    }

    void clear() {
        clear(root);
    }

    const char* implement() {
        return "Red Black Tree";
    }

    void insert(T data) {
        Node<T>* father = nullptr;
        Node<T>* current = root;
        while (current != nullptr) {
            father = current, father -> size++;
            if (data < current -> data) current = current -> leftSon;
            else if (data > current -> data) current = current -> rightSon;
            else {
                current -> repeat++;
                return;
            }
        }
        current = new Node<T>(data, father);
        if (father == nullptr) root = current;
        else if (data < father -> data) father -> leftSon = current;
        else father -> rightSon = current;
        fixInsert(current);
    }

    void remove(T data) {
        Node<T>* father = nullptr;
        Node<T>* current = root;
        while (current != nullptr) {
            father = current, father -> size--;
            if (data < current -> data) current = current -> leftSon;
            else if (data > current -> data) current = current -> rightSon;
            else {
                if (current -> repeat > 1) {
                    current -> repeat--;
                    return;
                }
                break;
            }
        }
        if (current == nullptr) {
            while (father != nullptr) {
                father -> size++;
                father = father -> father;
            }
        }
        else if (current -> leftSon == nullptr) {
            relink(current, current -> rightSon);
            if (!current -> color) fixRemove(current -> rightSon);
            delete current;
        }
        else if (current -> rightSon == nullptr) {
            relink(current, current -> leftSon);
            if (!current -> color) fixRemove(current -> leftSon);
            delete current;
        }
        else {
            Node<T>* node = getMinNode(current -> rightSon);
            Node<T>* fixNode = node -> rightSon;
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
            bool color = node -> color;
            node -> color = current -> color;
            node -> pushUp();
            if (!color) fixRemove(fixNode);
            delete current;
        }
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

#endif //TREESTRUCTURE_RBTREE_H
