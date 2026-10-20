#include "set/include/UFSets.h"

#include <memory>
#include <stdexcept>
#include <utility>

UFSets::UFSets(int sz) : parent(nullptr), size(sz) {
    if (sz < 0)
        throw std::invalid_argument("negative UFSets size");
    auto newParent = std::make_unique<int[]>(size);
    for (int i = 0; i < size; ++i) {
        newParent[i] = -1;
    }
    parent = newParent.release();
}

UFSets::UFSets(const UFSets& other) : UFSets(other.size) {
    for (int i = 0; i < size; ++i) {
        parent[i] = other.parent[i];
    }
}

UFSets::~UFSets() { delete[] parent; }

UFSets& UFSets::operator=(const UFSets& other) {
    if (this != &other) {
        UFSets copy(other);
        std::swap(parent, copy.parent);
        std::swap(size, copy.size);
    }
    return *this;
}

void UFSets::checkMember(int member) const {
    if (member < 0 || member >= size)
        throw std::out_of_range("UFSets member out of range");
}

void UFSets::checkRoots(int firstRoot, int secondRoot) const {
    checkMember(firstRoot);
    checkMember(secondRoot);
    if (parent[firstRoot] >= 0 || parent[secondRoot] >= 0)
        throw std::invalid_argument("Union requires root indices from Find");
}

int UFSets::Find(int member) const {
    checkMember(member);
    while (parent[member] >= 0) member = parent[member];
    return member;
}

void UFSets::attach(int root, int childRoot) {
    parent[root] += parent[childRoot];
    parent[childRoot] = root;
}

void UFSets::Union(int firstRoot, int secondRoot) {
    checkRoots(firstRoot, secondRoot);
    if (firstRoot != secondRoot)
        attach(firstRoot, secondRoot);
}

void UFSets::WeightedUnion(int firstRoot, int secondRoot) {
    checkRoots(firstRoot, secondRoot);
    if (firstRoot == secondRoot)
        return;
    if (parent[secondRoot] < parent[firstRoot])
        attach(secondRoot, firstRoot);
    else
        attach(firstRoot, secondRoot);
}

int UFSets::CollapsingFind(int member) {
    checkMember(member);
    int root = member;
    while (parent[root] >= 0)
        root = parent[root];
    while (member != root) {
        int previousParent = parent[member];
        parent[member] = root;
        member = previousParent;
    }
    return root;
}

int UFSets::Parent(int member) const { checkMember(member); return parent[member]; }
int UFSets::SetSize(int member) const { return -parent[Find(member)]; }
int UFSets::SetCount() const {
    int count = 0;
    for (int i = 0; i < size; ++i)
        if (parent[i] < 0)
            ++count;
    return count;
}
