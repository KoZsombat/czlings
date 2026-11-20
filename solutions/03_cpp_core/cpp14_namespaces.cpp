// cpp14_namespaces -- namespaces, qualification, using-declarations.
//
// Coming from TypeScript: `namespace` is an older TS feature and modules are
// the norm. A C++ `using namespace X;` is not an import: it drops every name
// from X into the current scope, which invites ambiguity. Prefer qualified
// names or a using-declaration for the one name you need.

#include <string>

#include "cztest.h"

namespace app {
namespace math {
int twice(int value) {
    return 2 * value;
}
}  // namespace math

namespace text {
std::string twice(const std::string& text) {
    return text + text;
}
}  // namespace text
}  // namespace app

int main() {
    CZ_CHECK_INT(app::math::twice(3), 6);
    const std::string doubled = app::text::twice("ab");
    CZ_CHECK_STR(doubled.c_str(), "abab");

    namespace local = app::math;
    CZ_CHECK_INT(local::twice(4), 8);

    using app::math::twice;
    using app::text::twice;
    CZ_CHECK_INT(twice(5), 10);
    const std::string single = twice(std::string("x"));
    CZ_CHECK_STR(single.c_str(), "xx");

    return cztest_summary();
}
