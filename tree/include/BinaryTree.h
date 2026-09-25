#pragma once

#include <cctype>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <functional>

// 5.2-5.3 二叉树及遍历
// 以 2026 秋课件 part1 为主整理。

template <class T>
struct BinTreeNode {
    T data;
    BinTreeNode<T>* leftChild;
    BinTreeNode<T>* rightChild;

    BinTreeNode() : leftChild(nullptr), rightChild(nullptr) {}

    BinTreeNode(const T& x, BinTreeNode<T>* l = nullptr, BinTreeNode<T>* r = nullptr)
        : data(x), leftChild(l), rightChild(r) {}
};

template <class T>
class BinaryTree {
public:
    BinaryTree() : root(nullptr), RefValue() {}

    explicit BinaryTree(const T& refValue)
        : root(nullptr), RefValue(refValue) {}

    BinaryTree(BinTreeNode<T>* lch, BinTreeNode<T>* rch, const T& item, const T& refValue = T())
        : root(new BinTreeNode<T>(item, lch, rch)), RefValue(refValue) {}

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

    BinTreeNode<T>* Parent(BinTreeNode<T>* t) const {
        if (root == nullptr || root == t)
            return nullptr;

        return Parent(root, t);
    }

    BinTreeNode<T>* LeftChild(BinTreeNode<T>* t) const {
        return t != nullptr ? t->leftChild : nullptr;
    }

    BinTreeNode<T>* RightChild(BinTreeNode<T>* t) const {
        return t != nullptr ? t->rightChild : nullptr;
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

        std::stack<BinTreeNode<T>*> s;
        BinTreeNode<T>* p = root;
        s.push(nullptr);  // 与课件中的空指针哨兵一致

        while (p != nullptr) {
            visit(p);

            if (p->rightChild != nullptr)
                s.push(p->rightChild);

            if (p->leftChild != nullptr) {
                p = p->leftChild;
            }
            else {
                p = s.top();
                s.pop();
            }
        }
    }

    void inOrderNonRecursive(std::function<void(BinTreeNode<T>*)> visit) const {
        std::stack<BinTreeNode<T>*> s;
        BinTreeNode<T>* p = root;

        do {
            while (p != nullptr) {
                s.push(p);
                p = p->leftChild;
            }

            if (!s.empty()) {
                p = s.top();
                s.pop();
                visit(p);
                p = p->rightChild;
            }
        } while (p != nullptr || !s.empty());
    }

    void postOrderNonRecursive(std::function<void(BinTreeNode<T>*)> visit) const {
        enum class Tag { L, R };

        struct StackNode {
            BinTreeNode<T>* ptr;
            Tag tag;
        };

        std::stack<StackNode> s;
        BinTreeNode<T>* p = root;

        do {
            while (p != nullptr) {
                s.push({p, Tag::L});
                p = p->leftChild;
            }

            bool continuePop = true;
            while (continuePop && !s.empty()) {
                StackNode w = s.top();
                s.pop();
                p = w.ptr;

                if (w.tag == Tag::L) {
                    // 从左边来的，说明左子树处理完了，去处理右子树
                    w.tag = Tag::R;
                    s.push(w);
                    continuePop = false;
                    p = p->rightChild;
                }
                else {
                    // 从右边来的，左右都处理完了，访问根节点
                    visit(p);
                }
            }
        } while (!s.empty());
    }

    // ---------- 层次序遍历 ----------
    // 按 2026 课件写法：先访问，再入队。
    void levelOrder(std::function<void(BinTreeNode<T>*)> visit) const {
        if (root == nullptr)
            return;

        std::queue<BinTreeNode<T>*> q;
        BinTreeNode<T>* p = root;
        visit(p);
        q.push(p);

        while (!q.empty()) {
            p = q.front();
            q.pop();

            if (p->leftChild != nullptr) {
                visit(p->leftChild);
                q.push(p->leftChild);
            }

            if (p->rightChild != nullptr) {
                visit(p->rightChild);
                q.push(p->rightChild);
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

        std::stack<BinTreeNode<T>*> s;
        BinTreeNode<T>* p = nullptr;
        int flag = 0;  // 1: 左子女，2: 右子女

        for (char ch : input) {
            if (ch == '#') {
                return root != nullptr;
            }
            if (std::isspace(static_cast<unsigned char>(ch))) {
                continue;
            }

            switch (ch) {
                case '(':
                    if (p == nullptr) {
                        destroy(root);
                        root = nullptr;
                        return false;
                    }
                    s.push(p);
                    flag = 1;
                    break;

                case ')':
                    if (s.empty()) {
                        destroy(root);
                        root = nullptr;
                        return false;
                    }
                    s.pop();
                    break;

                case ',':
                    flag = 2;
                    break;

                default: {
                    p = new BinTreeNode<T>(static_cast<T>(ch));
                    if (root == nullptr) {
                        root = p;
                    } else {
                        if (s.empty()) {
                            delete p;
                            destroy(root);
                            root = nullptr;
                            return false;
                        }

                        BinTreeNode<T>* parent = s.top();
                        if (flag == 1) {
                            parent->leftChild = p;
                        } else if (flag == 2) {
                            parent->rightChild = p;
                        } else {
                            delete p;
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

    BinTreeNode<T>* Copy(BinTreeNode<T>* r) const {
        if (r == nullptr)
            return nullptr;

        BinTreeNode<T>* temp = new BinTreeNode<T>(r->data);
        temp->leftChild = Copy(r->leftChild);
        temp->rightChild = Copy(r->rightChild);
        return temp;
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

    BinTreeNode<T>* Parent(BinTreeNode<T>* subTree, BinTreeNode<T>* t) const {
        if (subTree == nullptr)
            return nullptr;

        if (subTree->leftChild == t || subTree->rightChild == t)
            return subTree;

        BinTreeNode<T>* p = Parent(subTree->leftChild, t);
        if (p != nullptr)
            return p;
        return Parent(subTree->rightChild, t);
    }

    BinTreeNode<T>* Find(BinTreeNode<T>* subTree, const T& item) const {
        if (subTree == nullptr)
            return nullptr;

        if (subTree->data == item)
            return subTree;

        BinTreeNode<T>* p = Find(subTree->leftChild, item);
        if (p != nullptr)
            return p;
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

        BinTreeNode<T>* temp = subTree->leftChild;
        subTree->leftChild = subTree->rightChild;
        subTree->rightChild = temp;
    }
};
