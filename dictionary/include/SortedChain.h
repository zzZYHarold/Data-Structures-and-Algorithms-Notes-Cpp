#pragma once

#include <iostream>
#include <stdexcept>

// 6.3 有序链表字典，2026 秋正式课件第 77~83 页。
// E 为属性类型，K 为关键码类型；与 SortedList 中 E 表示整个表项不同。
template <class E, class K>
struct ChainNode {
    K name{};
    E attr{};
    ChainNode* link = nullptr;

    ChainNode() = default;
    ChainNode(K key, E attribute, ChainNode* next = nullptr)
        : name(key), attr(attribute), link(next) {}
};

template <class E, class K>
class SortedChain {
    ChainNode<E, K>* first;  // 不存放有效表项的头结点

    // 同时定位目标或插入位置的前驱。未找到时仍保留有效的 previousNode。
    ChainNode<E, K>* Search(K key, ChainNode<E, K>*& previousNode) const {
        previousNode = first;
        auto* currentNode = first->link;
        while (currentNode && currentNode->name < key) {
            previousNode = currentNode;
            currentNode = currentNode->link;
        }
        if (currentNode && currentNode->name == key) return currentNode;
        return nullptr;
    }

public:
    SortedChain() : first(new ChainNode<E, K>) {}
    ~SortedChain() { makeEmpty(); delete first; }
    SortedChain(const SortedChain&) = delete;
    SortedChain& operator=(const SortedChain&) = delete;

    void makeEmpty() {
        while (first->link) {
            auto* targetNode = first->link;
            first->link = targetNode->link;
            delete targetNode;
        }
    }
    bool Search(K key) const {
        ChainNode<E, K>* previousNode;
        return Search(key, previousNode) != nullptr;
    }
    void Insert(K key, E attribute) {
        ChainNode<E, K>* previousNode;
        if (Search(key, previousNode))
            throw std::invalid_argument("duplicate dictionary key");
        // 未找到时不能使用 Search 返回的 nullptr 作为后继，否则会截断原链。
        auto* newNode = new ChainNode<E, K>(key, attribute, previousNode->link);
        previousNode->link = newNode;
    }
    bool Remove(K key, E& attribute) {
        ChainNode<E, K>* previousNode;
        auto* targetNode = Search(key, previousNode);
        if (!targetNode) return false;
        attribute = targetNode->attr;
        previousNode->link = targetNode->link;
        delete targetNode;
        return true;
    }
    ChainNode<E, K>* Begin() const { return first->link; }
    ChainNode<E, K>* Next(ChainNode<E, K>* currentNode) const {
        return currentNode ? currentNode->link : nullptr;
    }
    // 课件仅声明 printDic；补充逐行输出“关键码: 属性”的格式。
    void printDic(std::ostream& output = std::cout) const {
        for (auto* currentNode = first->link; currentNode; currentNode = currentNode->link)
            output << currentNode->name << ": " << currentNode->attr << '\n';
    }
};
