// cpp08_static -- static data members, static functions, class constants.
//
// Coming from TypeScript: `static` fields live on the class too, but in C++
// you must *define* a non-const static data member outside the class body.
// Static member functions have no `this` and cannot touch instance members.
//
// Task: alive_ counts live Widget objects. Update it in the constructor and
// destructor so alive() reports the truth.

#include "cztest.h"

class Widget {
public:
    Widget() {
        // TODO: count this object
    }

    explicit Widget(int id) : id_(id) {
        // TODO: count this object
    }

    ~Widget() {
        // TODO: stop counting this object
    }

    static int alive() {
        return alive_;
    }

    int id() const {
        return id_;
    }

    static constexpr int kMaxAlive = 8;

private:
    int id_ = 0;
    static int alive_;
};

int Widget::alive_ = 0;

int main() {
    CZ_CHECK_INT(Widget::alive(), 0);
    {
        Widget first;
        Widget second(2);
        CZ_CHECK_INT(Widget::alive(), 2);
        CZ_CHECK_INT(second.id(), 2);
    }
    CZ_CHECK_INT(Widget::alive(), 0);
    CZ_CHECK(Widget::kMaxAlive == 8);

    return cztest_summary();
}

// I AM NOT DONE
