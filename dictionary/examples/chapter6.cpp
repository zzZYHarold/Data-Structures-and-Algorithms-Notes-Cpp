#include "set/include/bitSet.h"
#include "set/include/LinkedSet.h"
#include "set/include/UFSets.h"
#include "set/include/SetApplications.h"
#include "dictionary/include/DictionaryEntry.h"
#include "dictionary/include/SortedList.h"
#include "dictionary/include/SortedChain.h"
#include "dictionary/include/BidirectionalSearch.h"
#include "dictionary/include/HashTable.h"
#include "dictionary/include/ChainedHashTable.h"

#include <iomanip>
#include <cassert>
#include <iostream>
#include <string>

using IntEntry = DictionaryEntry<int, std::string>;

void showProbing(const char* title, ProbingMethod method) {
    HashTable<IntEntry, int> table(10, [](const int& key) { return key % 10; },
                                  method, [](const int& key) { return 7 - key % 7; });
    const int keys[] = {89, 18, 49, 58, 69};
    for (int key : keys) table.Insert(key, {key, std::to_string(key)});
    std::cout << title << ": ";
    for (int index = 0; index < table.Size(); ++index) {
        IntEntry entry;
        if (table.getData(index, entry)) std::cout << std::setw(3) << entry.key;
        else std::cout << "  -";
    }
    int totalProbes = 0;
    for (int key : keys) {
        int probes;
        table.Search(key, &probes);
        totalProbes += probes;
    }
    std::cout << "  ASL(success)=" << static_cast<double>(totalProbes) / 5 << '\n';
}

// 第 123~124 页：交替使用正、负平方增量，不同于第 121 页的单向平方探查。
void showAlternatingQuadratic() {
    HashTable<IntEntry, int> table(19, [](const int& key) { return key % 19; },
                                  ProbingMethod::QuadraticAlternating);
    const int keys[] = {37, 25, 14, 36, 49, 68, 57, 11};
    for (int key : keys) table.Insert(key, {key, std::to_string(key)});
    std::cout << "alternating quadratic (m=19): ";
    for (int index = 0; index < table.Size(); ++index) {
        IntEntry entry;
        if (table.getData(index, entry)) std::cout << std::setw(3) << entry.key;
        else std::cout << "  -";
    }
    int successfulProbes = 0;
    for (int key : keys) {
        int probes;
        table.Search(key, &probes);
        successfulProbes += probes;
    }
    int unsuccessfulProbes = 0;
    for (int home = 0; home < table.Size(); ++home) {
        int probes;
        table.Search(home + 190, &probes); // 不在表中的关键码，覆盖全部 19 个初始地址
        unsuccessfulProbes += probes;
    }
    assert(table.Length() == 8 && table.Search(11) == 10 && table.Search(68) == 12);
    assert(successfulProbes == 11 && unsuccessfulProbes == 33);
    std::cout << "  ASL(success)=11/8=" << static_cast<double>(successfulProbes) / 8
              << ", ASL(failure)=33/19=" << static_cast<double>(unsuccessfulProbes) / 19 << '\n';
}

// 第 126~127 页：表长为质数 11，所有第二散列步长均与表长互素。
void showDoubleHashing() {
    HashTable<IntEntry, int> table(11, [](const int& key) { return 3 * key % 11; },
                                  ProbingMethod::DoubleHashing,
                                  [](const int& key) { return 7 * key % 10 + 1; });
    const int keys[] = {22, 41, 53, 46, 30, 13, 1, 67};
    for (int key : keys) table.Insert(key, {key, std::to_string(key)});
    std::cout << "double hashing (m=11): ";
    for (int index = 0; index < table.Size(); ++index) {
        IntEntry entry;
        if (table.getData(index, entry)) std::cout << std::setw(3) << entry.key;
        else std::cout << "  -";
    }
    int totalProbes = 0;
    for (int key : keys) {
        int probes;
        table.Search(key, &probes);
        totalProbes += probes;
    }
    assert(table.Length() == 8 && totalProbes == 17 && table.Search(1) == 10);
    std::cout << "  ASL(success)=17/8=" << static_cast<double>(totalProbes) / 8 << '\n';
}

int main() {
    // 6.1~6.3：2026 秋正式课件。
    bitSet<int> leftSet(50), rightSet(50);
    for (int member = 0; member < 10; ++member) {
        leftSet.addMember(member);
        rightSet.addMember(member + 7);
    }
    std::cout << "union: " << leftSet + rightSet << '\n'
              << "intersection: " << leftSet * rightSet << '\n'
              << "difference: " << leftSet - rightSet << '\n';
    LinkedSet<int> linkedSet;
    for (int member : {23, 8, 49, 17, 35, 72}) linkedSet.addMember(member);
    int minimum, maximum;
    linkedSet.Min(minimum);
    linkedSet.Max(maximum);
    std::cout << "LinkedSet Min/Max: " << minimum << '/' << maximum << '\n';
    LinkedSet<int> otherSet;
    otherSet.addMember(17);
    otherSet.addMember(100);
    LinkedSet<int> common(linkedSet); // 需要保留原集合时，先显式复制。
    common * otherSet;              // 原地求交，common 只剩 17。
    std::cout << "LinkedSet intersection contains 17: " << common.Contains(17) << '\n';

    UFSets equivalenceClasses(12);
    const int pairs[][2] = {{0,4}, {3,1}, {6,10}, {8,9}, {7,4},
                           {6,8}, {3,5}, {2,11}, {11,0}};
    for (const auto& pair : pairs)
        equivalenceClasses.WeightedUnion(equivalenceClasses.Find(pair[0]),
                                         equivalenceClasses.Find(pair[1]));
    std::cout << "equivalence class count: " << equivalenceClasses.SetCount() << '\n';
    const int isConnected[] = {1,1,0, 1,1,0, 0,0,1};
    std::cout << "province count: " << CountProvinces(isConnected, 3) << '\n';

    SortedList<IntEntry, int> dictionary;
    for (int key : {40, 10, 60, 20, 50, 30})
        dictionary.Insert(key, {key, "attribute"});
    std::cout << "Search(40): " << dictionary.Search(40)
              << ", BinarySearch(40): " << dictionary.BinarySearch(40) << '\n';
    // Search/BinarySearch 返回 0-based 下标；继承的 getData 仍按第二章用 1-based 序号。
    IntEntry foundEntry;
    dictionary.getData(dictionary.Search(40) + 1, foundEntry);
    std::cout << "attribute of 40: " << foundEntry.attribute << '\n';

    SortedChain<std::string, int> chain;
    chain.Insert(30, "book C");
    chain.Insert(10, "book A");
    chain.Insert(20, "book B");
    std::cout << "SortedChain:\n";
    chain.printDic();
    std::string removedAttribute;
    chain.Remove(20, removedAttribute);
    std::cout << "removed attribute: " << removedAttribute << '\n';

    using bidirectional_search::Node;
    Node firstNode(10), middleNode(20), lastNode(30);
    firstNode.next = &middleNode;
    middleNode.prev = &firstNode;
    middleNode.next = &lastNode;
    lastNode.prev = &middleNode;
    Node* currentNode = nullptr;
    bidirectional_search::search(&firstNode, currentNode, 30);
    bidirectional_search::search(&firstNode, currentNode, 10); // 从 30 向左找到 10
    std::cout << "bidirectional search current key: " << currentNode->key << '\n';

    // 6.4：2026 秋完整正式课件第 110~134 页。
    std::cout << "6.4 hashing (2026):\n";
    showProbing("linear   ", ProbingMethod::Linear);
    showProbing("quadratic", ProbingMethod::Quadratic);
    showAlternatingQuadratic();
    showDoubleHashing();
    showProbing("double (m=10 exercise)", ProbingMethod::DoubleHashing);

    using NameEntry = DictionaryEntry<std::string, int>;
    ChainedHashTable<NameEntry, std::string> names(26, [](const std::string& key) {
        return key.front() - 'A';
    });
    const std::string keys[] = {"Burke", "Ekers", "Broad", "Blum", "Attlee",
                                "Alton", "Hecht", "Ederly"};
    for (const auto& key : keys) names.Insert(key, {key, 0});
    for (int bucket = 0; bucket < names.Size(); ++bucket) {
        bool printedBucket = false;
        names.VisitBucket(bucket, [&](const NameEntry& entry) {
            if (!printedBucket) std::cout << "bucket " << bucket << ':';
            std::cout << ' ' << entry.key;
            printedBucket = true;
        });
        if (printedBucket) std::cout << '\n';
    }
}
