# 第三章 3.3~3.5：队列、优先级队列与双端队列

本目录统一整理第三章剩余的队列族内容：

- 3.3 队列：循环队列、链式队列、杨辉三角应用；
- 3.4 优先级队列：数组有序实现；
- 3.5 双端队列：基于循环队列的数组实现。

本次没有创建 `examples/`。这些实现本身已经足够完整，不需要为了“可运行”额外增加 `main.cpp`。

## 文件说明

| 小节 | 课件内容 | 文件 |
|---|---|---|
| 3.3 | 队列抽象数据类型 | `include/Queue.h` |
| 3.3 | 顺序（循环）队列 | `include/SeqQueue.h` |
| 3.3 | 链式队列 | `include/LinkedQueue.h` |
| 3.3 | 杨辉三角 | `include/QueueApplications.h` + `src/QueueApplications.cpp` |
| 3.4 | 数组优先级队列 | `include/PQueue.h` |
| 3.5 | 双端队列抽象接口 | `include/Deque.h` |
| 3.5 | 数组双端队列 | `include/SeqDeque.h` |

## 3.3 循环队列

保持课件的核心设计：

```text
front 指向实际队头元素
rear  指向队尾元素的下一个位置

队空：front == rear
队满：(rear + 1) % maxSize == front
元素个数：(rear - front + maxSize) % maxSize
```

由于用一个空位置区分队空和队满，所以 `maxSize` 是底层数组长度，真正最多能保存 `maxSize - 1` 个元素。

## 3.4 优先级队列

保持课件的数组实现：新元素先放到数组末尾，再用类似插入排序的方式调整，使数组保持升序。课件约定“数值越小，优先权越高”，所以 `RemoveMin()` 删除数组第 0 个元素。

相等元素在 `adjust()` 中不会越过已有的相等元素，因此保持 FIFO 次序。

课件明确说明堆实现放到第五章，因此这里没有提前用堆或 `std::priority_queue` 替代。

## 3.5 双端队列

`SeqDeque` 继续复用 `SeqQueue` 中的循环数组以及 `front / rear / maxSize`：

- `EnQueueHead`：`front` 向前移动后写入；
- `EnQueueTail`：复用普通循环队列入队；
- `DeQueueHead`：复用普通循环队列出队；
- `DeQueueTail`：`rear` 向前移动后取出元素。

## 相较课件的必要修正 / 补充

1. **Queue 接口返回类型统一**：课件 `Queue` 中 `EnQueue` 写成 `void`，而 `SeqQueue` / `LinkedQueue` 实现均返回 `bool`。整理时统一为 `bool`，否则无法正常 override。
2. **const 接口统一**：`getFront` 等只读操作补充 `const`，使抽象类与实现保持一致。
3. **链式队列删除最后一个结点**：课件 `DeQueue` 删除最后一个结点后没有把 `rear` 置空，会留下悬空指针；已修正。
4. **链结点命名**：课件栈与队列都使用全局 `LinkNode`。如果同一程序同时包含二者会发生重定义，因此队列中改名为 `QueueNode`。
5. **输出流错误**：课件 `LinkedQueue` 的 `operator<<` 最后使用 `cout << endl`，已改为统一写入传入的 `ostream`。
6. **杨辉三角包含方式**：课件示例 `#include "LinkedQueue.cpp"` 不符合当前仓库的模板组织方式；整理为包含 `LinkedQueue.h`。
7. **Deque 接口补全**：课件声明了普通 `EnQueue / DeQueue` 但未给出实现。这里规定其语义为“队尾入、队头出”，并在 `SeqDeque` 中显式桥接到 `SeqQueue`，同时让 `SeqDeque` 实现 `Deque` 接口。

## CMake

本目录没有新增 `examples/main.cpp`。

按当前根 CMake 的约定，`queue/src/QueueApplications.cpp` 和所有头文件会进入 `course_code` 的全仓库编译检查，因此 **本次 3.3~3.5 不需要新增 `add_executable(...)`**。
