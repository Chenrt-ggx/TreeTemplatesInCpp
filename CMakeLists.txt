cmake_minimum_required(VERSION 3.16)
project(TreeStructure)

set(CMAKE_CXX_STANDARD 17)

add_executable(TreeStructure main.cpp
    utils/NoSuchElementException.h
    utils/BalancedTree.h
    utils/Comparator.h
    utils/FastIO.h

    trees/AVLTree.h
    trees/SBTree.h
    trees/Treap.h
    trees/FHQTreap.h
    trees/RBTree.h
    trees/Splay.h
    trees/SGTree.h

    trees/BITree.h
    trees/SegmentTree.h
)
