// Tests the static library by linking and using it
#include <iostream>
#include <vector>
#include <cmath>
#include "mathlib.h"

int main() {
    std::cout << "=== Static library test ===\n";

    // Basic arithmetic
    std::cout << "add(3,4) = " << mathlib::add(3, 4) << "\n";
    std::cout << "multiply(6,7) = " << mathlib::multiply(6, 7) << "\n";
    std::cout << "divide(22,7) = " << mathlib::divide(22.0, 7.0) << "\n";

    // Exception from library
    try {
        mathlib::divide(1.0, 0.0);
    } catch (const std::invalid_argument& e) {
        std::cout << "divide by zero caught: " << e.what() << "\n";
    }

    // Statistical functions
    std::vector<double> data = {2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0};
    std::cout << "mean = " << mathlib::mean(data) << "\n";
    std::cout << "stddev = " << mathlib::standard_deviation(data) << "\n";
    std::cout << "median = " << mathlib::median(data) << "\n";

    // Template function from header
    std::cout << "clamp(15, 0, 10) = " << mathlib::clamp(15, 0, 10) << "\n";
    std::cout << "clamp(-5.0, 0.0, 1.0) = " << mathlib::clamp(-5.0, 0.0, 1.0) << "\n";

    std::cout << "\nStatic library test passed.\n";
    return 0;
}
