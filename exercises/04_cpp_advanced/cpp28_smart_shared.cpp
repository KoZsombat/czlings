// std::shared_ptr is counted ownership; std::weak_ptr observes without owning.
// A shared_ptr that points back up the graph creates a cycle that never drops
// to zero, so the back-edge must be weak.
//
// Note: weak ownership is the normal way to express "parent owns child, child
// refers to parent", and use_count() makes the reference graph observable in
// tests.
//
// Task: link() stores child in parent->next (shared) and parent in
// child->parent (weak). parent_name() goes through lock() so a dead parent
// reads as "(none)" instead of a dangling pointer.
//
// From TS: the GC collects cycles. `shared_ptr` counts references, so a cycle
// keeps itself alive; `weak_ptr` observes without owning and breaks the cycle.
#include "cztest.h"

#include <memory>
#include <string>
#include <utility>

struct Node {
    explicit Node(std::string n) : name(std::move(n)) {}
    std::string name;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> parent;
};

void link(const std::shared_ptr<Node>& parent, const std::shared_ptr<Node>& child)
{
    parent->next = child;
    // TODO: record the parent without owning it.
}

std::string parent_name(const std::shared_ptr<Node>& node)
{
    const std::shared_ptr<Node> owner = node->parent.lock();
    return owner ? owner->name : std::string("(none)");
}

int main()
{
    auto parent = std::make_shared<Node>("parent");
    auto child = std::make_shared<Node>("child");
    link(parent, child);

    CZ_CHECK_INT(parent.use_count(), 1);
    CZ_CHECK_INT(child.use_count(), 2);
    const std::string owner = parent_name(child);
    CZ_CHECK_STR(owner.c_str(), "parent");

    const std::weak_ptr<Node> observer = child;
    parent.reset();
    CZ_CHECK(child.use_count() == 1 && child->parent.expired());

    child.reset();
    CZ_CHECK(observer.expired());
    return cztest_summary();
}

// I AM NOT DONE
