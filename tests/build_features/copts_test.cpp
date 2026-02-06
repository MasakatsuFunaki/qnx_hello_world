// Tests: copts with specific warning/optimization flags
#include <iostream>
#include <cstdint>

// This code deliberately exercises patterns that -Wall -Wextra would flag
// if not written carefully. If it compiles, copts are working.

[[nodiscard]] int compute(int x) {
    return x * 2 + 1;
}

// Test that -std=c++17 is effective
#if __cplusplus >= 201703L
constexpr bool has_cpp17 = true;
#else
constexpr bool has_cpp17 = false;
#endif

int main() {
    std::cout << "=== Copts test ===\n";
    std::cout << "__cplusplus = " << __cplusplus << "\n";
    std::cout << "C++17 available: " << (has_cpp17 ? "yes" : "no") << "\n";

    // Ensure [[nodiscard]] result is used
    int val = compute(21);
    std::cout << "compute(21) = " << val << "\n";

    std::cout << "Copts test passed.\n";
    return 0;
}
