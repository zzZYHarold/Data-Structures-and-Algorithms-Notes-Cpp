#pragma once

#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>

// 6.4 开散列（链地址法），2026 秋完整课件第 130~134 页。
// 课件无类定义，按示意图补写数组 + 手写同义词链。
template <class E, class K>
class ChainedHashTable {
    struct Node {
        E entry;
        Node* link = nullptr;
        explicit Node(const E& value) : entry(value) {}
    };
    int tableSize;
    int count = 0;
    std::unique_ptr<Node*[]> buckets;
    std::function<int(const K&)> hash;

    int bucketIndex(const K& key) const {
        int remainder = hash(key) % tableSize;
        return remainder < 0 ? remainder + tableSize : remainder;
    }

public:
    ChainedHashTable(int sz, std::function<int(const K&)> hashFunction)
        : tableSize(sz), hash(std::move(hashFunction)) {
        if (sz <= 0 || !hash) throw std::invalid_argument("invalid chained hash table");
        buckets = std::make_unique<Node*[]>(tableSize);
    }
    ChainedHashTable(const ChainedHashTable&) = delete;
    ChainedHashTable& operator=(const ChainedHashTable&) = delete;
    ~ChainedHashTable() { makeEmpty(); }

    bool Search(const K& key, E& entry, int* probes = nullptr) const {
        if (probes) *probes = 0;
        for (auto* member = buckets[bucketIndex(key)]; member; member = member->link) {
            if (probes) ++*probes;
            if (member->entry == key) { entry = member->entry; return true; }
        }
        // 与课件 ASL 约定一致：失败计入链尾的空指针检查。
        if (probes) ++*probes;
        return false;
    }
    bool Insert(const K& key, const E& entry) {
        if (!(entry == key)) throw std::invalid_argument("entry key mismatch");
        Node** nextLink = &buckets[bucketIndex(key)];
        while (*nextLink) {
            if ((*nextLink)->entry == key) return false;
            nextLink = &(*nextLink)->link;
        }
        *nextLink = new Node(entry);  // 尾插，保持课件图中的插入顺序
        ++count;
        return true;
    }
    bool Remove(const K& key, E& removedEntry) {
        Node** nextLink = &buckets[bucketIndex(key)];
        while (*nextLink && !((*nextLink)->entry == key)) nextLink = &(*nextLink)->link;
        if (!*nextLink) return false;
        auto* targetNode = *nextLink;
        removedEntry = targetNode->entry;
        *nextLink = targetNode->link;
        delete targetNode;
        --count;
        return true;
    }
    bool Update(const K& key, const E& entry) {
        if (!(entry == key)) throw std::invalid_argument("Update cannot change a key");
        for (auto* member = buckets[bucketIndex(key)]; member; member = member->link) {
            if (member->entry == key) { member->entry = entry; return true; }
        }
        return false;
    }
    template <class Visitor>
    void VisitBucket(int index, Visitor visit) const {
        if (index < 0 || index >= tableSize) throw std::out_of_range("invalid bucket");
        for (auto* member = buckets[index]; member; member = member->link)
            visit(static_cast<const E&>(member->entry));
    }
    void makeEmpty() {
        for (int index = 0; index < tableSize; ++index) {
            while (buckets[index]) {
                auto* member = buckets[index];
                buckets[index] = member->link;
                delete member;
            }
        }
        count = 0;
    }
    int Size() const { return tableSize; }
    int Length() const { return count; }
};
