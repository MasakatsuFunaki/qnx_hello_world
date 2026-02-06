// Tests the multi-level library dependency chain:
//   app -> config -> logger
#include <iostream>
#include "app.h"

int main() {
    std::cout << "=== Library chain test (app -> config -> logger) ===\n";

    app::Application myapp("TestApp");
    myapp.configure("host", "192.168.1.1");
    myapp.configure("port", "8080");
    myapp.configure("debug", "true");

    std::cout << "host = " << myapp.setting("host") << "\n";
    std::cout << "port = " << myapp.setting("port") << "\n";
    std::cout << "missing = " << myapp.setting("missing") << "\n";

    myapp.run();

    // Verify logging happened through the chain
    auto& log = myapp.get_logger();
    std::cout << "\nLog entries (" << log.count() << "):\n";
    for (const auto& entry : log.entries()) {
        std::cout << "  [" << logger::Logger::level_name(entry.level)
                  << "] " << entry.message << "\n";
    }

    std::cout << "\nLibrary chain test passed.\n";
    return 0;
}
