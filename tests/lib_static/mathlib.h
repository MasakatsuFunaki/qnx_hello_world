// Static library: a math utility library
#ifndef MATHLIB_H
#define MATHLIB_H

#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <stdexcept>

namespace mathlib {

// Basic arithmetic
int add(int a, int b);
int multiply(int a, int b);
double divide(double a, double b);

// Statistical functions
double mean(const std::vector<double>& data);
double standard_deviation(const std::vector<double>& data);
double median(std::vector<double> data);  // takes by value intentionally

// Template function (in header)
template <typename T>
T clamp(T value, T low, T high) {
    return std::max(low, std::min(value, high));
}

}  // namespace mathlib

#endif  // MATHLIB_H
