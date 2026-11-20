// An abstract base class (pure virtual area/name) with a virtual destructor,
// and a container of std::unique_ptr<Shape> that sorts by area. Calls through
// Shape& or Shape* dispatch to the dynamic type because the functions are
// virtual; deleting through Shape* is safe only because ~Shape is virtual.
//
// Note: the base must declare `virtual` for every function that should dispatch
// dynamically, and a missing virtual destructor makes `delete base_ptr`
// undefined behaviour.
#include "cztest.h"

#include <algorithm>
#include <cmath>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual std::string name() const = 0;
};

class Circle : public Shape {
public:
    explicit Circle(double radius) : radius_(radius) {}
    double area() const override { return 3.141592653589793 * radius_ * radius_; }
    std::string name() const override { return "circle"; }

private:
    double radius_;
};

class Rectangle : public Shape {
public:
    Rectangle(double width, double height) : width_(width), height_(height) {}
    double area() const override { return width_ * height_; }
    std::string name() const override { return "rectangle"; }

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
    for (const auto& shape : shapes) {
        names.push_back(shape->name());
    }
    return names;
}

int main()
{
    const Circle circle(5.0);
    CZ_CHECK_NEAR(circle.area(), 78.539816, 1e-4);

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
