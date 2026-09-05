#pragma once

#include <string>

namespace stack_app {
    // 括号匹配：忽略非括号字符。
    bool parenthesesMatched(const std::string &expression);

    // 中缀表达式直接求值。
    // 支持 + - * /、圆括号、整数/小数；不处理一元负号。
    bool evaluateInfix(const std::string &expression, double &result);

    // 后缀表达式求值。各 token 之间用空格分隔，例如：
    // "12 6 2 / 0.5 - *"
    bool evaluatePostfix(const std::string &expression, double &result);

    // 中缀表达式转换为后缀表达式。
    // 输出 token 之间以空格分隔，例如：
    // "12*(6-3.5)" -> "12 6 3.5 - *"
    bool infixToPostfix(const std::string &infix, std::string &postfix);
} // namespace stack_app
