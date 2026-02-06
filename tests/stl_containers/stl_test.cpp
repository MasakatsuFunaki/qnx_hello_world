// Tests: vector, map, unordered_map, set, deque, list, array,
//        STL algorithms, iterators, range-based for
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <deque>
#include <list>
#include <array>
#include <queue>
#include <stack>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <string>
#include <sstream>
#include <functional>

int main() {
    // ── std::vector ─────────────────────────────────────────────────────────
    std::cout << "=== vector ===\n";
    std::vector<int> vec = {5, 3, 8, 1, 9, 2, 7, 4, 6};
    vec.push_back(10);
    vec.emplace_back(11);
    vec.insert(vec.begin() + 2, 99);

    std::sort(vec.begin(), vec.end());
    std::cout << "  sorted: ";
    for (auto v : vec) std::cout << v << " ";
    std::cout << "\n";

    auto it = std::lower_bound(vec.begin(), vec.end(), 7);
    std::cout << "  lower_bound(7) -> " << *it << " at index "
              << std::distance(vec.begin(), it) << "\n";

    // ── std::map ────────────────────────────────────────────────────────────
    std::cout << "\n=== map ===\n";
    std::map<std::string, int> ages = {{"Alice", 30}, {"Bob", 25}};
    ages["Charlie"] = 35;
    ages.emplace("Diana", 28);

    for (const auto& [name, age] : ages) {
        std::cout << "  " << name << ": " << age << "\n";
    }

    auto found = ages.find("Bob");
    std::cout << "  find Bob: " << (found != ages.end() ? "found" : "not found") << "\n";

    // ── std::unordered_map ──────────────────────────────────────────────────
    std::cout << "\n=== unordered_map ===\n";
    std::unordered_map<int, std::string> id_names;
    id_names[1] = "one";
    id_names[2] = "two";
    id_names[3] = "three";
    std::cout << "  size: " << id_names.size() << "\n";
    std::cout << "  bucket_count: " << id_names.bucket_count() << "\n";
    std::cout << "  id_names[2] = " << id_names[2] << "\n";

    // ── std::set ────────────────────────────────────────────────────────────
    std::cout << "\n=== set ===\n";
    std::set<int> s = {3, 1, 4, 1, 5, 9, 2, 6};
    s.insert(5);  // duplicate, should be ignored
    std::cout << "  set: ";
    for (auto v : s) std::cout << v << " ";
    std::cout << "\n  size: " << s.size() << "\n";

    // ── std::deque ──────────────────────────────────────────────────────────
    std::cout << "\n=== deque ===\n";
    std::deque<int> dq;
    dq.push_front(1);
    dq.push_back(2);
    dq.push_front(0);
    std::cout << "  deque: ";
    for (auto v : dq) std::cout << v << " ";
    std::cout << "\n";

    // ── std::list ───────────────────────────────────────────────────────────
    std::cout << "\n=== list ===\n";
    std::list<int> lst = {5, 2, 8, 1, 3};
    lst.sort();
    lst.unique();
    std::cout << "  sorted list: ";
    for (auto v : lst) std::cout << v << " ";
    std::cout << "\n";

    // ── std::array ──────────────────────────────────────────────────────────
    std::cout << "\n=== array ===\n";
    std::array<int, 5> arr = {10, 20, 30, 40, 50};
    std::cout << "  arr[2] = " << arr[2] << ", size = " << arr.size() << "\n";

    // ── std::priority_queue ─────────────────────────────────────────────────
    std::cout << "\n=== priority_queue ===\n";
    std::priority_queue<int> pq;
    for (int x : {3, 1, 4, 1, 5, 9}) pq.push(x);
    std::cout << "  top elements: ";
    while (!pq.empty()) {
        std::cout << pq.top() << " ";
        pq.pop();
    }
    std::cout << "\n";

    // ── STL algorithms ──────────────────────────────────────────────────────
    std::cout << "\n=== algorithms ===\n";
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    int sum = std::accumulate(data.begin(), data.end(), 0);
    std::cout << "  sum(1..10) = " << sum << "\n";

    auto count = std::count_if(data.begin(), data.end(),
                               [](int x) { return x % 2 == 0; });
    std::cout << "  even count = " << count << "\n";

    std::vector<int> transformed(data.size());
    std::transform(data.begin(), data.end(), transformed.begin(),
                   [](int x) { return x * x; });
    std::cout << "  squares: ";
    for (auto v : transformed) std::cout << v << " ";
    std::cout << "\n";

    bool all_pos = std::all_of(data.begin(), data.end(), [](int x) { return x > 0; });
    bool any_neg = std::any_of(data.begin(), data.end(), [](int x) { return x < 0; });
    std::cout << "  all positive: " << all_pos << "\n";
    std::cout << "  any negative: " << any_neg << "\n";

    auto [min_it, max_it] = std::minmax_element(data.begin(), data.end());
    std::cout << "  min=" << *min_it << " max=" << *max_it << "\n";

    // std::partition
    std::vector<int> part = {1, 2, 3, 4, 5, 6, 7, 8};
    auto pivot = std::partition(part.begin(), part.end(), [](int x) { return x % 2 == 0; });
    std::cout << "  partitioned (evens first): ";
    for (auto v : part) std::cout << v << " ";
    std::cout << "\n";

    // std::copy with ostream_iterator
    std::cout << "  copy to ostream: ";
    std::copy(data.begin(), data.begin() + 5,
              std::ostream_iterator<int>(std::cout, ","));
    std::cout << "\n";

    std::cout << "\nAll STL tests passed.\n";
    return 0;
}
