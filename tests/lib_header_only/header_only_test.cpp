// Tests the header-only event library
#include <iostream>
#include <string>
#include <unordered_map>
#include "event.hpp"

int main() {
    std::cout << "=== Header-only library test ===\n";

    // Typed event
    events::Event<int, std::string> on_message;

    auto id1 = on_message.subscribe([](int code, const std::string& msg) {
        std::cout << "  handler1: code=" << code << " msg=" << msg << "\n";
    });

    auto id2 = on_message.subscribe([](int code, const std::string& msg) {
        std::cout << "  handler2: code=" << code << " msg=" << msg << "\n";
    });

    std::cout << "subscribers: " << on_message.subscriber_count() << "\n";
    on_message.emit(200, "OK");

    on_message.unsubscribe(id1);
    std::cout << "after unsubscribe: " << on_message.subscriber_count() << "\n";
    on_message.emit(404, "Not Found");

    // Void event
    events::Event<> on_close;
    on_close.subscribe([]() { std::cout << "  closing!\n"; });
    on_close.emit();

    // Event bus
    std::cout << "\n=== EventBus ===\n";
    events::EventBus bus;
    bus.on("start", []() { std::cout << "  started!\n"; });
    bus.on("start", []() { std::cout << "  also started!\n"; });
    bus.on("stop", []() { std::cout << "  stopped!\n"; });
    bus.emit("start");
    bus.emit("stop");
    bus.emit("nonexistent");  // should be a no-op

    std::cout << "\nHeader-only library test passed.\n";
    return 0;
}
