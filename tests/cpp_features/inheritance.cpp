// Tests: virtual functions, abstract classes, multiple inheritance,
//        override, final, dynamic_cast, RTTI
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <typeinfo>

// ── Abstract base ───────────────────────────────────────────────────────────
class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual std::string name() const = 0;

    void print() const {
        std::cout << "  " << name() << " area=" << area() << "\n";
    }
};

// ── Concrete classes ────────────────────────────────────────────────────────
class Circle final : public Shape {
public:
    Circle(double r) : radius_(r) {}
    double area() const override { return 3.14159265 * radius_ * radius_; }
    std::string name() const override { return "Circle"; }
private:
    double radius_;
};

class Rectangle : public Shape {
public:
    Rectangle(double w, double h) : w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
    std::string name() const override { return "Rectangle"; }
protected:
    double w_, h_;
};

class Square final : public Rectangle {
public:
    Square(double side) : Rectangle(side, side) {}
    std::string name() const override { return "Square"; }
};

// ── Multiple inheritance ────────────────────────────────────────────────────
class Printable {
public:
    virtual ~Printable() = default;
    virtual void pretty_print(std::ostream& os) const = 0;
};

class Serializable {
public:
    virtual ~Serializable() = default;
    virtual std::string serialize() const = 0;
};

class Drawable : public Shape, public Printable, public Serializable {
public:
    Drawable(double val) : val_(val) {}

    double area() const override { return val_ * val_; }
    std::string name() const override { return "Drawable"; }

    void pretty_print(std::ostream& os) const override {
        os << "[Drawable val=" << val_ << "]";
    }

    std::string serialize() const override {
        return "drawable:" + std::to_string(val_);
    }
private:
    double val_;
};

// ── Diamond inheritance with virtual base ───────────────────────────────────
class Base {
public:
    virtual ~Base() = default;
    int base_val = 42;
};

class Left : virtual public Base {
public:
    int left_val = 1;
};

class Right : virtual public Base {
public:
    int right_val = 2;
};

class Diamond : public Left, public Right {
public:
    int sum() const { return base_val + left_val + right_val; }
};

int main() {
    // Polymorphism via base pointer
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Rectangle>(4.0, 6.0));
    shapes.push_back(std::make_unique<Square>(3.0));

    std::cout << "=== Polymorphic shapes ===\n";
    for (const auto& s : shapes) {
        s->print();
    }

    // RTTI / dynamic_cast
    std::cout << "\n=== RTTI ===\n";
    Shape* raw = shapes[1].get();
    if (auto* rect = dynamic_cast<Rectangle*>(raw)) {
        std::cout << "  dynamic_cast to Rectangle succeeded\n";
        std::cout << "  typeid: " << typeid(*rect).name() << "\n";
    }

    // Multiple inheritance
    std::cout << "\n=== Multiple inheritance ===\n";
    Drawable d(7.0);
    d.print();
    d.pretty_print(std::cout);
    std::cout << "\n";
    std::cout << "  serialized: " << d.serialize() << "\n";

    // Diamond
    std::cout << "\n=== Diamond inheritance ===\n";
    Diamond diamond;
    std::cout << "  base_val=" << diamond.base_val
              << " left=" << diamond.left_val
              << " right=" << diamond.right_val
              << " sum=" << diamond.sum() << "\n";

    return 0;
}
