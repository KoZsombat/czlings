// std::unique_ptr is exclusive ownership: it deletes exactly once, cannot be
// copied, and std::move hands the pointer over leaving the source null.
//
// Coming from TypeScript: the GC decides when an object dies, so "ownership"
// never appears in the type. Here the type states it: unique_ptr means one
// owner, and a moved-from unique_ptr is a null pointer, not an empty object.
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
    if (kind == "dog") {
        return std::make_unique<Dog>();
    }
    if (kind == "cat") {
        return std::make_unique<Cat>();
    }
    return nullptr;
}

std::string speak_all(const std::vector<std::unique_ptr<Animal>>& animals)
{
    std::string out;
    for (const auto& animal : animals) {
        if (!animal) {
            continue;
        }
        if (!out.empty()) {
            out += ' ';
        }
        out += animal->speak();
    }
    return out;
}

std::unique_ptr<int> take_ownership(std::unique_ptr<int> value)
{
    return value;
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
