// An abstract Shape base with pure virtual area()/name() and a virtual
// destructor, plus a container of std::unique_ptr<Shape> sorted by area.
//
// Note: the base must declare `virtual`, and a missing virtual destructor makes
// deleting through Shape* undefined behaviour.
//
// Task: make area() and name() pure virtual in Shape and override them in both
// subclasses, so every call through a Shape& reaches the real implementation.
//
// From TS: an abstract class or interface plus `implements` dispatches through
// the base type. C++ spells the same idea pure virtual in the base and
// `override` in the subclass.
#include "cztest.h"

#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class Shape {
public:
    virtual ~Shape() = default;
    // TODO: these must be pure virtual.
    double area() const { return 0.0; }
    std::string name() const { return "shape"; }
};

class Circle : public Shape {
public:
    explicit Circle(double radius) : radius_(radius) {}
    double area() const { return 3.141592653589793 * radius_ * radius_; }
    std::string name() const { return "circle"; }

private:
    double radius_;
};

class Rectangle : public Shape {
public:
    Rectangle(double width, double height) : width_(width), height_(height) {}
    double area() const { return width_ * height_; }
    std::string name() const { return "rectangle"; }

private:
    double width_;
    double height_;
};

std::vector<std::string> names_by_area(std::vector<std::unique_ptr<Shape>> shapes)
{
    std::sort(shapes.begin(), shapes.end(),
              [](const std::unique_ptr<Shape>& a, const std::unique_ptr<Shape>& b) {
                  return a->area() < b->area();
              });
    std::vector<std::string> names;
    for (const auto& shape : shapes) { names.push_back(shape->name()); }
    return names;
}

int main()
{
    CZ_CHECK_NEAR(Circle(5.0).area(), 78.539816, 1e-4);

    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Rectangle>(3.0, 4.0));

    const Shape& base = *shapes[1];
    CZ_CHECK_NEAR(base.area(), 12.0, 1e-9);
    const std::string label = base.name();
    CZ_CHECK_STR(label.c_str(), "rectangle");

    const std::vector<std::string> order = names_by_area(std::move(shapes));
    CZ_CHECK(order.size() == 2 && order[0] == "rectangle" && order[1] == "circle");
    return cztest_summary();
}

// I AM NOT DONE
