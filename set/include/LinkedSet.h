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
// 课件只有声明；以下操作按有序链表及双指针归并规则补全。
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

    LinkedSet operator+(const LinkedSet& other) const {
        LinkedSet result;
        auto* leftMember = first->link;
        auto* rightMember = other.first->link;

        while (leftMember || rightMember) {
            if (!rightMember || (leftMember && leftMember->data < rightMember->data)) {
                result.append(leftMember->data);
                leftMember = leftMember->link;
            } else if (!leftMember || leftMember->data > rightMember->data) {
                result.append(rightMember->data);
                rightMember = rightMember->link;
            } else {
                result.append(leftMember->data);
                leftMember = leftMember->link;
                rightMember = rightMember->link;
            }
        }
        return result;
    }

    LinkedSet operator*(const LinkedSet& other) const {
        LinkedSet result;
        auto* leftMember = first->link;
        auto* rightMember = other.first->link;

        while (leftMember && rightMember) {
            if (leftMember->data < rightMember->data)
                leftMember = leftMember->link;
            else if (leftMember->data > rightMember->data)
                rightMember = rightMember->link;
            else {
                result.append(leftMember->data);
                leftMember = leftMember->link;
                rightMember = rightMember->link;
            }
        }
        return result;
    }

    LinkedSet operator-(const LinkedSet& other) const {
        LinkedSet result;
        auto* rightMember = other.first->link;

        for (auto* leftMember = first->link; leftMember; leftMember = leftMember->link) {
            while (rightMember && rightMember->data < leftMember->data)
                rightMember = rightMember->link;
            if (!rightMember || !(rightMember->data == leftMember->data))
                result.append(leftMember->data);
        }
        return result;
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
