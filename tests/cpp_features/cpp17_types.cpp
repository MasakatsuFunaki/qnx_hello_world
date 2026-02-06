// Tests: optional, variant, any, string_view, std::apply, std::invoke (C++17)
#include <iostream>
#include <optional>
#include <variant>
#include <any>
#include <string>
#include <string_view>
#include <functional>
#include <tuple>
#include <vector>
#include <map>

// ── std::optional ───────────────────────────────────────────────────────────
std::optional<int> find_even(const std::vector<int>& v) {
    for (auto x : v) {
        if (x % 2 == 0) return x;
    }
    return std::nullopt;
}

// ── std::variant visitor ────────────────────────────────────────────────────
using Value = std::variant<int, double, std::string>;

struct ValuePrinter {
    void operator()(int v) const    { std::cout << "  int: " << v << "\n"; }
    void operator()(double v) const { std::cout << "  double: " << v << "\n"; }
    void operator()(const std::string& v) const { std::cout << "  string: " << v << "\n"; }
};

// ── string_view processing ──────────────────────────────────────────────────
bool starts_with(std::string_view sv, std::string_view prefix) {
    return sv.substr(0, prefix.size()) == prefix;
}

std::string_view trim_left(std::string_view sv) {
    auto pos = sv.find_first_not_of(" \t\n");
    return (pos == std::string_view::npos) ? "" : sv.substr(pos);
}

// ── std::apply ──────────────────────────────────────────────────────────────
int add3(int a, int b, int c) { return a + b + c; }

int main() {
    // std::optional
    std::cout << "=== std::optional ===\n";
    auto r1 = find_even({1, 3, 5, 4, 7});
    auto r2 = find_even({1, 3, 5, 7});
    std::cout << "  find_even({1,3,5,4,7}): "
              << (r1.has_value() ? std::to_string(*r1) : "none") << "\n";
    std::cout << "  find_even({1,3,5,7}): "
              << (r2.has_value() ? std::to_string(*r2) : "none") << "\n";
    std::cout << "  value_or: " << r2.value_or(-1) << "\n";

    // std::variant
    std::cout << "\n=== std::variant ===\n";
    std::vector<Value> values = {42, 3.14, std::string("hello")};
    for (const auto& v : values) {
        std::visit(ValuePrinter{}, v);
    }
    // index() and holds_alternative
    Value v = 42;
    std::cout << "  index: " << v.index() << "\n";
    std::cout << "  holds int: " << std::holds_alternative<int>(v) << "\n";
    std::cout << "  get<int>: " << std::get<int>(v) << "\n";

    // std::any
    std::cout << "\n=== std::any ===\n";
    std::any a = 42;
    std::cout << "  type: " << a.type().name() << "\n";
    std::cout << "  value: " << std::any_cast<int>(a) << "\n";
    a = std::string("switched to string");
    std::cout << "  type: " << a.type().name() << "\n";
    std::cout << "  value: " << std::any_cast<std::string>(a) << "\n";
    try {
        std::any_cast<int>(a);  // should throw
    } catch (const std::bad_any_cast& e) {
        std::cout << "  bad_any_cast: " << e.what() << "\n";
    }

    // std::string_view
    std::cout << "\n=== std::string_view ===\n";
    std::string_view sv = "Hello, QNX World!";
    std::cout << "  sv: " << sv << "\n";
    std::cout << "  starts_with 'Hello': " << starts_with(sv, "Hello") << "\n";
    std::cout << "  starts_with 'QNX': " << starts_with(sv, "QNX") << "\n";
    std::cout << "  trim_left: '" << trim_left("   trimmed") << "'\n";

    // std::apply
    std::cout << "\n=== std::apply ===\n";
    auto tup = std::make_tuple(1, 2, 3);
    std::cout << "  apply(add3, (1,2,3)) = " << std::apply(add3, tup) << "\n";

    // std::invoke
    std::cout << "\n=== std::invoke ===\n";
    auto result = std::invoke([](int x) { return x * x; }, 7);
    std::cout << "  invoke(lambda, 7) = " << result << "\n";

    std::cout << "\nAll C++17 type tests passed.\n";
    return 0;
}
