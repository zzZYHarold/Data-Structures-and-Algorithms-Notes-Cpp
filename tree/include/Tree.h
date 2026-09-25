#pragma once

#include <queue>

// 5.5 树与森林：子女-兄弟表示
// 依据 2025 秋课件整理；当前 2026 part1 尚未覆盖本节。

template <class T>
struct TreeNode {
    T data;
    TreeNode<T>* firstChild;
    TreeNode<T>* nextSibling;

    TreeNode(const T& value = T(),
             TreeNode<T>* fc = nullptr,
             TreeNode<T>* ns = nullptr)
        : data(value), firstChild(fc), nextSibling(ns) {}
};

template <class T>
class Tree {
public:
    Tree()
        : root(nullptr), current(nullptr) {}

    ~Tree() {
        RemoveSubTree(root);
        root = current = nullptr;
    }

    bool IsEmpty() const {
        return root == nullptr;
    }

    // 补充：对应 5.1 抽象接口中的 BuildRoot。
    bool BuildRoot(const T& value) {
        if (root != nullptr) {
            return false;
        }
        root = new TreeNode<T>(value);
        current = root;
        return true;
    }

    // 补充：在指定父结点的子女链末尾插入新子女。
    TreeNode<T>* InsertChild(TreeNode<T>* parentNode, const T& value) {
        if (parentNode == nullptr) {
            return nullptr;
        }

        TreeNode<T>* node = new TreeNode<T>(value);
        if (parentNode->firstChild == nullptr) {
            parentNode->firstChild = node;
        } else {
            TreeNode<T>* p = parentNode->firstChild;
            while (p->nextSibling != nullptr) {
                p = p->nextSibling;
            }
            p->nextSibling = node;
        }
        return node;
    }

    // 让根结点成为当前结点。
    bool Root() {
        if (root == nullptr) {
            current = nullptr;
            return false;
        }
        current = root;
        return true;
    }

    // 将当前结点的第一个子女置为当前结点。
    bool FirstChild() {
        if (current != nullptr && current->firstChild != nullptr) {
            current = current->firstChild;
            return true;
        }
        current = nullptr;
        return false;
    }

    // 将当前结点的下一个兄弟置为当前结点。
    bool NextSibling() {
        if (current != nullptr && current->nextSibling != nullptr) {
            current = current->nextSibling;
            return true;
        }
        current = nullptr;
        return false;
    }

    // 将当前结点的双亲置为当前结点。
    bool Parent() {
        TreeNode<T>* p = current;
        if (current == nullptr || current == root) {
            current = nullptr;
            return false;
        }
        return FindParent(root, p);
    }

    // 搜索含 value 的结点，并置为当前结点。
    bool Find(const T& value) {
        current = nullptr;
        return Find(root, value);
    }

    TreeNode<T>* getRoot() const {
        return root;
    }

    TreeNode<T>* getCurrent() const {
        return current;
    }

    // 广度优先（层次序）遍历。
    void LevelOrder(void (*visit)(TreeNode<T>*)) {
        if (root == nullptr) {
            return;
        }

        TreeNode<T>* saved = current;
        std::queue<TreeNode<T>*> q;
        q.push(root);

        while (!q.empty()) {
            current = q.front();
            q.pop();
            visit(current);

            TreeNode<T>* child = current->firstChild;
            while (child != nullptr) {
                q.push(child);
                child = child->nextSibling;
            }
        }

        current = saved;
    }

private:
    TreeNode<T>* root;
    TreeNode<T>* current;

    bool Find(TreeNode<T>* p, const T& value) {
        if (p == nullptr) {
            return false;
        }

        if (p->data == value) {
            current = p;
            return true;
        }

        TreeNode<T>* child = p->firstChild;
        while (child != nullptr) {
            if (Find(child, value)) {
                return true;
            }
            child = child->nextSibling;
        }
        return false;
    }

    void RemoveSubTree(TreeNode<T>* p) {
        if (p == nullptr) {
            return;
        }

        TreeNode<T>* child = p->firstChild;
        while (child != nullptr) {
            TreeNode<T>* next = child->nextSibling;
            RemoveSubTree(child);
            child = next;
        }
        delete p;
    }

    bool FindParent(TreeNode<T>* t, TreeNode<T>* p) {
        if (t == nullptr) {
            current = nullptr;
            return false;
        }

        TreeNode<T>* q = t->firstChild;
        while (q != nullptr) {
            if (q == p) {
                current = t;
                return true;
            }

            if (FindParent(q, p)) {
                return true;
            }
            q = q->nextSibling;
        }

        current = nullptr;
        return false;
    }
};
