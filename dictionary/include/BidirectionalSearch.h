#pragma once

// 6.3 第 90~91 页练习：升序双向链表从上次成功搜索的位置继续搜索。
// head 指向首个有效结点（无头哨兵）；链表两端以 nullptr 结束。
// currentNode 必须为 nullptr 或本链表仍有效的结点，失败保留上次成功位置。
namespace bidirectional_search {

struct Node {
    int key;
    Node* prev;
    Node* next;
    explicit Node(int value) : key(value), prev(nullptr), next(nullptr) {}
};

bool search(Node* head, Node*& currentNode, int key);

}  // namespace bidirectional_search
