#include "stack/include/DualStack.h"
#include "stack/include/LinkedStack.h"
#include "stack/include/SeqStack.h"
#include "stack/include/StackApplications.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>

int main() {
    // 1. 顺序栈
    SeqStack<int> seq(2, 2);
    seq.Push(10);
    seq.Push(20);
    seq.Push(30); // 自动扩容

    int x = 0;
    assert(seq.Pop(x) && x == 30);
    assert(seq.getTop(x) && x == 20);

    // 2. 双栈共享空间
    DualStack<int> dual(4);
    assert(dual.Push(1, 0));
    assert(dual.Push(2, 0));
    assert(dual.Push(9, 1));
    assert(dual.Push(8, 1));
    assert(dual.IsFull());

    // 3. 链式栈
    LinkedStack<std::string> linked;
    linked.Push("a");
    linked.Push("b");
    std::string text;
    assert(linked.Pop(text) && text == "b");

    // 4. 括号匹配
    assert(stack_app::parenthesesMatched("{()[({})]}") == true);
    assert(stack_app::parenthesesMatched("([)]") == false);

    // 5. 中缀表达式求值
    double value = 0.0;
    assert(stack_app::evaluateInfix("12*(6-3.5)=", value));
    assert(std::abs(value - 30.0) < 1e-12);

    // 6. 后缀表达式求值
    assert(stack_app::evaluatePostfix("12 6 2 / 0.5 - *", value));
    assert(std::abs(value - 30.0) < 1e-12);

    // 7. 中缀转后缀
    std::string postfix;
    assert(stack_app::infixToPostfix("12*(6-3.5)", postfix));
    assert(postfix == "12 6 3.5 - *");

    std::cout << "All chapter 3.1 stack examples passed.\n";
    return 0;
}
