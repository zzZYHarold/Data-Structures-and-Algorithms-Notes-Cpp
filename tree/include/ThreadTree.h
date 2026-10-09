#pragma once

#include <functional>

// 5.4 线索化二叉树
// 依据 2026 秋完整课件第 90-121 页整理。

template <class T>
struct ThreadNode {
    int ltag;  // 0：左子女；1：中序前驱线索
    int rtag;  // 0：右子女；1：中序后继线索
    ThreadNode<T>* leftChild;
    ThreadNode<T>* rightChild;
    T data;

    explicit ThreadNode(const T& item)
        : ltag(0), rtag(0), leftChild(nullptr), rightChild(nullptr), data(item) {}
};

template <class T>
class ThreadTree {
public:
    ThreadTree() : root(nullptr) {}

    explicit ThreadTree(ThreadNode<T>* rootNode) : root(rootNode) {}

    // 本类拥有结点；课件未给出复制算法，禁止默认浅拷贝。
    ThreadTree(const ThreadTree&) = delete;
    ThreadTree& operator=(const ThreadTree&) = delete;

    ~ThreadTree() { destroy(root); }

    ThreadNode<T>* getRoot() const {
        return root;
    }

    // 为了便于和课件中的结点结构配合，补充根指针设置接口。
    // ThreadTree 接管传入树的所有权。
    void setRoot(ThreadNode<T>* rootNode) {
        if (root != rootNode) {
            destroy(root);
            root = rootNode;
        }
    }

    // 建立中序线索二叉树。
    void createInThread() {
        ThreadNode<T>* previousNode = nullptr;
        if (root != nullptr) {
            createInThread(root, previousNode);
            previousNode->rightChild = nullptr;
            previousNode->rtag = 1;
        }
    }

    // 寻找中序序列下第一个结点。
    ThreadNode<T>* First(ThreadNode<T>* current) const {
        if (current == nullptr)
            return nullptr;

        while (current->ltag == 0 && current->leftChild != nullptr)
            current = current->leftChild;

        return current;
    }

    // 寻找中序序列下最后一个结点。
    ThreadNode<T>* Last(ThreadNode<T>* current) const {
        if (current == nullptr)
            return nullptr;

        while (current->rtag == 0 && current->rightChild != nullptr)
            current = current->rightChild;

        return current;
    }

    // 寻找当前结点在中序序列下的后继。
    ThreadNode<T>* Next(ThreadNode<T>* current) const {
        if (current == nullptr)
            return nullptr;

        if (current->rtag == 1)
            return current->rightChild;

        return First(current->rightChild);
    }

    // 寻找当前结点在中序序列下的前驱。
    ThreadNode<T>* Prior(ThreadNode<T>* current) const {
        if (current == nullptr)
            return nullptr;

        if (current->ltag == 1)
            return current->leftChild;

        return Last(current->leftChild);
    }

    // 第 118-120 页练习：通过两条线索路径查找双亲。
    // targetNode 必须属于本树，且本树已经完成中序线索化。
    ThreadNode<T>* parent(ThreadNode<T>* targetNode) const {
        if (targetNode == nullptr || targetNode == root)
            return nullptr;

        // 路径一：子树中序第一个结点的前驱，再沿真实右子女寻找。
        ThreadNode<T>* candidateParent = First(targetNode)->leftChild;
        while (candidateParent != nullptr && !isParentOf(candidateParent, targetNode)
               && candidateParent->rtag == 0) {
            candidateParent = candidateParent->rightChild;
        }
        if (isParentOf(candidateParent, targetNode))
            return candidateParent;

        // 路径二：子树中序最后一个结点的后继，再沿真实左子女寻找。
        candidateParent = Last(targetNode)->rightChild;
        while (candidateParent != nullptr && !isParentOf(candidateParent, targetNode)
               && candidateParent->ltag == 0) {
            candidateParent = candidateParent->leftChild;
        }
        return isParentOf(candidateParent, targetNode) ? candidateParent : nullptr;
    }

    // 补充：利用课件 First/Next 接口串联中序遍历。
    void inOrder(const std::function<void(ThreadNode<T>*)>& visit) const {
        for (ThreadNode<T>* node = First(root); node != nullptr; node = Next(node)) {
            visit(node);
        }
    }

private:
    ThreadNode<T>* root;

    void createInThread(ThreadNode<T>* current, ThreadNode<T>*& previousNode) {
        if (current == nullptr)
            return;

        // 仅沿真实子女递归，重复调用时也不会把已有线索当作子树。
        if (current->ltag == 0)
            createInThread(current->leftChild, previousNode);

        if (current->leftChild == nullptr) {
            current->leftChild = previousNode;
            current->ltag = 1;
        }

        if (previousNode != nullptr && previousNode->rightChild == nullptr) {
            previousNode->rightChild = current;
            previousNode->rtag = 1;
        }

        previousNode = current;
        if (current->rtag == 0)
            createInThread(current->rightChild, previousNode);
    }

    // 仅真实子女指针能证明双亲关系，前驱/后继线索不算子女。
    static bool isParentOf(ThreadNode<T>* candidateParent, ThreadNode<T>* targetNode) {
        return candidateParent != nullptr
            && ((candidateParent->ltag == 0 && candidateParent->leftChild == targetNode)
                || (candidateParent->rtag == 0 && candidateParent->rightChild == targetNode));
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
