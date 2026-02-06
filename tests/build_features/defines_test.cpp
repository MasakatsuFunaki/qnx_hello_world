// Tests: copts, defines, select(), includes, linkopts
// This file expects specific -D macros to be set via BUILD
#include <iostream>
#include <string>

int main() {
    std::cout << "=== Build features test ===\n";

    // Test defines passed via copts/defines
#ifdef TEST_DEFINE_ONE
    std::cout << "TEST_DEFINE_ONE is defined\n";
#else
    std::cout << "ERROR: TEST_DEFINE_ONE not defined!\n";
    return 1;
#endif

#ifdef TEST_DEFINE_TWO
    std::cout << "TEST_DEFINE_TWO = " << TEST_DEFINE_TWO << "\n";
#else
    std::cout << "ERROR: TEST_DEFINE_TWO not defined!\n";
    return 1;
#endif

    // Test platform-specific select()
#ifdef BUILD_FOR_QNX
    std::cout << "Platform: QNX (selected via select())\n";
#elif defined(BUILD_FOR_LINUX)
    std::cout << "Platform: Linux (selected via select())\n";
#else
    std::cout << "Platform: Unknown (no select matched)\n";
#endif

    // Test optimization level detection
#ifdef NDEBUG
    std::cout << "Build mode: release (NDEBUG defined)\n";
#else
    std::cout << "Build mode: debug (NDEBUG not defined)\n";
#endif

    // Test APP_VERSION define
#ifdef APP_VERSION
    std::cout << "APP_VERSION = " << APP_VERSION << "\n";
#else
    std::cout << "APP_VERSION not defined (expected)\n";
#endif

    std::cout << "\nBuild features test passed.\n";
    return 0;
}
