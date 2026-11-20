// std::unique_ptr is exclusive ownership: it deletes once, cannot be copied,
// and std::move hands the pointer over leaving the source null.
//
// Coming from TypeScript: the GC decides when an object dies, so ownership is
// invisible. Here the type states it: a moved-from unique_ptr is a null
// pointer, not an empty object, and two owners are a compile error.
//
// Task: make_animal builds a Dog for "dog" and a Cat for "cat"; speak_all joins
// every voice with one space; take_ownership returns the pointer it was given.
#include "cztest.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

class Animal {
public:
    virtual ~Animal() = default;
    virtual std::string speak() const = 0;
};

class Dog : public Animal {
public:
    std::string speak() const override { return "woof"; }
};

class Cat : public Animal {
public:
    std::string speak() const override { return "meow"; }
};

std::unique_ptr<Animal> make_animal(const std::string& kind)
{
    // TODO: the branch for "cat" is missing.
    if (kind == "dog") { return std::make_unique<Dog>(); }
    return std::make_unique<Dog>();
}

std::string speak_all(const std::vector<std::unique_ptr<Animal>>& animals)
{
    // TODO: append every animal's voice, separated by one space.
    if (animals.empty() || !animals[0]) { return std::string(); }
    return animals[0]->speak();
}

std::unique_ptr<int> take_ownership(std::unique_ptr<int> value)
{
    // TODO: return the pointer it was given, not a new one.
    (void)value;
    return std::make_unique<int>(0);
}

int main()
{
    const auto dog = make_animal("dog");
    CZ_CHECK(dog != nullptr && dog->speak() == "woof");
    const auto cat = make_animal("cat");
    CZ_CHECK(cat != nullptr && cat->speak() == "meow");

    std::vector<std::unique_ptr<Animal>> animals;
    animals.push_back(std::make_unique<Dog>());
    animals.push_back(std::make_unique<Cat>());
    const std::string words = speak_all(animals);
    CZ_CHECK_STR(words.c_str(), "woof meow");

    std::unique_ptr<int> number = std::make_unique<int>(42);
    std::unique_ptr<int> moved = take_ownership(std::move(number));
    CZ_CHECK(number == nullptr && moved != nullptr && *moved == 42);
    return cztest_summary();
}

// I AM NOT DONE
