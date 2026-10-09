#pragma once

#include <functional>
#include <queue>

// 5.5 树与森林：子女-兄弟表示
// 依据 2026 秋完整课件第 129-155 页整理。

template <class T>
struct TreeNode {
    T data;
    TreeNode<T>* firstChild;
    TreeNode<T>* nextSibling;

    TreeNode(const T& value = T(), TreeNode<T>* child = nullptr, TreeNode<T>* sibling = nullptr)
        : data(value), firstChild(child), nextSibling(sibling) {}
};

template <class T>
class Tree {
public:
    Tree() : root(nullptr) {}

    // 本类拥有结点；课件未给出复制算法，禁止默认浅拷贝。
    Tree(const Tree&) = delete;
    Tree& operator=(const Tree&) = delete;

    ~Tree() {
        RemoveSubTree(root);
    }

    bool IsEmpty() const {
        return root == nullptr;
    }

    // 补充：对应 5.1 抽象接口中的 BuildRoot。
    bool BuildRoot(const T& value) {
        if (root != nullptr)
            return false;

        root = new TreeNode<T>(value);
        return true;
    }

    // 补充：在指定父结点的子女链末尾插入新子女。
    TreeNode<T>* InsertChild(TreeNode<T>* parentNode, const T& value) {
        if (parentNode == nullptr)
            return nullptr;

        TreeNode<T>* newChild = new TreeNode<T>(value);
        if (parentNode->firstChild == nullptr) {
            parentNode->firstChild = newChild;
        }
        else {
            TreeNode<T>* lastChild = parentNode->firstChild;
            while (lastChild->nextSibling != nullptr)
                lastChild = lastChild->nextSibling;

            lastChild->nextSibling = newChild;
        }
        return newChild;
    }

    TreeNode<T>* getRoot() const {
        return root;
    }

    // 课件第 136 页：返回指定结点的第一个子女。
    TreeNode<T>* FirstChild(TreeNode<T>* node) const {
        return node != nullptr ? node->firstChild : nullptr;
    }

    // 课件第 137 页：返回指定结点的下一个兄弟。
    TreeNode<T>* NextSibling(TreeNode<T>* node) const {
        return node != nullptr ? node->nextSibling : nullptr;
    }

    // 课件第 134-135 页：从根开始查找 targetNode 的双亲。
    TreeNode<T>* Parent(TreeNode<T>* targetNode) const {
        if (targetNode == nullptr || targetNode == root)
            return nullptr;

        return FindParent(root, targetNode);
    }

    // 补充：课件仅声明此接口；按先根次序返回第一个匹配结点。
    TreeNode<T>* Find(const T& value) const {
        return Find(root, value);
    }

    // 课件第 145 页的先根规则：访问根，再依次遍历各子树。
    // 先根、后根遍历按课件规则补成实现，课件未给出完整 C++ 函数体。
    void PreOrder(const std::function<void(TreeNode<T>*)>& visit) const {
        PreOrder(root, visit);
    }

    // 第 146 页的后根规则：依次遍历各子树，再访问根。
    void PostOrder(const std::function<void(TreeNode<T>*)>& visit) const {
        PostOrder(root, visit);
    }

    // 第 147-148 页：按队列分层访问，再依次将各子女入队。
    void LevelOrder(const std::function<void(TreeNode<T>*)>& visit) const {
        if (root == nullptr)
            return;

        std::queue<TreeNode<T>*> pendingNodes;
        pendingNodes.push(root);

        while (!pendingNodes.empty()) {
            TreeNode<T>* node = pendingNodes.front();
            pendingNodes.pop();
            visit(node);

            TreeNode<T>* child = node->firstChild;
            while (child != nullptr) {
                pendingNodes.push(child);
                child = child->nextSibling;
            }
        }
    }

private:
    TreeNode<T>* root;

    TreeNode<T>* Find(TreeNode<T>* subtreeRoot, const T& value) const {
        if (subtreeRoot == nullptr)
            return nullptr;

        if (subtreeRoot->data == value)
            return subtreeRoot;

        TreeNode<T>* child = subtreeRoot->firstChild;
        while (child != nullptr) {
            TreeNode<T>* foundNode = Find(child, value);
            if (foundNode != nullptr)
                return foundNode;

            child = child->nextSibling;
        }
        return nullptr;
    }

    // 补充：课件声明了释放接口；这里释放当前结点及其全部子孙。
    void RemoveSubTree(TreeNode<T>* subtreeRoot) {
        if (subtreeRoot == nullptr)
            return;

        TreeNode<T>* child = subtreeRoot->firstChild;
        while (child != nullptr) {
            TreeNode<T>* nextSibling = child->nextSibling;
            RemoveSubTree(child);
            child = nextSibling;
        }
        delete subtreeRoot;
    }

    TreeNode<T>* FindParent(TreeNode<T>* subtreeRoot, TreeNode<T>* targetNode) const {
        // 课件默认子树非空；补充空指针检查。
        if (subtreeRoot == nullptr)
            return nullptr;

        TreeNode<T>* child = subtreeRoot->firstChild;
        while (child != nullptr) {
            if (child == targetNode)
                return subtreeRoot;

            TreeNode<T>* parentNode = FindParent(child, targetNode);
            if (parentNode != nullptr)
                return parentNode;

            child = child->nextSibling;
        }

        return nullptr;
    }

    void PreOrder(TreeNode<T>* subtreeRoot,
                  const std::function<void(TreeNode<T>*)>& visit) const {
        if (subtreeRoot == nullptr)
            return;

        visit(subtreeRoot);
        for (TreeNode<T>* child = subtreeRoot->firstChild;
             child != nullptr; child = child->nextSibling) {
            PreOrder(child, visit);
        }
    }

    void PostOrder(TreeNode<T>* subtreeRoot,
                   const std::function<void(TreeNode<T>*)>& visit) const {
        if (subtreeRoot == nullptr)
            return;

        for (TreeNode<T>* child = subtreeRoot->firstChild;
             child != nullptr; child = child->nextSibling) {
            PostOrder(child, visit);
        }
        visit(subtreeRoot);
    }
};
