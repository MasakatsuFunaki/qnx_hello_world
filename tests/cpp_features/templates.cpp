// Tests: templates, variadic templates, SFINAE, template specialization
#include <iostream>
#include <string>
#include <type_traits>

// ── Basic class template ────────────────────────────────────────────────────
template <typename T>
class Container {
public:
    explicit Container(T val) : value_(std::move(val)) {}
    const T& get() const { return value_; }
private:
    T value_;
};

// ── Template specialization ─────────────────────────────────────────────────
template <>
class Container<bool> {
public:
    explicit Container(bool val) : value_(val) {}
    bool get() const { return value_; }
    const char* as_string() const { return value_ ? "true" : "false"; }
private:
    bool value_;
};

// ── Variadic templates ──────────────────────────────────────────────────────
template <typename... Args>
constexpr std::size_t count_args(Args&&...) {
    return sizeof...(Args);
}

template <typename T>
T sum_impl(T v) { return v; }

template <typename T, typename... Rest>
T sum_impl(T first, Rest... rest) {
    return first + sum_impl(rest...);
}

// ── SFINAE / enable_if ──────────────────────────────────────────────────────
template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
double_value(T v) { return v * 2; }

template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type
double_value(T v) { return v * 2.0; }

// ── Partial specialization ──────────────────────────────────────────────────
template <typename T, typename U>
struct Pair {
    static const char* describe() { return "generic pair"; }
};

template <typename T>
struct Pair<T, T> {
    static const char* describe() { return "homogeneous pair"; }
};

int main() {
    Container<int> ci(42);
    Container<std::string> cs("hello");
    Container<bool> cb(true);

    std::cout << "int container: " << ci.get() << "\n";
    std::cout << "string container: " << cs.get() << "\n";
    std::cout << "bool container: " << cb.as_string() << "\n";

    std::cout << "count_args(1,2,3): " << count_args(1, 2, 3) << "\n";
    std::cout << "sum(1,2,3,4): " << sum_impl(1, 2, 3, 4) << "\n";

    std::cout << "double_value(int 5): " << double_value(5) << "\n";
    std::cout << "double_value(double 2.5): " << double_value(2.5) << "\n";

    std::cout << "Pair<int,double>: " << Pair<int, double>::describe() << "\n";
    std::cout << "Pair<int,int>: " << Pair<int, int>::describe() << "\n";

    return 0;
}
