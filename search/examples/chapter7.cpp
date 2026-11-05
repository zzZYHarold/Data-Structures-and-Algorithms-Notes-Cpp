// 依据 2026 秋 Lesson 7 最终版；页码均为最终版 PDF 页序。
#include "search/include/dataList.h"
#include "tree/include/BST.h"
#include "tree/include/AVLTree.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

// Release 构建也执行检查，避免 NDEBUG 跳过具有副作用的增删操作。
void require(bool condition) {
    if (!condition) throw std::logic_error("chapter 7 example check failed");
}

// BST/AVL 的 E 是完整记录；dataList 的 E 则只表示附加数据。
struct SearchRecord {
    int key{};
    std::string other;
};
std::ostream& operator<<(std::ostream& output, const SearchRecord& entry) {
    return output << entry.key << ':' << entry.other;
}
std::istream& operator>>(std::istream& input, SearchRecord& entry) {
    return input >> entry.key >> entry.other;
}

using SearchTree = BST<SearchRecord, int>;
using BalancedTree = AVLTree<SearchRecord, int>;

// 独立计算高度并核对每个结点的 bf，避免只检查中序有序却漏掉失衡。
int checkAVL(const BSTNode<SearchRecord, int>* subtreeRoot, int low, int high) {
    if (!subtreeRoot) return 0;
    require(low < subtreeRoot->data.key && subtreeRoot->data.key < high);
    require(subtreeRoot->data.other == std::to_string(subtreeRoot->data.key));
    int leftHeight = checkAVL(subtreeRoot->left, low, subtreeRoot->data.key);
    int rightHeight = checkAVL(subtreeRoot->right, subtreeRoot->data.key, high);
    const auto* avlNode = dynamic_cast<const AVLNode<SearchRecord, int>*>(subtreeRoot);
    require(avlNode && avlNode->bf == rightHeight - leftHeight);
    require(rightHeight - leftHeight >= -1 && rightHeight - leftHeight <= 1);
    return 1 + std::max(leftHeight, rightHeight);
}

void staticSearchExample() {
    dataList<std::string, int> table(6);
    for (int key : {10, 20, 30, 40, 50, 60})
        table.Insert(key, std::to_string(key));
    require(table.SeqSearch(30) == 3);
    require(table.SeqSearch(99) == 7); // 满表时仍有独立监视哨槽。
    require(table.SeqSearch(30, 1) == 3 && table.SeqSearch(99, 1) == 0);
    require(table.BinarySearch(30) == 3 && table.BinarySearch(99) == 0);
    require(!table.Insert(70, "70"));
    std::cout << "7.1: SeqSearch(30)=" << table.SeqSearch(30)
              << ", missing=" << table.SeqSearch(99) << '\n';

    dataList<std::string, int> copied(table);
    std::string removed;
    require(copied.Remove(20, removed) && removed == "20");
    require(copied.getKey(2) == 60 && table.getKey(2) == 20);
    require(copied.getData(2).getOther() == "60"); // key 与附加数据一起搬移。
    copied = table;
    copied.setKey(25, 2);
    require(table.getKey(2) == 20);
    std::istringstream invalidInput("2 1 one 2");
    invalidInput >> copied;
    require(invalidInput.fail() && copied.Length() == 6 && copied.getKey(2) == 25);
    std::istringstream validInput("2 1 one 2 two");
    validInput >> copied;
    require(copied.Length() == 2 && copied.getData(2).getOther() == "two");
    dataList<int, int> empty(0);
    require(empty.SeqSearch(1) == 1 && empty.SeqSearch(1, 1) == 0);
    require(empty.BinarySearch(1) == 0 && !empty.Insert(1, 1));
}

void binarySearchTreeExample() {
    SearchTree tree;
    // 第 50 页建树序列。
    for (int key : {53, 78, 65, 17, 87, 9, 81, 15})
        require(tree.Insert({key, std::to_string(key)}));
    require(!tree.Insert({53, "duplicate"}));
    require(tree.Search(81) && tree.SearchIterative(81) && !tree.Search(28));
    require(tree.Min().key == 9 && tree.Max().key == 87);
    SearchTree copied(tree);
    require(tree.Remove(53)); // 双子女：中序后继 65 替补。
    require(tree.getRoot()->data.key == 65 && tree.getRoot()->data.other == "65");
    require(tree.Remove(15) && tree.Remove(9) && !tree.Remove(999));
    require(copied.Search(53) && copied.Search(15));
    copied = tree;
    tree.makeEmpty();
    require(!tree.Search(65) && copied.Search(65));
    bool caught = false;
    try { tree.Min(); } catch (const std::underflow_error&) { caught = true; }
    require(caught);
    std::cout << "7.2: BST after removals (in order):\n";
    copied.PrintTree();
}

void avlExample() {
    // LL、RR、LR、RL 四种插入；最后均以 20 为根。
    const int rotations[][3] = {{30, 20, 10}, {10, 20, 30}, {30, 10, 20}, {10, 30, 20}};
    for (const auto& keys : rotations) {
        BalancedTree tree;
        for (int key : keys) require(tree.Insert({key, std::to_string(key)}));
        require(tree.getRoot()->data.key == 20);
        checkAVL(tree.getRoot(), 0, 100);
    }
    // 第 100 页删除特例及镜像：较高子树根 bf == 0，单旋后高度不变。
    const int deletionCases[][5] = {{2, 1, 4, 3, 5}, {4, 2, 5, 1, 3}};
    for (int caseIndex = 0; caseIndex < 2; ++caseIndex) {
        BalancedTree tree;
        for (int key : deletionCases[caseIndex]) tree.Insert({key, std::to_string(key)});
        require(tree.Remove(caseIndex == 0 ? 1 : 5));
        require(tree.getRoot()->bf == (caseIndex == 0 ? -1 : 1));
        require(tree.Height() == 3);
        checkAVL(tree.getRoot(), 0, 100);
    }

    // 最终版第 111 页作业（4）：图 1，按中序直接前驱替补。
    // 按层序插入复现原图，再依次删除 22、3、10、9。
    BalancedTree homework;
    for (int key : {17, 13, 22, 9, 16, 20, 27, 3, 12, 15, 18, 10})
        homework.Insert({key, std::to_string(key)});
    require(homework.getRoot()->data.key == 17);
    const int removedKeys[] = {22, 3, 10, 9};
    const int expectedRoots[] = {13, 13, 13, 17};
    // 分别是根部右单旋、局部右左双旋、无需旋转、根部左单旋。
    for (int index = 0; index < 4; ++index) {
        SearchRecord removed;
        require(homework.Remove(removedKeys[index], removed));
        require(removed.key == removedKeys[index]);
        require(homework.getRoot()->data.key == expectedRoots[index]);
        checkAVL(homework.getRoot(), 0, 100);
    }

    BalancedTree tree;
    // 第 92-94 页插入序列，每步检查次序、高度和平衡因子。
    const int keys[] = {16, 3, 7, 11, 9, 26, 18, 14, 15};
    for (int key : keys) {
        require(tree.Insert({key, std::to_string(key)}));
        require(checkAVL(tree.getRoot(), 0, 100) == tree.Height());
    }
    require(!tree.Insert({16, "duplicate"}));
    AVLNode<SearchRecord, int>* parentNode;
    auto* found = tree.Search(15, parentNode);
    require(found && found->data.other == "15");
    require(!tree.Search(99, parentNode) && parentNode && parentNode->data.key == 26);
    std::cout << "7.3: AVL height=" << tree.Height() << ", in order:\n" << tree;

    BalancedTree copied(tree);
    for (int key : keys) {
        SearchRecord removed;
        require(tree.Remove(key, removed) && removed.key == key && removed.other == std::to_string(key));
        checkAVL(tree.getRoot(), 0, 100);
        require(!tree.Search(key));
    }
    SearchRecord unchanged{99, "unchanged"};
    require(tree.Height() == 0 && !tree.Remove(99, unchanged) && unchanged.other == "unchanged");
    tree = copied;
    checkAVL(tree.getRoot(), 0, 100);
    // 即使经 BST 接口增删或赋值，仍构造 AVLNode 并维护平衡。
    SearchTree& baseView = tree;
    require(baseView.Insert({40, "40"}) && baseView.Remove(3));
    SearchTree unbalanced;
    for (int key = 1; key <= 10; ++key) unbalanced.Insert({key, std::to_string(key)});
    baseView = unbalanced;
    checkAVL(tree.getRoot(), 0, 100);
    copied.makeEmpty();
    require(tree.Search(10));
    BalancedTree inputTree(-1);
    std::istringstream input("3 3 2 2 1 1 -1 stop");
    input >> inputTree;
    require(inputTree.Height() == 2 && !inputTree.Search(-1));
    checkAVL(inputTree.getRoot(), 0, 100);
}

int main() {
    staticSearchExample();
    binarySearchTreeExample();
    avlExample();
    std::cout << "Chapter 7 (2026 preview) checks passed.\n";
}
