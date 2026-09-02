#ifndef LINKED_LIST_IMPLEMENTATION
#define LINKED_LIST_IMPLEMENTATION

#include "LinkedList.h"

template <class T>
List<T>::List() : first(new LinkNode<T>()) {}

template <class T>
List<T>::List(const T& x) : first(new LinkNode<T>(x)) {}

template <class T>
List<T>::List(const List<T>& other) : first(new LinkNode<T>()) {
    LinkNode<T>* tail = first;
    for (LinkNode<T>* p = other.first->link; p != nullptr; p = p->link) {
        tail->link = new LinkNode<T>(p->data);
        tail = tail->link;
    }
}

template <class T>
List<T>::~List() {
    makeEmpty();
    delete first;
}

template <class T>
void List<T>::makeEmpty() {
    while (first->link != nullptr) {
        LinkNode<T>* q = first->link;
        first->link = q->link;
        delete q;
    }
}

template <class T>
int List<T>::Length() const {
    LinkNode<T>* p = first->link;
    int count = 0;
    while (p != nullptr) {
        p = p->link;
        ++count;
    }
    return count;
}

template <class T>
LinkNode<T>* List<T>::Search(const T& x) const {
    LinkNode<T>* current = first->link;
    while (current != nullptr && current->data != x)
        current = current->link;
    return current;
}

template <class T>
LinkNode<T>* List<T>::Locate(int i) const {
    if (i < 0)
        return nullptr;

    LinkNode<T>* current = first;
    int k = 0;
    while (current != nullptr && k < i) {
        current = current->link;
        ++k;
    }
    return current;
}

template <class T>
bool List<T>::getData(int i, T& x) const {
    if (i <= 0)
        return false;

    LinkNode<T>* current = Locate(i);
    if (current == nullptr)
        return false;
    x = current->data;
    return true;
}

template <class T>
void List<T>::setData(int i, const T& x) {
    if (i <= 0)
        return;

    LinkNode<T>* current = Locate(i);
    if (current != nullptr)
        current->data = x;
}

template <class T>
bool List<T>::Insert(int i, const T& x) {
    LinkNode<T>* current = Locate(i);
    if (current == nullptr)
        return false;

    LinkNode<T>* newNode = new LinkNode<T>(x);
    newNode->link = current->link;
    current->link = newNode;
    return true;
}

template <class T>
bool List<T>::Remove(int i, T& x) {
    if (i <= 0)
        return false;

    LinkNode<T>* current = Locate(i - 1);
    if (current == nullptr || current->link == nullptr)
        return false;

    LinkNode<T>* del = current->link;
    current->link = del->link;
    x = del->data;
    delete del;
    return true;
}

template <class T>
void List<T>::inputFront(const T& endTag, std::istream& in) {
    makeEmpty();

    T val;
    while (in >> val && val != endTag) {
        LinkNode<T>* newNode = new LinkNode<T>(val);
        newNode->link = first->link;
        first->link = newNode;
    }
}

template <class T>
void List<T>::inputRear(const T& endTag, std::istream& in) {
    makeEmpty();

    LinkNode<T>* last = first;
    T val;
    while (in >> val && val != endTag) {
        LinkNode<T>* newNode = new LinkNode<T>(val);
        last->link = newNode;
        last = newNode;
    }
    last->link = nullptr;
}

template <class T>
void List<T>::output(std::ostream& out) const {
    for (LinkNode<T>* p = first->link; p != nullptr; p = p->link) {
        out << p->data;
        if (p->link != nullptr)
            out << ' ';
    }
}

template <class T>
List<T>& List<T>::operator=(const List<T>& other) {
    if (this == &other)
        return *this;

    makeEmpty();
    LinkNode<T>* tail = first;
    for (LinkNode<T>* p = other.first->link; p != nullptr; p = p->link) {
        tail->link = new LinkNode<T>(p->data);
        tail = tail->link;
    }
    return *this;
}

template <class T>
void List<T>::reverse() {
    LinkNode<T>* pre = nullptr;
    LinkNode<T>* cur = first->link;

    while (cur != nullptr) {
        LinkNode<T>* suc = cur->link;
        cur->link = pre;
        pre = cur;
        cur = suc;
    }

    first->link = pre;
}

#endif
