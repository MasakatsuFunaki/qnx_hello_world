// Tests: lambdas, closures, generic lambdas, mutable lambdas, IIFE
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>

int main() {
    // ── Basic lambda ────────────────────────────────────────────────────────
    auto greet = []() { return "Hello from lambda!"; };
    std::cout << greet() << "\n";

    // ── Capture by value ────────────────────────────────────────────────────
    int x = 10;
    auto add_x = [x](int y) { return x + y; };
    std::cout << "add_x(5) = " << add_x(5) << "\n";

    // ── Capture by reference ────────────────────────────────────────────────
    int counter = 0;
    auto increment = [&counter]() { ++counter; };
    increment();
    increment();
    increment();
    std::cout << "counter after 3 increments: " << counter << "\n";

    // ── Mutable lambda (capture by value, but modifiable copy) ──────────────
    int seed = 100;
    auto mutable_lambda = [seed]() mutable {
        seed += 10;
        return seed;
    };
    std::cout << "mutable call 1: " << mutable_lambda() << "\n";
    std::cout << "mutable call 2: " << mutable_lambda() << "\n";
    std::cout << "original seed unchanged: " << seed << "\n";

    // ── Generic lambda (C++14) ──────────────────────────────────────────────
    auto generic_add = [](auto a, auto b) { return a + b; };
    std::cout << "generic_add(3,4) = " << generic_add(3, 4) << "\n";
    std::cout << "generic_add(1.5,2.5) = " << generic_add(1.5, 2.5) << "\n";
    std::cout << "generic_add(strings) = " << generic_add(std::string("foo"), std::string("bar")) << "\n";

    // ── Lambda with std::function ───────────────────────────────────────────
    std::function<int(int, int)> op = [](int a, int b) { return a * b; };
    std::cout << "std::function op(6,7) = " << op(6, 7) << "\n";

    // ── Lambda in STL algorithm ─────────────────────────────────────────────
    std::vector<int> nums = {5, 3, 8, 1, 9, 2};
    std::sort(nums.begin(), nums.end(), [](int a, int b) { return a > b; });
    std::cout << "sorted descending: ";
    for (auto n : nums) std::cout << n << " ";
    std::cout << "\n";

    // ── IIFE (Immediately Invoked Function Expression) ──────────────────────
    const auto result = [&]() {
        int total = 0;
        for (auto n : nums) total += n;
        return total;
    }();
    std::cout << "IIFE sum = " << result << "\n";

    // ── Init-capture (C++14) ────────────────────────────────────────────────
    auto init_cap = [val = 42]() { return val; };
    std::cout << "init-capture val = " << init_cap() << "\n";

    // ── Returning lambda from function (via auto) ───────────────────────────
    auto make_adder = [](int base) {
        return [base](int x) { return base + x; };
    };
    auto add10 = make_adder(10);
    std::cout << "add10(25) = " << add10(25) << "\n";

    return 0;
}
