// Tests: constexpr, static_assert, type_traits, auto, structured bindings,
//        if constexpr, fold expressions, enum class, user-defined literals
#include <iostream>
#include <tuple>
#include <array>
#include <type_traits>
#include <string>
#include <cstdint>

// ── constexpr functions ─────────────────────────────────────────────────────
constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

constexpr int fibonacci(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    int a = 0, b = 1;
    for (int i = 2; i <= n; ++i) {
        int tmp = a + b;
        a = b;
        b = tmp;
    }
    return b;
}

// ── static_assert ───────────────────────────────────────────────────────────
static_assert(factorial(5) == 120, "factorial(5) must be 120");
static_assert(fibonacci(10) == 55, "fibonacci(10) must be 55");
static_assert(sizeof(int) >= 4, "int must be at least 32 bits");

// ── enum class ──────────────────────────────────────────────────────────────
enum class Color : uint8_t { Red = 0, Green, Blue };

const char* color_name(Color c) {
    switch (c) {
        case Color::Red:   return "Red";
        case Color::Green: return "Green";
        case Color::Blue:  return "Blue";
    }
    return "Unknown";
}

// ── User-defined literal ────────────────────────────────────────────────────
constexpr long double operator"" _km(long double val) {
    return val * 1000.0L;  // convert to meters
}

constexpr unsigned long long operator"" _KB(unsigned long long val) {
    return val * 1024;
}

// ── if constexpr (C++17) ────────────────────────────────────────────────────
template <typename T>
std::string describe_type(T val) {
    if constexpr (std::is_integral_v<T>) {
        return "integral: " + std::to_string(val);
    } else if constexpr (std::is_floating_point_v<T>) {
        return "floating: " + std::to_string(val);
    } else {
        return "other type";
    }
}

// ── Fold expressions (C++17) ────────────────────────────────────────────────
template <typename... Args>
auto fold_sum(Args... args) {
    return (args + ...);
}

template <typename... Args>
bool all_positive(Args... args) {
    return (... && (args > 0));
}

// ── RAII wrapper ────────────────────────────────────────────────────────────
class ScopeGuard {
public:
    ScopeGuard(const char* name) : name_(name) {
        std::cout << "  ScopeGuard(" << name_ << ") enter\n";
    }
    ~ScopeGuard() {
        std::cout << "  ScopeGuard(" << name_ << ") exit\n";
    }
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
private:
    const char* name_;
};

// ── Move semantics ──────────────────────────────────────────────────────────
class MoveOnly {
public:
    MoveOnly(int v) : data_(new int(v)) {}
    ~MoveOnly() { delete data_; }

    MoveOnly(MoveOnly&& other) noexcept : data_(other.data_) {
        other.data_ = nullptr;
    }
    MoveOnly& operator=(MoveOnly&& other) noexcept {
        if (this != &other) {
            delete data_;
            data_ = other.data_;
            other.data_ = nullptr;
        }
        return *this;
    }

    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;

    int value() const { return data_ ? *data_ : -1; }
private:
    int* data_;
};

int main() {
    // constexpr
    constexpr auto f5 = factorial(5);
    constexpr auto fib10 = fibonacci(10);
    std::cout << "factorial(5) = " << f5 << "\n";
    std::cout << "fibonacci(10) = " << fib10 << "\n";

    // enum class
    Color c = Color::Green;
    std::cout << "color = " << color_name(c) << "\n";

    // User-defined literals
    constexpr auto dist = 5.0_km;
    constexpr auto mem = 64_KB;
    std::cout << "5 km = " << dist << " meters\n";
    std::cout << "64 KB = " << mem << " bytes\n";

    // constexpr array
    constexpr std::array<int, 4> arr = {10, 20, 30, 40};
    static_assert(arr[2] == 30, "arr[2] must be 30");

    // Structured bindings (C++17)
    auto [a, b, c2] = std::make_tuple(1, 2.0, std::string("three"));
    std::cout << "structured bindings: " << a << ", " << b << ", " << c2 << "\n";

    // if constexpr
    std::cout << describe_type(42) << "\n";
    std::cout << describe_type(3.14) << "\n";

    // Fold expressions
    std::cout << "fold_sum(1,2,3,4) = " << fold_sum(1, 2, 3, 4) << "\n";
    std::cout << "all_positive(1,2,3) = " << all_positive(1, 2, 3) << "\n";
    std::cout << "all_positive(1,-2,3) = " << all_positive(1, -2, 3) << "\n";

    // RAII
    {
        ScopeGuard guard("test");
        std::cout << "  inside guarded scope\n";
    }

    // Move semantics
    MoveOnly m1(99);
    MoveOnly m2(std::move(m1));
    std::cout << "m1 after move: " << m1.value() << "\n";
    std::cout << "m2 after move: " << m2.value() << "\n";

    // auto + decltype
    auto x = 42;
    decltype(x) y = 100;
    std::cout << "auto x=" << x << ", decltype y=" << y << "\n";

    return 0;
}
