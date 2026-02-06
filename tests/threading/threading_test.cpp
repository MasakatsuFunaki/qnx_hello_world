// Tests: std::thread, std::mutex, std::lock_guard, std::unique_lock,
//        std::condition_variable, std::atomic, std::async, std::future,
//        std::promise, thread_local
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <future>
#include <vector>
#include <queue>
#include <chrono>
#include <functional>
#include <numeric>

// ── Basic mutex + lock_guard ────────────────────────────────────────────────
std::mutex g_mtx;
int g_counter = 0;

void increment_n(int n) {
    for (int i = 0; i < n; ++i) {
        std::lock_guard<std::mutex> lock(g_mtx);
        ++g_counter;
    }
}

// ── Atomic ──────────────────────────────────────────────────────────────────
std::atomic<int> g_atomic_counter{0};

void atomic_increment(int n) {
    for (int i = 0; i < n; ++i) {
        g_atomic_counter.fetch_add(1, std::memory_order_relaxed);
    }
}

// ── Condition variable (producer/consumer) ──────────────────────────────────
std::queue<int> g_queue;
std::mutex g_queue_mtx;
std::condition_variable g_cv;
bool g_done = false;

void producer(int count) {
    for (int i = 0; i < count; ++i) {
        {
            std::lock_guard<std::mutex> lock(g_queue_mtx);
            g_queue.push(i);
        }
        g_cv.notify_one();
    }
    {
        std::lock_guard<std::mutex> lock(g_queue_mtx);
        g_done = true;
    }
    g_cv.notify_all();
}

int consumer() {
    int total = 0;
    while (true) {
        std::unique_lock<std::mutex> lock(g_queue_mtx);
        g_cv.wait(lock, [] { return !g_queue.empty() || g_done; });
        while (!g_queue.empty()) {
            total += g_queue.front();
            g_queue.pop();
        }
        if (g_done && g_queue.empty()) break;
    }
    return total;
}

// ── thread_local ────────────────────────────────────────────────────────────
thread_local int tl_value = 0;

void set_thread_local(int val) {
    tl_value = val;
    std::cout << "  thread " << std::this_thread::get_id()
              << " tl_value=" << tl_value << "\n";
}

int main() {
    // ── Multiple threads with mutex ─────────────────────────────────────────
    std::cout << "=== Threads with mutex ===\n";
    {
        const int per_thread = 10000;
        std::vector<std::thread> threads;
        for (int i = 0; i < 4; ++i) {
            threads.emplace_back(increment_n, per_thread);
        }
        for (auto& t : threads) t.join();
        std::cout << "  g_counter (expected 40000): " << g_counter << "\n";
    }

    // ── Atomic operations ───────────────────────────────────────────────────
    std::cout << "\n=== Atomic operations ===\n";
    {
        const int per_thread = 10000;
        std::vector<std::thread> threads;
        for (int i = 0; i < 4; ++i) {
            threads.emplace_back(atomic_increment, per_thread);
        }
        for (auto& t : threads) t.join();
        std::cout << "  atomic_counter (expected 40000): "
                  << g_atomic_counter.load() << "\n";
    }

    // ── Condition variable ──────────────────────────────────────────────────
    std::cout << "\n=== Condition variable ===\n";
    {
        const int count = 100;
        std::thread prod(producer, count);
        int result = 0;
        std::thread cons([&result]() { result = consumer(); });
        prod.join();
        cons.join();
        int expected = (count - 1) * count / 2;
        std::cout << "  consumer total (expected " << expected << "): " << result << "\n";
    }

    // ── std::async / std::future ────────────────────────────────────────────
    std::cout << "\n=== std::async / std::future ===\n";
    {
        auto fut = std::async(std::launch::async, []() {
            int sum = 0;
            for (int i = 1; i <= 100; ++i) sum += i;
            return sum;
        });
        std::cout << "  async sum(1..100) = " << fut.get() << "\n";
    }

    // ── std::promise / std::future ──────────────────────────────────────────
    std::cout << "\n=== std::promise / std::future ===\n";
    {
        std::promise<std::string> prom;
        auto fut = prom.get_future();

        std::thread t([&prom]() {
            prom.set_value("Hello from promise!");
        });

        std::cout << "  promise value: " << fut.get() << "\n";
        t.join();
    }

    // ── thread_local ────────────────────────────────────────────────────────
    std::cout << "\n=== thread_local ===\n";
    {
        std::thread t1(set_thread_local, 100);
        std::thread t2(set_thread_local, 200);
        t1.join();
        t2.join();
        std::cout << "  main tl_value (should be 0): " << tl_value << "\n";
    }

    // ── std::chrono ─────────────────────────────────────────────────────────
    std::cout << "\n=== std::chrono ===\n";
    {
        auto start = std::chrono::steady_clock::now();
        volatile int x = 0;
        for (int i = 0; i < 1000000; ++i) x += i;
        auto end = std::chrono::steady_clock::now();
        auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "  loop took " << us << " microseconds\n";
    }

    std::cout << "\nAll threading tests passed.\n";
    return 0;
}
