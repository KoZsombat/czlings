// cpp11_inheritance -- base and derived classes, protected, name hiding.
//
// Coming from TypeScript: `extends` and `super(...)` exist, but C++ requires
// the base constructor call in the derived initializer list; there is no
// implicit super(). A derived function with the same name *hides* the base
// one; call it explicitly with Base::name().

#include <string>

#include "cztest.h"

class Animal {
public:
    explicit Animal(const std::string& name) : name_(name) {}

    std::string name() const {
        return name_;
    }

    std::string speak() const {
        return "...";
    }

protected:
    std::string name_;
};

class Dog : public Animal {
public:
    explicit Dog(const std::string& name) : Animal(name) {}

    std::string speak() const {
        return "Woof";
    }

    std::string base_speak() const {
        return Animal::speak();
    }

    std::string tag() const {
        return name_ + " the dog";
    }
};

int main() {
    Dog dog("rex");
    const std::string name = dog.name();
    CZ_CHECK_STR(name.c_str(), "rex");
    const std::string said = dog.speak();
    CZ_CHECK_STR(said.c_str(), "Woof");
    const std::string inherited = dog.base_speak();
    CZ_CHECK_STR(inherited.c_str(), "...");
    const std::string tagged = dog.tag();
    CZ_CHECK_STR(tagged.c_str(), "rex the dog");

    return cztest_summary();
}
