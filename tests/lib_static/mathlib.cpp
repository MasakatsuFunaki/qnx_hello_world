// Static library implementation
#include "mathlib.h"

namespace mathlib {

int add(int a, int b) { return a + b; }

int multiply(int a, int b) { return a * b; }

double divide(double a, double b) {
    if (b == 0.0) throw std::invalid_argument("division by zero");
    return a / b;
}

double mean(const std::vector<double>& data) {
    if (data.empty()) throw std::invalid_argument("empty data");
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    return sum / static_cast<double>(data.size());
}

double standard_deviation(const std::vector<double>& data) {
    if (data.size() < 2) throw std::invalid_argument("need at least 2 values");
    double m = mean(data);
    double sq_sum = 0.0;
    for (auto v : data) {
        double diff = v - m;
        sq_sum += diff * diff;
    }
    return std::sqrt(sq_sum / static_cast<double>(data.size() - 1));
}

double median(std::vector<double> data) {
    if (data.empty()) throw std::invalid_argument("empty data");
    std::sort(data.begin(), data.end());
    auto n = data.size();
    if (n % 2 == 0) {
        return (data[n / 2 - 1] + data[n / 2]) / 2.0;
    }
    return data[n / 2];
}

}  // namespace mathlib
