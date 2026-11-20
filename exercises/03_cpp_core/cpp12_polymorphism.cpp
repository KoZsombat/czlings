// cpp12_polymorphism -- virtual dispatch through a base pointer.
//
// Coming from TypeScript: methods dispatch dynamically by default. In C++ only
// functions declared `virtual` do; otherwise the static type wins. Deleting
// through a base pointer also requires a virtual destructor.
//
// Task: make area/name virtual in Shape so a Shape* reaches Circle/Square, and
// make ~Shape virtual so Tracked's destructor runs on delete.

#include <string>

#include "cztest.h"

class Shape {
public:
    ~Shape() {}
    double area() const { return 0.0; }
    std::string name() const { return "shape"; }
};

class Circle : public Shape {
public:
    explicit Circle(double radius) : radius_(radius) {}
    double area() const { return 3.14159 * radius_ * radius_; }
    std::string name() const { return "circle"; }

private:
    double radius_;
};

class Square : public Shape {
public:
    explicit Square(double side) : side_(side) {}
    double area() const { return side_ * side_; }
    std::string name() const { return "square"; }

private:
    double side_;
};

class Tracked : public Shape {
public:
    Tracked() { ++live_; }
    ~Tracked() { --live_; }
    static int live_;
};

int Tracked::live_ = 0;

int main() {
    Shape* circle = new Circle(2.0);
    CZ_CHECK_NEAR(circle->area(), 12.56636, 1e-9);
    const std::string circle_name = circle->name();
    CZ_CHECK_STR(circle_name.c_str(), "circle");
    delete circle;
    Shape* square = new Square(3.0);
    CZ_CHECK_NEAR(square->area(), 9.0, 1e-9);
    delete square;
    Shape* tracked = new Tracked();
    CZ_CHECK_INT(Tracked::live_, 1);
    delete tracked;
    CZ_CHECK_INT(Tracked::live_, 0);
    return cztest_summary();
}

// I AM NOT DONE
