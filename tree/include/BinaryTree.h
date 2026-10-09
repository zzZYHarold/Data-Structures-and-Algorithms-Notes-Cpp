#pragma once

#include <cctype>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <functional>

// 5.2-5.3 二叉树及遍历
// 依据 2026 秋完整课件第 37-88 页整理。

template <class T>
struct BinTreeNode {
    T data;
    BinTreeNode<T>* leftChild;
    BinTreeNode<T>* rightChild;

    BinTreeNode() : leftChild(nullptr), rightChild(nullptr) {}

    BinTreeNode(const T& item, BinTreeNode<T>* left = nullptr, BinTreeNode<T>* right = nullptr)
        : data(item), leftChild(left), rightChild(right) {}
};

template <class T>
class BinaryTree {
public:
    BinaryTree() : root(nullptr), RefValue() {}

    explicit BinaryTree(const T& refValue)
        : root(nullptr), RefValue(refValue) {}

    BinaryTree(BinTreeNode<T>* leftSubtree, BinTreeNode<T>* rightSubtree,
               const T& item, const T& refValue = T())
        : root(new BinTreeNode<T>(item, leftSubtree, rightSubtree)), RefValue(refValue) {}

    BinaryTree(const BinaryTree<T>& other) : root(Copy(other.root)), RefValue(other.RefValue) {}

    BinaryTree<T>& operator=(const BinaryTree<T>& other) {
        if (this != &other) {
            destroy(root);
            root = Copy(other.root);
            RefValue = other.RefValue;
        }
        return *this;
    }

    ~BinaryTree() { destroy(root); }

    bool IsEmpty() const {
        return root == nullptr;
    }

    int Height() const {
        return Height(root);
    }

    int Size() const {
        return Size(root);
    }

    BinTreeNode<T>* Parent(BinTreeNode<T>* targetNode) const {
        if (targetNode == nullptr || root == nullptr || root == targetNode)
            return nullptr;

        return Parent(root, targetNode);
    }

    BinTreeNode<T>* LeftChild(BinTreeNode<T>* targetNode) const {
        return targetNode != nullptr ? targetNode->leftChild : nullptr;
    }

    BinTreeNode<T>* RightChild(BinTreeNode<T>* targetNode) const {
        return targetNode != nullptr ? targetNode->rightChild : nullptr;
    }

    BinTreeNode<T>* getRoot() const {
        return root;
    }

    // 课件列出了 Find，这里补全为普通二叉树的递归查找。
    BinTreeNode<T>* Find(const T& item) const {
        return Find(root, item);
    }

    // ---------- 递归遍历 ----------
    void preOrder(std::function<void(BinTreeNode<T>*)> visit) const {
        preOrder(root, visit);
    }

    void inOrder(std::function<void(BinTreeNode<T>*)> visit) const {
        inOrder(root, visit);
    }

    void postOrder(std::function<void(BinTreeNode<T>*)> visit) const {
        postOrder(root, visit);
    }

    // ---------- 非递归遍历 ----------
    // 与课件思路一致：前序遍历只把“稍后再访问”的右子树压栈。
    void preOrderNonRecursive(std::function<void(BinTreeNode<T>*)> visit) const {
        if (root == nullptr)
            return;

        std::stack<BinTreeNode<T>*> nodeStack;
        BinTreeNode<T>* currentNode = root;
        nodeStack.push(nullptr);  // 与课件中的空指针哨兵一致

        while (currentNode != nullptr) {
            visit(currentNode);

            if (currentNode->rightChild != nullptr)
                nodeStack.push(currentNode->rightChild);

            if (currentNode->leftChild != nullptr) {
                currentNode = currentNode->leftChild;
            }
            else {
                currentNode = nodeStack.top();
                nodeStack.pop();
            }
        }
    }

    void inOrderNonRecursive(std::function<void(BinTreeNode<T>*)> visit) const {
        std::stack<BinTreeNode<T>*> nodeStack;
        BinTreeNode<T>* currentNode = root;

        do {
            while (currentNode != nullptr) {
                nodeStack.push(currentNode);
                currentNode = currentNode->leftChild;
            }

            if (!nodeStack.empty()) {
                currentNode = nodeStack.top();
                nodeStack.pop();
                visit(currentNode);
                currentNode = currentNode->rightChild;
            }
        } while (currentNode != nullptr || !nodeStack.empty());
    }

    void postOrderNonRecursive(std::function<void(BinTreeNode<T>*)> visit) const {
        enum class Tag { Left, Right };

        struct StackNode {
            BinTreeNode<T>* node;
            Tag tag;
        };

        std::stack<StackNode> nodeStack;
        BinTreeNode<T>* currentNode = root;

        do {
            while (currentNode != nullptr) {
                nodeStack.push({currentNode, Tag::Left});
                currentNode = currentNode->leftChild;
            }

            bool continuePop = true;
            while (continuePop && !nodeStack.empty()) {
                StackNode stackEntry = nodeStack.top();
                nodeStack.pop();
                currentNode = stackEntry.node;

                if (stackEntry.tag == Tag::Left) {
                    // 从左边来的，说明左子树处理完了，去处理右子树
                    stackEntry.tag = Tag::Right;
                    nodeStack.push(stackEntry);
                    continuePop = false;
                    currentNode = currentNode->rightChild;
                }
                else {
                    // 从右边来的，左右都处理完了，访问根节点
                    visit(currentNode);
                }
            }
        } while (!nodeStack.empty());
    }

    // ---------- 层次序遍历 ----------
    // 按 2026 课件写法：先访问，再入队。
    void levelOrder(std::function<void(BinTreeNode<T>*)> visit) const {
        if (root == nullptr)
            return;

        std::queue<BinTreeNode<T>*> pendingNodes;
        BinTreeNode<T>* currentNode = root;
        visit(currentNode);
        pendingNodes.push(currentNode);

        while (!pendingNodes.empty()) {
            currentNode = pendingNodes.front();
            pendingNodes.pop();

            if (currentNode->leftChild != nullptr) {
                visit(currentNode->leftChild);
                pendingNodes.push(currentNode->leftChild);
            }

            if (currentNode->rightChild != nullptr) {
                visit(currentNode->rightChild);
                pendingNodes.push(currentNode->rightChild);
            }
        }
    }

    // ---------- 建树 ----------
    // 以“前序序列 + 空结点标记 RefValue”递归建立二叉树。
    // 例如字符树可用 '@' 作为空结点标记。
    bool createFromPreOrder(std::istream& in) {
        destroy(root);
        root = nullptr;
        return CreateBinTree(in, root);
    }

    // 2026 课件新增：由广义表串建立二叉树。
    // 课件算法按单字符结点值设计，例如 A(B(D,E(G,)),C(,F))#。
    // 因而此接口适用于 T 能由 char 转换得到的情形。
    bool createFromGeneralizedList(const std::string& input) {
        destroy(root);
        root = nullptr;

        std::stack<BinTreeNode<T>*> nodeStack;
        BinTreeNode<T>* currentNode = nullptr;
        int childSide = 0;  // 1: 左子女，2: 右子女

        for (char character : input) {
            if (character == '#') {
                return root != nullptr;
            }
            if (std::isspace(static_cast<unsigned char>(character))) {
                continue;
            }

            switch (character) {
                case '(':
                    if (currentNode == nullptr) {
                        destroy(root);
                        root = nullptr;
                        return false;
                    }
                    nodeStack.push(currentNode);
                    childSide = 1;
                    break;

                case ')':
                    if (nodeStack.empty()) {
                        destroy(root);
                        root = nullptr;
                        return false;
                    }
                    nodeStack.pop();
                    break;

                case ',':
                    childSide = 2;
                    break;

                default: {
                    currentNode = new BinTreeNode<T>(static_cast<T>(character));
                    if (root == nullptr) {
                        root = currentNode;
                    } else {
                        if (nodeStack.empty()) {
                            delete currentNode;
                            destroy(root);
                            root = nullptr;
                            return false;
                        }

                        BinTreeNode<T>* parentNode = nodeStack.top();
                        if (childSide == 1) {
                            parentNode->leftChild = currentNode;
                        } else if (childSide == 2) {
                            parentNode->rightChild = currentNode;
                        } else {
                            delete currentNode;
                            destroy(root);
                            root = nullptr;
                            return false;
                        }
                    }
                    break;
                }
            }
        }

        // 课件规定以 # 结束；没有 # 视为输入不完整。
        destroy(root);
        root = nullptr;
        return false;
    }

    // 2026 课件新增：按广义表形式输出二叉树。
    void printBTree(std::ostream& out = std::cout) const {
        printBTree(root, out);
    }

    // 2026 课件课堂练习：交换所有结点的左右子树。
    void swapChildren() {
        swapChildren(root);
    }

protected:
    BinTreeNode<T>* root;
    T RefValue;  // 数据输入停止标志

    bool CreateBinTree(std::istream& in, BinTreeNode<T>*& subTree) {
        T item;
        if (!(in >> item)) {
            subTree = nullptr;
            return false;
        }

        if (item != RefValue) {
            subTree = new BinTreeNode<T>(item);

            if (!CreateBinTree(in, subTree->leftChild)) {
                destroy(subTree);
                return false;
            }
            if (!CreateBinTree(in, subTree->rightChild)) {
                destroy(subTree);
                return false;
            }
        }
        else {
            subTree = nullptr;
        }
        return true;
    }

    void destroy(BinTreeNode<T>*& subTree) {
        if (subTree != nullptr) {
            destroy(subTree->leftChild);
            destroy(subTree->rightChild);
            delete subTree;
            subTree = nullptr;
        }
    }

    BinTreeNode<T>* Copy(BinTreeNode<T>* sourceRoot) const {
        if (sourceRoot == nullptr)
            return nullptr;

        BinTreeNode<T>* copiedRoot = new BinTreeNode<T>(sourceRoot->data);
        copiedRoot->leftChild = Copy(sourceRoot->leftChild);
        copiedRoot->rightChild = Copy(sourceRoot->rightChild);
        return copiedRoot;
    }

    int Height(BinTreeNode<T>* subTree) const {
        if (subTree == nullptr)
            return 0;

        int leftHeight = Height(subTree->leftChild);
        int rightHeight = Height(subTree->rightChild);
        return (rightHeight > leftHeight ? rightHeight : leftHeight) + 1;
    }

    int Size(BinTreeNode<T>* subTree) const {
        if (subTree == nullptr)
            return 0;
        return Size(subTree->leftChild) + Size(subTree->rightChild) + 1;
    }

    BinTreeNode<T>* Parent(BinTreeNode<T>* subTree, BinTreeNode<T>* targetNode) const {
        if (subTree == nullptr)
            return nullptr;

        if (subTree->leftChild == targetNode || subTree->rightChild == targetNode)
            return subTree;

        BinTreeNode<T>* parentNode = Parent(subTree->leftChild, targetNode);
        if (parentNode != nullptr)
            return parentNode;
        return Parent(subTree->rightChild, targetNode);
    }

    BinTreeNode<T>* Find(BinTreeNode<T>* subTree, const T& item) const {
        if (subTree == nullptr)
            return nullptr;

        if (subTree->data == item)
            return subTree;

        BinTreeNode<T>* foundNode = Find(subTree->leftChild, item);
        if (foundNode != nullptr)
            return foundNode;
        return Find(subTree->rightChild, item);
    }

    void preOrder(BinTreeNode<T>* subTree, std::function<void(BinTreeNode<T>*)> visit) const {
        if (subTree != nullptr) {
            visit(subTree);
            preOrder(subTree->leftChild, visit);
            preOrder(subTree->rightChild, visit);
        }
    }

    void inOrder(BinTreeNode<T>* subTree, std::function<void(BinTreeNode<T>*)> visit) const {
        if (subTree != nullptr) {
            inOrder(subTree->leftChild, visit);
            visit(subTree);
            inOrder(subTree->rightChild, visit);
        }
    }

    void postOrder(BinTreeNode<T>* subTree, std::function<void(BinTreeNode<T>*)> visit) const {
        if (subTree != nullptr) {
            postOrder(subTree->leftChild, visit);
            postOrder(subTree->rightChild, visit);
            visit(subTree);
        }
    }

    void printBTree(BinTreeNode<T>* subTree, std::ostream& out) const {
        if (subTree == nullptr)
            return;

        out << subTree->data;
        if (subTree->leftChild != nullptr || subTree->rightChild != nullptr) {
            out << '(';
            printBTree(subTree->leftChild, out);
            out << ',';
            if (subTree->rightChild != nullptr) {
                printBTree(subTree->rightChild, out);
            }
            out << ')';
        }
    }

    void swapChildren(BinTreeNode<T>* subTree) {
        if (subTree == nullptr)
            return;

        swapChildren(subTree->leftChild);
        swapChildren(subTree->rightChild);

        BinTreeNode<T>* previousLeftChild = subTree->leftChild;
        subTree->leftChild = subTree->rightChild;
        subTree->rightChild = previousLeftChild;
    }
};
