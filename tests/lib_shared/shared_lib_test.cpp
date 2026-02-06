// Tests the shared/dynamic library
#include <iostream>
#include <vector>
#include <string>
#include "stringutils.h"

int main() {
    std::cout << "=== Shared library test ===\n";

    std::cout << "to_upper: " << stringutils::to_upper("hello qnx") << "\n";
    std::cout << "to_lower: " << stringutils::to_lower("HELLO QNX") << "\n";
    std::cout << "trim: '" << stringutils::trim("  spaced  ") << "'\n";

    auto parts = stringutils::split("one,two,three", ',');
    std::cout << "split: ";
    for (const auto& p : parts) std::cout << "[" << p << "] ";
    std::cout << "\n";

    std::cout << "join: " << stringutils::join({"a", "b", "c"}, "-") << "\n";
    std::cout << "contains: " << stringutils::contains("hello world", "world") << "\n";
    std::cout << "replace_all: " << stringutils::replace_all("foo bar foo", "foo", "baz") << "\n";

    std::cout << "\nShared library test passed.\n";
    return 0;
}
