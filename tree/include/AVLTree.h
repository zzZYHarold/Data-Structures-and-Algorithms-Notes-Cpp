#pragma once

#include "tree/include/BST.h"
#include <algorithm>

// 2026 秋 Lesson 7（预习课件，已核对），7.3；bf = 右子树高度 - 左子树高度。
template <class E, class K>
struct AVLNode : public BSTNode<E, K> {
    int bf = 0;
    AVLNode() = default;
    explicit AVLNode(const E& entry, AVLNode* leftChild = nullptr, AVLNode* rightChild = nullptr)
        : BSTNode<E, K>(entry, leftChild, rightChild) {}
};

template <class E, class K>
class AVLTree : public BST<E, K> {
    using Base = BST<E, K>;
    // 复用基类的根与子女指针；所有实际结点均由本类分配为 AVLNode。
    using Node = BSTNode<E, K>;
public:
    AVLTree() = default;
    explicit AVLTree(K stopKey) { this->RefValue = stopKey; }
    AVLTree(const AVLTree& other) {
        this->RefValue = other.RefValue;
        this->root = Copy(other.root);
    }
    AVLTree& operator=(const AVLTree& other) {
        Base::operator=(other);
        return *this;
    }

    using Base::Search;
    AVLNode<E, K>* Search(K key, AVLNode<E, K>*& parentNode) const {
        parentNode = nullptr;
        Node* currentNode = this->root;
        while (currentNode) {
            if (key == currentNode->data.key) return static_cast<AVLNode<E, K>*>(currentNode);
            parentNode = static_cast<AVLNode<E, K>*>(currentNode);
            currentNode = key < currentNode->data.key ? currentNode->left : currentNode->right;
        }
        return nullptr;
    }
    int Height() const { return Height(this->root); }
    bool Insert(const E& entry) override {
        bool taller = false;
        return Insert(this->root, entry, taller);
    }
    bool Remove(K key, E& removedEntry) {
        bool shorter = false;
        return Remove(this->root, key, &removedEntry, shorter);
    }
    bool Remove(const K key) override {
        bool shorter = false;
        return Remove(this->root, key, nullptr, shorter);
    }
    const AVLNode<E, K>* getRoot() const {
        return static_cast<const AVLNode<E, K>*>(this->root);
    }

    // 补充：向现有树追加记录，遇 RefValue 停止；流失败时保留已插入的记录。
    friend std::istream& operator>>(std::istream& input, AVLTree& tree) {
        E entry;
        while (input >> entry && entry.key != tree.RefValue) tree.Insert(entry);
        return input;
    }
    friend std::ostream& operator<<(std::ostream& output, const AVLTree& tree) {
        tree.PrintTree(output);
        return output;
    }

protected:
    static int Height(const Node* subtreeRoot) {
        if (!subtreeRoot) return 0;
        return 1 + std::max(Height(subtreeRoot->left), Height(subtreeRoot->right));
    }
    static int& balance(Node* node) { return static_cast<AVLNode<E, K>*>(node)->bf; }

    // 第 63/66 页单旋：这里先按插入情形将两个 bf 置零。
    static void RotateL(Node*& subtreeRoot) {
        Node* oldRoot = subtreeRoot;
        subtreeRoot = oldRoot->right;
        oldRoot->right = subtreeRoot->left;
        subtreeRoot->left = oldRoot;
        balance(subtreeRoot) = balance(oldRoot) = 0;
    }
    static void RotateR(Node*& subtreeRoot) {
        Node* oldRoot = subtreeRoot;
        subtreeRoot = oldRoot->left;
        oldRoot->left = subtreeRoot->right;
        subtreeRoot->right = oldRoot;
        balance(subtreeRoot) = balance(oldRoot) = 0;
    }
    // 第 71 页：先左后右双旋，按旋转前中间结点的 bf 分情况更新。
    static void RotateLR(Node*& subtreeRoot) {
        Node* oldRoot = subtreeRoot;
        Node* leftChild = oldRoot->left;
        subtreeRoot = leftChild->right;
        leftChild->right = subtreeRoot->left;
        subtreeRoot->left = leftChild;
        balance(leftChild) = balance(subtreeRoot) <= 0 ? 0 : -1;
        oldRoot->left = subtreeRoot->right;
        subtreeRoot->right = oldRoot;
        balance(oldRoot) = balance(subtreeRoot) == -1 ? 1 : 0;
        balance(subtreeRoot) = 0;
    }
    // 第 76 页：先右后左双旋。
    static void RotateRL(Node*& subtreeRoot) {
        Node* oldRoot = subtreeRoot;
        Node* rightChild = oldRoot->right;
        subtreeRoot = rightChild->left;
        rightChild->left = subtreeRoot->right;
        subtreeRoot->right = rightChild;
        balance(rightChild) = balance(subtreeRoot) >= 0 ? 0 : 1;
        oldRoot->right = subtreeRoot->left;
        subtreeRoot->left = oldRoot;
        balance(oldRoot) = balance(subtreeRoot) == 1 ? -1 : 0;
        balance(subtreeRoot) = 0;
    }

    // 调用前 bf 为 +/-2；返回旋转是否使子树高度减 1（删除回溯使用）。
    static bool Rebalance(Node*& subtreeRoot) {
        if (balance(subtreeRoot) == -2) {
            int childBalance = balance(subtreeRoot->left);
            if (childBalance > 0) RotateLR(subtreeRoot);
            else {
                RotateR(subtreeRoot);
                if (childBalance == 0) {
                    // 第 92 页删除特例：单旋后高度不变，bf 不能都置零。
                    balance(subtreeRoot) = 1;
                    balance(subtreeRoot->right) = -1;
                    return false;
                }
            }
        } else {
            int childBalance = balance(subtreeRoot->right);
            if (childBalance < 0) RotateRL(subtreeRoot);
            else {
                RotateL(subtreeRoot);
                if (childBalance == 0) {
                    balance(subtreeRoot) = -1;
                    balance(subtreeRoot->left) = 1;
                    return false;
                }
            }
        }
        return true;
    }

    // 第 79-86 页只有步骤和例子；补充递归回溯，用 taller 传递高度变化。
    static bool Insert(Node*& subtreeRoot, const E& entry, bool& taller) {
        if (!subtreeRoot) {
            subtreeRoot = new AVLNode<E, K>(entry);
            taller = true;
            return true;
        }
        int change;
        bool inserted;
        if (entry.key < subtreeRoot->data.key) {
            inserted = Insert(subtreeRoot->left, entry, taller);
            change = -1;
        } else if (entry.key > subtreeRoot->data.key) {
            inserted = Insert(subtreeRoot->right, entry, taller);
            change = 1;
        } else { taller = false; return false; }
        if (!inserted || !taller) return inserted;
        balance(subtreeRoot) += change;
        if (balance(subtreeRoot) == 0) taller = false;
        else if (balance(subtreeRoot) == -2 || balance(subtreeRoot) == 2) {
            Rebalance(subtreeRoot);
            taller = false;
        }
        return true;
    }

    // 第 89-94 页：左子树缩短则 bf+1，右子树缩短则 bf-1。
    static void AfterShrink(Node*& subtreeRoot, int change, bool& shorter) {
        if (!shorter) return;
        balance(subtreeRoot) += change;
        if (balance(subtreeRoot) == 1 || balance(subtreeRoot) == -1) shorter = false;
        else if (balance(subtreeRoot) == 2 || balance(subtreeRoot) == -2)
            shorter = Rebalance(subtreeRoot);
        // bf == 0 时 shorter 保持 true，继续向上回溯。
    }

    // 第 87-99 页规则补全：双子女结点使用中序直接前驱替补。
    static bool Remove(Node*& subtreeRoot, const K& key, E* removedEntry, bool& shorter) {
        if (!subtreeRoot) { shorter = false; return false; }
        bool removed;
        if (key < subtreeRoot->data.key) {
            removed = Remove(subtreeRoot->left, key, removedEntry, shorter);
            AfterShrink(subtreeRoot, 1, shorter);
        } else if (key > subtreeRoot->data.key) {
            removed = Remove(subtreeRoot->right, key, removedEntry, shorter);
            AfterShrink(subtreeRoot, -1, shorter);
        } else {
            if (removedEntry) *removedEntry = subtreeRoot->data;
            if (subtreeRoot->left && subtreeRoot->right) {
                Node* predecessor = subtreeRoot->left;
                while (predecessor->right) predecessor = predecessor->right;
                subtreeRoot->data = predecessor->data;
                removed = Remove(subtreeRoot->left, subtreeRoot->data.key, nullptr, shorter);
                AfterShrink(subtreeRoot, 1, shorter);
            } else {
                Node* removedNode = subtreeRoot;
                subtreeRoot = subtreeRoot->left ? subtreeRoot->left : subtreeRoot->right;
                delete removedNode;
                shorter = true;
                removed = true;
            }
        }
        return removed;
    }

    // 补充复制：即使经 BST 引用赋值普通 BST，也重新插入以维持 AVL 不变量。
    Node* Copy(const Node* sourceRoot) const override {
        AVLTree replacement;
        CopyInto(sourceRoot, replacement);
        return std::exchange(replacement.root, nullptr);
    }

private:
    static void CopyInto(const Node* sourceRoot, AVLTree& destination) {
        if (!sourceRoot) return;
        destination.Insert(sourceRoot->data);
        CopyInto(sourceRoot->left, destination);
        CopyInto(sourceRoot->right, destination);
    }
};
