#pragma once

#include "tree/include/MinHeap.h"

#include <stdexcept>

// 5.7 Huffman 树
// 依据 2026 秋完整课件第 178-196 页整理。
// 沿用课件的两个模板参数；当前 E 表示数值权值，T 未用于保存字符。

template <class T, class E>
struct HuffmanNode {
    E data;  // 权值
    HuffmanNode<T, E>* parent;
    HuffmanNode<T, E>* leftChild;
    HuffmanNode<T, E>* rightChild;

    HuffmanNode() : data(), parent(nullptr), leftChild(nullptr), rightChild(nullptr) {}

    explicit HuffmanNode(const E& weight, HuffmanNode<T, E>* parentNode = nullptr,
                         HuffmanNode<T, E>* left = nullptr, HuffmanNode<T, E>* right = nullptr)
        : data(weight), parent(parentNode), leftChild(left), rightChild(right) {}
};

template <class T, class E>
class HuffmanTree {
private:
    using Node = HuffmanNode<T, E>;

    // 堆元素按根权值比较，附加数据保存独立分配的结点指针。
    // 堆调整会搬移 Element，但不会搬移 Huffman 结点。
    using ForestElement = Element<E, Node*>;

public:
    HuffmanTree() : root(nullptr) {}

    HuffmanTree(const E weights[], int leafCount) : root(nullptr) { build(weights, leafCount); }

    // 补充：拥有动态结点，禁止默认浅拷贝造成重复释放。
    HuffmanTree(const HuffmanTree&) = delete;
    HuffmanTree& operator=(const HuffmanTree&) = delete;

    ~HuffmanTree() { deleteTree(root); }

    Node* getRoot() const {
        return root;
    }

    // weights 使用 0-based 下标，保存非负频度或概率权值。
    void build(const E weights[], int leafCount) {
        if (leafCount < 0 || (leafCount > 0 && weights == nullptr))
            throw std::invalid_argument("invalid Huffman weight array");

        deleteTree(root);
        root = nullptr;

        if (leafCount == 0)
            return;

        // 初始森林最多有 leafCount 棵树，每次合并后棵数减少一。
        MinHeap<E, Node*> forestHeap(leafCount);

        for (int index = 0; index < leafCount; ++index) {
            Node* leafNode = new Node(weights[index]);
            forestHeap.Insert(ForestElement(weights[index], leafNode));
        }

        if (leafCount == 1) {
            ForestElement onlyTree;
            forestHeap.Remove(onlyTree);
            root = onlyTree.other;
            return;
        }

        // 课件第 190-192 页：取出两棵权值最小的树，合并后重新入堆。
        for (int mergeCount = 0; mergeCount < leafCount - 1; ++mergeCount) {
            ForestElement firstTree;
            ForestElement secondTree;
            forestHeap.Remove(firstTree);
            forestHeap.Remove(secondTree);

            Node* parentNode = nullptr;
            mergeTree(firstTree.other, secondTree.other, parentNode);
            forestHeap.Insert(ForestElement(parentNode->data, parentNode));
            root = parentNode;
        }
    }

    // 补充实现，对应课件第 182 页的 WPL 定义。单叶树深度为 0，WPL 为 0。
    E weightedPathLength() const {
        return weightedPathLength(root, 0);
    }

private:
    Node* root;

    void deleteTree(Node* subtreeRoot) {
        if (subtreeRoot == nullptr) {
            return;
        }
        deleteTree(subtreeRoot->leftChild);
        deleteTree(subtreeRoot->rightChild);
        delete subtreeRoot;
    }

    void mergeTree(Node* leftRoot, Node* rightRoot, Node*& parentNode) {
        parentNode = new Node(leftRoot->data + rightRoot->data, nullptr, leftRoot, rightRoot);
        leftRoot->parent = parentNode;
        rightRoot->parent = parentNode;
    }

    E weightedPathLength(Node* subtreeRoot, int depth) const {
        if (subtreeRoot == nullptr)
            return E();

        if (subtreeRoot->leftChild == nullptr && subtreeRoot->rightChild == nullptr)
            return subtreeRoot->data * depth;

        return weightedPathLength(subtreeRoot->leftChild, depth + 1)
             + weightedPathLength(subtreeRoot->rightChild, depth + 1);
    }
};
