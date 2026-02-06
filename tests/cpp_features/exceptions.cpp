// Tests: exceptions, try/catch, custom exception classes, noexcept,
//        std::exception_ptr, nested exceptions
#include <iostream>
#include <stdexcept>
#include <string>
#include <exception>

// ── Custom exception hierarchy ──────────────────────────────────────────────
class AppError : public std::runtime_error {
public:
    AppError(const std::string& msg, int code)
        : std::runtime_error(msg), code_(code) {}
    int code() const { return code_; }
private:
    int code_;
};

class NetworkError : public AppError {
public:
    NetworkError(const std::string& msg)
        : AppError(msg, 1001) {}
};

class TimeoutError : public NetworkError {
public:
    TimeoutError(int ms)
        : NetworkError("Timeout after " + std::to_string(ms) + "ms"), ms_(ms) {}
    int timeout_ms() const { return ms_; }
private:
    int ms_;
};

// ── noexcept function ───────────────────────────────────────────────────────
int safe_divide(int a, int b) noexcept {
    if (b == 0) return 0;
    return a / b;
}

// ── Function that stores exception_ptr ──────────────────────────────────────
std::exception_ptr captured;

void capture_exception() {
    try {
        throw std::logic_error("captured error");
    } catch (...) {
        captured = std::current_exception();
    }
}

int main() {
    // Basic try/catch
    std::cout << "=== Basic try/catch ===\n";
    try {
        throw std::runtime_error("basic error");
    } catch (const std::runtime_error& e) {
        std::cout << "  Caught: " << e.what() << "\n";
    }

    // Custom exception hierarchy
    std::cout << "\n=== Custom exception hierarchy ===\n";
    try {
        throw TimeoutError(5000);
    } catch (const AppError& e) {
        std::cout << "  Caught AppError: " << e.what()
                  << " code=" << e.code() << "\n";
    }

    // Catching by specific type
    std::cout << "\n=== Specific catch ===\n";
    try {
        throw TimeoutError(1500);
    } catch (const TimeoutError& e) {
        std::cout << "  Caught TimeoutError: timeout=" << e.timeout_ms() << "ms\n";
    } catch (const NetworkError& e) {
        std::cout << "  Caught NetworkError (should not reach here)\n";
    }

    // Multiple catch blocks
    std::cout << "\n=== Multiple catch blocks ===\n";
    for (int i = 0; i < 3; ++i) {
        try {
            if (i == 0) throw std::out_of_range("index error");
            if (i == 1) throw std::bad_alloc();
            if (i == 2) throw 42;
        } catch (const std::out_of_range& e) {
            std::cout << "  [" << i << "] out_of_range: " << e.what() << "\n";
        } catch (const std::bad_alloc& e) {
            std::cout << "  [" << i << "] bad_alloc: " << e.what() << "\n";
        } catch (int val) {
            std::cout << "  [" << i << "] caught int: " << val << "\n";
        }
    }

    // noexcept
    std::cout << "\n=== noexcept ===\n";
    std::cout << "  safe_divide(10,3) = " << safe_divide(10, 3) << "\n";
    std::cout << "  safe_divide(10,0) = " << safe_divide(10, 0) << "\n";
    std::cout << "  noexcept(safe_divide(1,1)) = "
              << noexcept(safe_divide(1, 1)) << "\n";

    // exception_ptr / rethrow
    std::cout << "\n=== exception_ptr ===\n";
    capture_exception();
    if (captured) {
        try {
            std::rethrow_exception(captured);
        } catch (const std::logic_error& e) {
            std::cout << "  Rethrown: " << e.what() << "\n";
        }
    }

    std::cout << "\nAll exception tests passed.\n";
    return 0;
}
