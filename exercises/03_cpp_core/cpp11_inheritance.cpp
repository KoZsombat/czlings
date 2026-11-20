// cpp11_inheritance -- base and derived classes, protected, name hiding.
//
// Note: C++ requires the base constructor call in the derived initializer
// list; there is no implicit super(). A derived function with the same name
// *hides* the base one; call it explicitly with Base::name().
//
// Task: chain the constructor so a Dog keeps its name, hide speak() with
// "Woof", provide base_speak() calling Animal::speak(), and let tag() use the
// protected member.

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
    explicit Dog(const std::string&) : Animal("animal") {}

    std::string speak() const {
        return "...";  // TODO: "Woof"
    }

    std::string base_speak() const {
        return speak();  // TODO: call Animal::speak()
    }

    std::string tag() const {
        return "dog";  // TODO: name_ + " the dog"
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

// I AM NOT DONE
