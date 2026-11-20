// cpp05_ctor_init -- member-initializer lists and delegation.
//
// Note: the initializer list is *not* the initialization order. Members are
// constructed in declaration order, and a list written in another order is a
// common bug (compilers warn with -Wreorder).

#include <string>
#include <utility>

#include "cztest.h"

std::string g_log;

struct Mark {
    explicit Mark(char name) : name(name) {
        g_log += name;
    }
    char name;
};

class Widget {
public:
    Widget() : first_('a'), second_('b') {}

private:
    Mark first_;
    Mark second_;
};

class Email {
public:
    explicit Email(std::string user) : Email(std::move(user), "example.com") {}

    Email(std::string user, std::string domain)
        : user_(std::move(user)), domain_(std::move(domain)) {}

    std::string str() const {
        return user_ + "@" + domain_;
    }

private:
    std::string user_;
    std::string domain_;
};

int main() {
    {
        Widget widget;
        (void)widget;
    }
    CZ_CHECK_STR(g_log.c_str(), "ab");

    const std::string default_email = Email("bob").str();
    CZ_CHECK_STR(default_email.c_str(), "bob@example.com");
    const std::string custom_email = Email("bob", "x.org").str();
    CZ_CHECK_STR(custom_email.c_str(), "bob@x.org");

    return cztest_summary();
}
