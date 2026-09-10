#include "queue/include/QueueApplications.h"
#include "queue/include/LinkedQueue.h"

#include <ostream>

namespace queue_app {
    void printYangHui(int n, std::ostream &os) {
        if (n <= 0)
            return;

        LinkedQueue<int> q;
        int s = 0;
        int t = 0;

        // 初始队列：i = 1 -> 1 1
        q.EnQueue(1);
        q.EnQueue(1);

        for (int i = 1; i <= n; ++i) {
            q.EnQueue(0); // 相邻两行之间补一个 0

            for (int j = 1; j <= i + 2; ++j) {
                q.DeQueue(t);
                q.EnQueue(s + t);
                s = t;

                // 最后弹出的 0 只用于生成下一行，不属于当前行。
                if (j != i + 2) {
                    os << s;
                    if (j != i + 1)
                        os << ' ';
                }
            }
            os << '\n';
        }
    }
} // namespace queue_app
