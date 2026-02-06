// Tests strip_include_prefix: includes myapi.h without full path
#include <iostream>
#include "myapi/myapi.h"

int main() {
    std::cout << "=== Include path test ===\n";
    std::cout << "API name: " << myapi::get_name() << "\n";
    std::cout << "API version: " << myapi::get_version() << "\n";
    std::cout << "Include path test passed.\n";
    return 0;
}
