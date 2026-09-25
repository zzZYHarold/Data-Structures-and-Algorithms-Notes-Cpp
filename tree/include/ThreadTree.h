#pragma once

#include <cstddef>

// 5.4 线索化二叉树
// 依据 2025 秋课件整理；当前 2026 part1 尚未覆盖本节。

template <class T>
struct ThreadNode {
    int ltag;
    int rtag;
    ThreadNode<T>* leftChild;
    ThreadNode<T>* rightChild;
    T data;

    explicit ThreadNode(const T& item)
        : ltag(0),
          rtag(0),
          leftChild(nullptr),
          rightChild(nullptr),
          data(item) {}
};

template <class T>
class ThreadTree {
public:
    ThreadTree()
        : root(nullptr) {}

    explicit ThreadTree(ThreadNode<T>* r)
        : root(r) {}

    ~ThreadTree() {
        destroy(root);
    }

    ThreadNode<T>* getRoot() const {
        return root;
    }

    // 为了便于和课件中的结点结构配合，补充根指针设置接口。
    // ThreadTree 接管传入树的所有权。
    void setRoot(ThreadNode<T>* r) {
        if (root != r) {
            destroy(root);
            root = r;
        }
    }

    // 建立中序线索二叉树。
    void createInThread() {
        ThreadNode<T>* pre = nullptr;
        if (root != nullptr) {
            createInThread(root, pre);
            pre->rightChild = nullptr;
            pre->rtag = 1;
        }
    }

    // 寻找中序序列下第一个结点。
    ThreadNode<T>* First(ThreadNode<T>* current) const {
        if (current == nullptr) {
            return nullptr;
        }

        while (current->ltag == 0 && current->leftChild != nullptr) {
            current = current->leftChild;
        }
        return current;
    }

    // 寻找中序序列下最后一个结点。
    ThreadNode<T>* Last(ThreadNode<T>* current) const {
        if (current == nullptr) {
            return nullptr;
        }

        while (current->rtag == 0 && current->rightChild != nullptr) {
            current = current->rightChild;
        }
        return current;
    }

    // 寻找当前结点在中序序列下的后继。
    ThreadNode<T>* Next(ThreadNode<T>* current) const {
        if (current == nullptr) {
            return nullptr;
        }

        if (current->rtag == 1) {
            return current->rightChild;
        }
        return First(current->rightChild);
    }

    // 寻找当前结点在中序序列下的前驱。
    ThreadNode<T>* Prior(ThreadNode<T>* current) const {
        if (current == nullptr) {
            return nullptr;
        }

        if (current->ltag == 1) {
            return current->leftChild;
        }
        return Last(current->leftChild);
    }

    ThreadNode<T>* parent(ThreadNode<T>* t) const {
        if (t == nullptr || t == root) {
            return nullptr;
        }
        return parent(root, t);
    }

    void inOrder(void (*visit)(ThreadNode<T>*)) const {
        for (ThreadNode<T>* p = First(root); p != nullptr; p = Next(p)) {
            visit(p);
        }
    }

private:
    ThreadNode<T>* root;

    void createInThread(ThreadNode<T>* current,
                        ThreadNode<T>*& pre) {
        if (current == nullptr) {
            return;
        }

        // 在线索化之前 ltag/rtag 均为 0，因此这里沿真实左子树递归。
        createInThread(current->leftChild, pre);

        if (current->leftChild == nullptr) {
            current->leftChild = pre;
            current->ltag = 1;
        }

        if (pre != nullptr && pre->rightChild == nullptr) {
            pre->rightChild = current;
            pre->rtag = 1;
        }

        pre = current;
        createInThread(current->rightChild, pre);
    }

    ThreadNode<T>* parent(ThreadNode<T>* current,
                          ThreadNode<T>* target) const {
        if (current == nullptr) {
            return nullptr;
        }

        if (current->ltag == 0 && current->leftChild == target) {
            return current;
        }
        if (current->rtag == 0 && current->rightChild == target) {
            return current;
        }

        ThreadNode<T>* p = nullptr;
        if (current->ltag == 0) {
            p = parent(current->leftChild, target);
            if (p != nullptr) {
                return p;
            }
        }

        if (current->rtag == 0) {
            return parent(current->rightChild, target);
        }
        return nullptr;
    }

    void destroy(ThreadNode<T>* current) {
        if (current == nullptr) {
            return;
        }

        if (current->ltag == 0) {
            destroy(current->leftChild);
        }
        if (current->rtag == 0) {
            destroy(current->rightChild);
        }
        delete current;
    }
};
