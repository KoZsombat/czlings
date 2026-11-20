// cpp06_dtor_raii -- destructors run at scope exit; RAII ties cleanup to it.
//
// Coming from TypeScript: there are no destructors and no deterministic
// destruction. JS has `finally`, but C++ runs destructors of locals on every
// exit path, including `return` and thrown exceptions.
//
// Task: DepthGuard increments the counter in its constructor. Complete its
// destructor so the counter is decremented and the log records a closing
// parenthesis; nested guards must log "(())".

#include <string>

#include "cztest.h"

std::string g_log;

class DepthGuard {
public:
    explicit DepthGuard(int& depth) : depth_(depth) {
        ++depth_;
        g_log += '(';
    }

    ~DepthGuard() {
        // TODO: decrement depth_ and append ')'
    }

private:
    int& depth_;
};

int main() {
    int depth = 0;
    {
        DepthGuard outer(depth);
        CZ_CHECK_INT(depth, 1);
        {
            DepthGuard inner(depth);
            CZ_CHECK_INT(depth, 2);
        }
        CZ_CHECK_INT(depth, 1);
    }
    CZ_CHECK_INT(depth, 0);
    CZ_CHECK_STR(g_log.c_str(), "(())");

    return cztest_summary();
}

// I AM NOT DONE
