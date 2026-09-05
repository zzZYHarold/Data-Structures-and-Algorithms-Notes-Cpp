#include "stack/include/StackApplications.h"

#include "stack/include/SeqStack.h"

#include <cctype>
#include <cmath>
#include <sstream>
#include <string>

namespace stack_app {
    namespace {
        bool isLeftBracket(char c) {
            return c == '(' || c == '[' || c == '{';
        }

        bool isRightBracket(char c) {
            return c == ')' || c == ']' || c == '}';
        }

        bool bracketsMatch(char left, char right) {
            return (left == '(' && right == ')') ||
                   (left == '[' && right == ']') ||
                   (left == '{' && right == '}');
        }

        bool isOperator(char c) {
            return c == '+' || c == '-' || c == '*' || c == '/';
        }

        int precedence(char op) {
            if (op == '*' || op == '/') {
                return 2;
            }
            if (op == '+' || op == '-') {
                return 1;
            }
            return 0;
        }

        bool calculate(double left, char op, double right, double &value) {
            switch (op) {
                case '+': value = left + right;
                    return true;
                case '-': value = left - right;
                    return true;
                case '*': value = left * right;
                    return true;
                case '/':
                    if (std::abs(right) < 1e-12) {
                        return false;
                    }
                    value = left / right;
                    return true;
                default:
                    return false;
            }
        }

        bool applyTopOperator(SeqStack<double> &operands, SeqStack<char> &operators) {
            char op;
            double right;
            double left;
            double value;

            if (!operators.Pop(op) ||
                !operands.Pop(right) ||
                !operands.Pop(left) ||
                !calculate(left, op, right, value)) {
                return false;
            }

            operands.Push(value);
            return true;
        }

        bool parseNumber(const std::string &text, std::size_t &i, double &value, std::string *token = nullptr) {
            const std::size_t start = i;
            bool hasDigit = false;
            bool hasDot = false;

            while (i < text.size()) {
                const char c = text[i];
                if (std::isdigit(static_cast<unsigned char>(c))) {
                    hasDigit = true;
                    ++i;
                } else if (c == '.' && !hasDot) {
                    hasDot = true;
                    ++i;
                } else {
                    break;
                }
            }

            if (!hasDigit) {
                i = start;
                return false;
            }

            const std::string numberText = text.substr(start, i - start);
            try {
                std::size_t parsed = 0;
                value = std::stod(numberText, &parsed);
                if (parsed != numberText.size()) {
                    return false;
                }
            } catch (...) {
                return false;
            }

            if (token != nullptr) {
                *token = numberText;
            }
            return true;
        }
    } // namespace

    bool parenthesesMatched(const std::string &expression) {
        SeqStack<char> stack;

        for (char c: expression) {
            if (isLeftBracket(c)) {
                stack.Push(c);
            } else if (isRightBracket(c)) {
                char left;
                if (!stack.Pop(left) || !bracketsMatch(left, c)) {
                    return false;
                }
            }
        }

        return stack.IsEmpty();
    }

    bool evaluateInfix(const std::string &expression, double &result) {
        SeqStack<double> operands;
        SeqStack<char> operators;

        std::size_t i = 0;
        while (i < expression.size()) {
            const char c = expression[i];

            if (std::isspace(static_cast<unsigned char>(c))) {
                ++i;
                continue;
            }

            // 课件示例以 '=' 表示表达式结束。
            if (c == '=') {
                ++i;
                while (i < expression.size()) {
                    if (!std::isspace(static_cast<unsigned char>(expression[i]))) {
                        return false;
                    }
                    ++i;
                }
                break;
            }

            if (std::isdigit(static_cast<unsigned char>(c)) || c == '.') {
                double value;
                if (!parseNumber(expression, i, value)) {
                    return false;
                }
                operands.Push(value);
                continue;
            }

            if (c == '(') {
                operators.Push(c);
                ++i;
                continue;
            }

            if (c == ')') {
                char topOp;
                bool foundLeftParen = false;
                while (operators.getTop(topOp)) {
                    if (topOp == '(') {
                        operators.Pop(topOp);
                        foundLeftParen = true;
                        break;
                    }
                    if (!applyTopOperator(operands, operators)) {
                        return false;
                    }
                }
                if (!foundLeftParen) {
                    return false;
                }
                ++i;
                continue;
            }

            if (isOperator(c)) {
                char topOp;
                while (operators.getTop(topOp) &&
                       topOp != '(' &&
                       precedence(topOp) >= precedence(c)) {
                    if (!applyTopOperator(operands, operators)) {
                        return false;
                    }
                }
                operators.Push(c);
                ++i;
                continue;
            }

            return false;
        }

        char topOp;
        while (operators.getTop(topOp)) {
            if (topOp == '(') {
                return false;
            }
            if (!applyTopOperator(operands, operators)) {
                return false;
            }
        }

        if (operands.getSize() != 1) {
            return false;
        }
        return operands.Pop(result);
    }

    bool evaluatePostfix(const std::string &expression, double &result) {
        SeqStack<double> operands;
        std::istringstream in(expression);
        std::string token;

        while (in >> token) {
            if (token.size() == 1 && isOperator(token[0])) {
                double right;
                double left;
                double value;
                if (!operands.Pop(right) ||
                    !operands.Pop(left) ||
                    !calculate(left, token[0], right, value)) {
                    return false;
                }
                operands.Push(value);
                continue;
            }

            try {
                std::size_t parsed = 0;
                const double value = std::stod(token, &parsed);
                if (parsed != token.size()) {
                    return false;
                }
                operands.Push(value);
            } catch (...) {
                return false;
            }
        }

        if (operands.getSize() != 1) {
            return false;
        }
        return operands.Pop(result);
    }

    bool infixToPostfix(const std::string &infix, std::string &postfix) {
        SeqStack<char> operators;
        std::ostringstream out;
        bool firstToken = true;

        auto emit = [&](const std::string &token) {
            if (!firstToken) {
                out << ' ';
            }
            out << token;
            firstToken = false;
        };

        std::size_t i = 0;
        while (i < infix.size()) {
            const char c = infix[i];

            if (std::isspace(static_cast<unsigned char>(c))) {
                ++i;
                continue;
            }

            if (c == '=' || c == '#') {
                ++i;
                while (i < infix.size()) {
                    if (!std::isspace(static_cast<unsigned char>(infix[i]))) {
                        return false;
                    }
                    ++i;
                }
                break;
            }

            if (std::isdigit(static_cast<unsigned char>(c)) || c == '.') {
                double value;
                std::string token;
                if (!parseNumber(infix, i, value, &token)) {
                    return false;
                }
                emit(token);
                continue;
            }

            if (c == '(') {
                operators.Push(c);
                ++i;
                continue;
            }

            if (c == ')') {
                char topOp;
                bool foundLeftParen = false;
                while (operators.Pop(topOp)) {
                    if (topOp == '(') {
                        foundLeftParen = true;
                        break;
                    }
                    emit(std::string(1, topOp));
                }
                if (!foundLeftParen) {
                    return false;
                }
                ++i;
                continue;
            }

            if (isOperator(c)) {
                char topOp;
                while (operators.getTop(topOp) &&
                       topOp != '(' &&
                       precedence(topOp) >= precedence(c)) {
                    operators.Pop(topOp);
                    emit(std::string(1, topOp));
                }
                operators.Push(c);
                ++i;
                continue;
            }

            return false;
        }

        char topOp;
        while (operators.Pop(topOp)) {
            if (topOp == '(') {
                return false;
            }
            emit(std::string(1, topOp));
        }

        postfix = out.str();
        return !postfix.empty();
    }
} // namespace stack_app
