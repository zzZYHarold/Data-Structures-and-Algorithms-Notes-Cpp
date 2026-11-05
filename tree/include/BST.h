#pragma once

#include <iostream>
#include <stdexcept>
#include <utility>

// 2026 秋 Lesson 7（最终版课件，已核对），7.2；E 是含公开成员 key 的完整记录，K 是关键码类型。
template <class E, class K>
struct BSTNode {
    E data{};
    BSTNode* left = nullptr;
    BSTNode* right = nullptr;

    BSTNode() = default;
    explicit BSTNode(const E& entry, BSTNode* leftChild = nullptr, BSTNode* rightChild = nullptr)
        : data(entry), left(leftChild), right(rightChild) {}
    virtual ~BSTNode() = default; // AVLNode 经基类指针释放。
    void setData(E entry) { data = entry; }
    E getData() const { return data; }
    bool operator<(const E& entry) const { return data.key < entry.key; }
    bool operator>(const E& entry) const { return data.key > entry.key; }
    bool operator==(const E& entry) const { return data.key == entry.key; }
};

template <class E, class K>
class BST {
public:
    BST() = default;
    // 第 51 页：从 cin 读到终止关键码；流失败也结束，终止记录不入树。
    explicit BST(K stopKey) : RefValue(stopKey) {
        try {
            E entry;
            while (std::cin >> entry && entry.key != RefValue) Insert(entry);
        } catch (...) {
            makeEmpty();
            throw;
        }
    }

    BST(const BST& other) : RefValue(other.RefValue) { root = Copy(other.root); }

    BST& operator=(const BST& other) {
        if (this != &other) {
            // 虚 Copy 让派生 AVL 对象赋值后仍保持平衡。
            K copiedRefValue = other.RefValue;
            Node* copiedRoot = Copy(other.root);
            try { RefValue = copiedRefValue; }
            catch (...) { makeEmpty(copiedRoot); throw; }
            makeEmpty();
            root = copiedRoot;
        }
        return *this;
    }

    virtual ~BST() { makeEmpty(); }

    bool Search(const K key) const { return Search(key, root) != nullptr; }
    // 补充：第 44 页非递归搜索对比入口。
    bool SearchIterative(const K key) const {
        const Node* currentNode = root;
        while (currentNode) {
            if (key < currentNode->data.key)
                currentNode = currentNode->left;
            else if (key > currentNode->data.key)
                currentNode = currentNode->right;
            else return true;
        }
        return false;
    }
    virtual bool Insert(const E& entry) { return Insert(entry, root); }
    virtual bool Remove(const K key) { return Remove(key, root); }
    void makeEmpty() { makeEmpty(root); }
    void PrintTree(std::ostream& output = std::cout) const { PrintTree(root, output); }

    E Min() const {
        if (!root)
            throw std::underflow_error("minimum of empty BST");
        return Min(root)->data;
    }
    E Max() const {
        if (!root)
            throw std::underflow_error("maximum of empty BST");
        const Node* currentNode = root;
        while (currentNode->right)
            currentNode = currentNode->right;
        return currentNode->data;
    }
    const BSTNode<E, K>* getRoot() const { return root; } // 补充：只读观察结构。

protected:
    using Node = BSTNode<E, K>;
    Node* root = nullptr;
    K RefValue{};

    static const Node* Search(const K& key, const Node* subtreeRoot) {
        if (!subtreeRoot)
            return nullptr;
        if (key < subtreeRoot->data.key)
            return Search(key, subtreeRoot->left);
        if (key > subtreeRoot->data.key)
            return Search(key, subtreeRoot->right);
        return subtreeRoot;
    }

    static bool Insert(const E& entry, Node*& subtreeRoot) {
        if (!subtreeRoot) {
            subtreeRoot = new Node(entry);
            return true;
        }
        if (entry.key < subtreeRoot->data.key)
            return Insert(entry, subtreeRoot->left);
        if (entry.key > subtreeRoot->data.key)
            return Insert(entry, subtreeRoot->right);
        return false;
    }

    static bool Remove(const K& key, Node*& subtreeRoot) {
        if (!subtreeRoot)
            return false;
        if (key < subtreeRoot->data.key)
            return Remove(key, subtreeRoot->left);
        if (key > subtreeRoot->data.key)
            return Remove(key, subtreeRoot->right);
        if (subtreeRoot->left && subtreeRoot->right) {
            const Node* successor = Min(subtreeRoot->right);
            subtreeRoot->data = successor->data; // 复制完整记录，保留 key 与附加数据对应关系。
            return Remove(subtreeRoot->data.key, subtreeRoot->right);
        }
        Node* removedNode = subtreeRoot;
        subtreeRoot = subtreeRoot->left ? subtreeRoot->left : subtreeRoot->right;
        delete removedNode;
        return true;
    }

    static const Node* Min(const Node* subtreeRoot) {
        while (subtreeRoot->left)
            subtreeRoot = subtreeRoot->left;
        return subtreeRoot;
    }

    static void makeEmpty(Node*& subtreeRoot) {
        if (!subtreeRoot)
            return;
        makeEmpty(subtreeRoot->left);
        makeEmpty(subtreeRoot->right);
        delete subtreeRoot;
        subtreeRoot = nullptr;
    }

    static void PrintTree(const Node* subtreeRoot, std::ostream& output) {
        if (!subtreeRoot)
            return;
        PrintTree(subtreeRoot->left, output);
        output << subtreeRoot->data << '\n';
        PrintTree(subtreeRoot->right, output);
    }

    virtual Node* Copy(const Node* sourceRoot) const { return CopyNodes(sourceRoot); }

private:
    static Node* CopyNodes(const Node* sourceRoot) {
        if (!sourceRoot) return nullptr;
        Node* copiedRoot = new Node(sourceRoot->data);
        try {
            copiedRoot->left = CopyNodes(sourceRoot->left);
            copiedRoot->right = CopyNodes(sourceRoot->right);
        } catch (...) { makeEmpty(copiedRoot); throw; }
        return copiedRoot;
    }
};
