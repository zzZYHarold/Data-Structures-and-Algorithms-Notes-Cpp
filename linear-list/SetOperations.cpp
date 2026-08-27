#include "SetOperations.h"

// 用顺序表实现集合的“并”运算（PPT 第 29 页）
void Union(SeqList<int>& LA, SeqList<int>& LB) {
    int n = LA.Length();
    int m = LB.Length();
    int x;

    for (int i = 1; i <= m; ++i) {
        LB.getData(i, x);       // 在 LB 中取一个元素
        int k = LA.Search(x);   // 在 LA 中搜索它

        if (k == 0) {           // 若未找到，插入到 LA 尾部
            LA.Insert(n, x);    // Insert 的 i 按 0-based 下标解释；n 恰好是尾后位置
            ++n;
        }
    }
}

// 用顺序表实现集合的“交”运算（PPT 第 30 页）
void Intersection(SeqList<int>& LA, SeqList<int>& LB) {
    int n = LA.Length();
    int i = 1;
    int x;

    while (i <= n) {
        LA.getData(i, x);       // 在 LA 中取一个元素
        int k = LB.Search(x);   // 在 LB 中搜索它

        if (k == 0) {
            LA.Remove(i, x);    // 未找到则从 LA 中删除
            --n;
        } else {
            ++i;
        }
    }
}
