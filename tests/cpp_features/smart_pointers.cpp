// Tests: smart pointers – unique_ptr, shared_ptr, weak_ptr, custom deleters
#include <iostream>
#include <memory>
#include <vector>
#include <string>

struct Resource {
    std::string name;
    Resource(std::string n) : name(std::move(n)) {
        std::cout << "  Resource(" << name << ") created\n";
    }
    ~Resource() {
        std::cout << "  Resource(" << name << ") destroyed\n";
    }
    void use() const {
        std::cout << "  Using resource: " << name << "\n";
    }
};

// ── Custom deleter ──────────────────────────────────────────────────────────
struct ArrayDeleter {
    void operator()(int* p) const {
        std::cout << "  ArrayDeleter: deleting array\n";
        delete[] p;
    }
};

// ── Factory returning unique_ptr ────────────────────────────────────────────
std::unique_ptr<Resource> make_resource(const std::string& name) {
    return std::make_unique<Resource>(name);
}

int main() {
    std::cout << "=== unique_ptr ===\n";
    {
        auto r1 = std::make_unique<Resource>("unique_1");
        r1->use();

        // Transfer ownership
        auto r2 = std::move(r1);
        std::cout << "  r1 is null: " << (r1 == nullptr) << "\n";
        r2->use();

        // Factory
        auto r3 = make_resource("from_factory");
        r3->use();
    }
    std::cout << "(all unique_ptrs destroyed)\n\n";

    std::cout << "=== shared_ptr ===\n";
    {
        auto s1 = std::make_shared<Resource>("shared_1");
        std::cout << "  ref count: " << s1.use_count() << "\n";

        {
            auto s2 = s1;  // copy
            std::cout << "  ref count after copy: " << s1.use_count() << "\n";
            s2->use();
        }
        std::cout << "  ref count after s2 scope: " << s1.use_count() << "\n";
    }
    std::cout << "(shared_ptr destroyed)\n\n";

    std::cout << "=== weak_ptr ===\n";
    {
        std::weak_ptr<Resource> weak;
        {
            auto shared = std::make_shared<Resource>("weak_target");
            weak = shared;
            std::cout << "  weak expired? " << weak.expired() << "\n";
            if (auto locked = weak.lock()) {
                locked->use();
            }
        }
        std::cout << "  weak expired after shared gone? " << weak.expired() << "\n";
    }

    std::cout << "\n=== unique_ptr with custom deleter ===\n";
    {
        std::unique_ptr<int[], ArrayDeleter> arr(new int[5]);
        for (int i = 0; i < 5; ++i) arr[i] = i * 10;
        std::cout << "  arr[3] = " << arr[3] << "\n";
    }

    std::cout << "\n=== unique_ptr in container ===\n";
    {
        std::vector<std::unique_ptr<Resource>> vec;
        vec.push_back(std::make_unique<Resource>("vec_0"));
        vec.push_back(std::make_unique<Resource>("vec_1"));
        vec.push_back(std::make_unique<Resource>("vec_2"));
        for (const auto& r : vec) r->use();
    }
    std::cout << "(vector of unique_ptrs destroyed)\n";

    return 0;
}
