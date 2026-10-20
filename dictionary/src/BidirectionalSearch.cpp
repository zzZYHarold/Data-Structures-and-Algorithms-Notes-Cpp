#include "dictionary/include/BidirectionalSearch.h"

namespace bidirectional_search {

bool search(Node* head, Node*& currentNode, int key) {
    if (!head) return false;
    Node* candidate = currentNode ? currentNode : head;
    if (key > candidate->key) {
        while (candidate && candidate->key < key) candidate = candidate->next;
    } else if (key < candidate->key) {
        while (candidate && candidate->key > key) candidate = candidate->prev;
    }
    if (candidate && candidate->key == key) {
        currentNode = candidate;
        return true;
    }
    return false;
}

}  // namespace bidirectional_search
