#pragma once

#include "MinHeap.h"

// 5.7 Huffman 树
// 依据 2025 秋课件整理；当前 2026 part1 尚未覆盖本节。

template <class T, class E>
struct HuffmanNode {
    E data;  // 权值
    HuffmanNode<T, E>* parent;
    HuffmanNode<T, E>* leftChild;
    HuffmanNode<T, E>* rightChild;

    HuffmanNode()
        : data(), parent(nullptr), leftChild(nullptr), rightChild(nullptr) {}

    explicit HuffmanNode(const E& elem,
                         HuffmanNode<T, E>* pr = nullptr,
                         HuffmanNode<T, E>* left = nullptr,
                         HuffmanNode<T, E>* right = nullptr)
        : data(elem), parent(pr), leftChild(left), rightChild(right) {}
};

template <class T, class E>
class HuffmanTree {
private:
    using Node = HuffmanNode<T, E>;

    // 课件原代码直接把 HuffmanNode 放进 MinHeap 后再取引用合并，
    // 会产生对象生命周期和地址失效问题。这里用“权值 + 结点指针”的
    // 堆元素修正这一问题，同时仍然使用本章的 MinHeap 完成构造。
    struct HeapItem {
        E weight;
        Node* node;

        HeapItem()
            : weight(), node(nullptr) {}

        HeapItem(const E& w, Node* p)
            : weight(w), node(p) {}

        bool operator<=(const HeapItem& other) const {
            return weight <= other.weight;
        }

        bool operator>(const HeapItem& other) const {
            return weight > other.weight;
        }
    };

public:
    HuffmanTree()
        : root(nullptr) {}

    HuffmanTree(const E w[], int n)
        : root(nullptr) {
        build(w, n);
    }

    ~HuffmanTree() {
        deleteTree(root);
    }

    Node* getRoot() const {
        return root;
    }

    void build(const E w[], int n) {
        deleteTree(root);
        root = nullptr;

        if (w == nullptr || n <= 0) {
            return;
        }

        MinHeap<T, HeapItem> hp(2 * n);

        for (int i = 0; i < n; ++i) {
            Node* node = new Node(w[i]);
            hp.Insert(HeapItem(w[i], node));
        }

        if (n == 1) {
            HeapItem only;
            hp.Remove(only);
            root = only.node;
            return;
        }

        for (int i = 0; i < n - 1; ++i) {
            HeapItem first;
            HeapItem second;
            hp.Remove(first);
            hp.Remove(second);

            Node* parent = nullptr;
            mergeTree(first.node, second.node, parent);
            hp.Insert(HeapItem(parent->data, parent));
            root = parent;
        }
    }

    // 树的带权路径长度 WPL。
    E weightedPathLength() const {
        return weightedPathLength(root, 0);
    }

private:
    Node* root;

    void deleteTree(Node* t) {
        if (t == nullptr) {
            return;
        }
        deleteTree(t->leftChild);
        deleteTree(t->rightChild);
        delete t;
    }

    void mergeTree(Node* bt1,
                   Node* bt2,
                   Node*& parent) {
        parent = new Node(bt1->data + bt2->data,
                          nullptr,
                          bt1,
                          bt2);
        bt1->parent = parent;
        bt2->parent = parent;
    }

    E weightedPathLength(Node* t, int depth) const {
        if (t == nullptr) {
            return E();
        }

        if (t->leftChild == nullptr && t->rightChild == nullptr) {
            return t->data * depth;
        }

        return weightedPathLength(t->leftChild, depth + 1)
             + weightedPathLength(t->rightChild, depth + 1);
    }
};
