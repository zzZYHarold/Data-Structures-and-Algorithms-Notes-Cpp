#pragma once

#include <utility>

template <class T>
struct SetNode {
    T data{};
    SetNode* link;
    SetNode() : link(nullptr) {}
    SetNode(const T& value, SetNode* next = nullptr) : data(value), link(next) {}
};

// 6.1 有序链表集合：first 为头结点，last 为尾结点，成员严格递增。
// 2026 秋正式课件第 20~32 页：并、交、差原地修改左操作数，返回自身引用。
template <class T>
class LinkedSet {
    SetNode<T>* first;
    SetNode<T>* last;

    void append(const T& value) {
        last->link = new SetNode<T>(value);
        last = last->link;
    }

public:
    LinkedSet() : first(new SetNode<T>), last(first) {}

    LinkedSet(const LinkedSet& other) : LinkedSet() {
        // 委托构造完成后，若复制成员抛异常，C++ 会调用本对象的析构函数。
        for (auto* member = other.first->link; member; member = member->link)
            append(member->data);
    }

    ~LinkedSet() { makeEmpty(); delete first; }

    LinkedSet& operator=(const LinkedSet& other) {
        if (this != &other) {
            LinkedSet copy(other);
            std::swap(first, copy.first);
            std::swap(last, copy.last);
        }
        return *this;
    }

    void makeEmpty() {
        while (first->link) {
            auto* member = first->link;
            first->link = member->link;
            delete member;
        }
        last = first;
    }

    bool addMember(const T& value) {
        auto* previousNode = first;
        while (previousNode->link && previousNode->link->data < value)
            previousNode = previousNode->link;

        if (previousNode->link && previousNode->link->data == value)
            return false;

        previousNode->link = new SetNode<T>(value, previousNode->link);
        if (last == previousNode)
            last = previousNode->link;
        return true;
    }

    bool delMember(const T& value) {
        auto* previousNode = first;
        while (previousNode->link && previousNode->link->data < value)
            previousNode = previousNode->link;

        auto* targetNode = previousNode->link;
        if (!targetNode || !(targetNode->data == value))
            return false;

        previousNode->link = targetNode->link;
        if (last == targetNode)
            last = previousNode;
        delete targetNode;
        return true;
    }

    bool Contains(const T& value) const {
        auto* member = first->link;
        while (member && member->data < value)
            member = member->link;
        return member && member->data == value;
    }

    bool Min(T& value) const {
        if (!first->link) return false;
        value = first->link->data;
        return true;
    }

    bool Max(T& value) const {
        if (last == first) return false;
        value = last->data;
        return true;
    }

    LinkedSet& operator+(const LinkedSet& other) {
        if (this == &other) return *this;
        auto* previousNode = first;
        auto* leftMember = first->link;
        auto* rightMember = other.first->link;

        while (leftMember && rightMember) {
            if (leftMember->data == rightMember->data) {
                previousNode = leftMember;
                leftMember = leftMember->link;
                rightMember = rightMember->link;
            } else if (leftMember->data < rightMember->data) {
                previousNode = leftMember;
                leftMember = leftMember->link;
            } else {
                previousNode->link = new SetNode<T>(rightMember->data, leftMember);
                previousNode = previousNode->link;
                rightMember = rightMember->link;
            }
        }
        // this 剩余的结点已经在原链中；只复制 other 剩余的结点。
        while (rightMember) {
            append(rightMember->data);
            rightMember = rightMember->link;
        }
        return *this;
    }

    LinkedSet& operator*(const LinkedSet& other) {
        if (this == &other) return *this;
        auto* previousNode = first;
        auto* leftMember = first->link;
        auto* rightMember = other.first->link;

        while (leftMember && rightMember) {
            if (leftMember->data < rightMember->data) {
                previousNode->link = leftMember->link;
                delete leftMember;
                leftMember = previousNode->link;
            } else if (rightMember->data < leftMember->data) {
                rightMember = rightMember->link;
            } else {
                previousNode = leftMember;
                leftMember = leftMember->link;
                rightMember = rightMember->link;
            }
        }
        while (leftMember) {
            previousNode->link = leftMember->link;
            delete leftMember;
            leftMember = previousNode->link;
        }
        last = previousNode;
        return *this;
    }

    LinkedSet& operator-(const LinkedSet& other) {
        if (this == &other) {
            makeEmpty();
            return *this;
        }
        auto* previousNode = first;
        auto* leftMember = first->link;
        auto* rightMember = other.first->link;

        while (leftMember && rightMember) {
            if (leftMember->data == rightMember->data) {
                rightMember = rightMember->link;
                previousNode->link = leftMember->link;
                if (last == leftMember) last = previousNode;
                delete leftMember;
                leftMember = previousNode->link;
            } else if (leftMember->data < rightMember->data) {
                previousNode = leftMember;
                leftMember = leftMember->link;
            } else {
                rightMember = rightMember->link;
            }
        }
        return *this;
    }

    bool subSet(const LinkedSet& other) const {
        auto* rightMember = other.first->link;
        for (auto* leftMember = first->link; leftMember; leftMember = leftMember->link) {
            while (rightMember && rightMember->data < leftMember->data)
                rightMember = rightMember->link;
            if (!rightMember || !(rightMember->data == leftMember->data)) return false;
        }
        return true;
    }

    bool operator==(const LinkedSet& other) const {
        auto* leftMember = first->link;
        auto* rightMember = other.first->link;
        while (leftMember && rightMember) {
            if (!(leftMember->data == rightMember->data)) return false;
            leftMember = leftMember->link;
            rightMember = rightMember->link;
        }
        return !leftMember && !rightMember;
    }
};
