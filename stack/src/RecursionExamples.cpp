#include "stack/include/RecursionExamples.h"

#include <ostream>
#include <stdexcept>

namespace recursion_app {

long Factorial(long n) {
    if (n < 0)
        throw std::invalid_argument("factorial requires n >= 0");
    if (n == 0)
        return 1;
    return n * Factorial(n - 1);
}

void Hanoi(int n, char A, char B, char C, std::ostream& os) {
    if (n <= 0)
        throw std::invalid_argument("Hanoi requires n > 0");

    if (n == 1) {
        os << "move " << A << " to " << C << '\n';
        return;
    }

    Hanoi(n - 1, A, C, B, os);
    os << "move " << A << " to " << C << '\n';
    Hanoi(n - 1, B, A, C, os);
}

long Fib(long n) {
    if (n < 0)
        throw std::invalid_argument("Fib requires n >= 0");
    if (n <= 1)
        return n;
    return Fib(n - 1) + Fib(n - 2);
}

long FibIter(long n) {
    if (n < 0)
        throw std::invalid_argument("FibIter requires n >= 0");
    if (n <= 1)
        return n;

    long twoBack = 0;
    long oneBack = 1;
    long current = 0;

    for (long i = 2; i <= n; ++i) {
        current = twoBack + oneBack;
        twoBack = oneBack;
        oneBack = current;
    }
    return current;
}

void printReverseRecursive(const int A[], int n, std::ostream& os) {
    if (n >= 0) {
        os << A[n] << ' ';
        printReverseRecursive(A, n - 1, os);
    }
}

void printReverseIterative(const int A[], int n, std::ostream& os) {
    while (n >= 0) {
        os << A[n] << ' ';
        --n;
    }
}

} // namespace recursion_app
