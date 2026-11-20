// A singly linked list where each node is owned by exactly one
// std::unique_ptr: no manual delete, and the chain dies with the head pointer.
//
// Coming from TypeScript: `{ value, next }` objects live until the GC decides,
// so ownership never appears in the code. Here the list owns the head and each
// node owns the next, so reverse() is a loop of moves, not copies.
//
// Task: push_front inserts at the front in O(1); to_vector reads top to bottom;
// reverse() relinks the nodes so the order flips and no node leaks.
#include "cztest.h"

#include <memory>
#include <utility>
#include <vector>

struct Node {
    explicit Node(int v) : value(v) {}
    int value;
    std::unique_ptr<Node> next;
};

class List {
public:
    void push_front(int value)
    {
        auto node = std::make_unique<Node>(value);
        // TODO: splice the new node in at the head, not at the tail.
        if (!head_) { head_ = std::move(node); return; }
        Node* last = head_.get();
        while (last->next) { last = last->next.get(); }
        last->next = std::move(node);
    }

    int sum() const
    {
        int total = 0;
        for (const Node* node = head_.get(); node != nullptr; node = node->next.get()) {
            total += node->value;
        }
        return total;
    }

    std::vector<int> to_vector() const
    {
        std::vector<int> out;
        for (const Node* node = head_.get(); node != nullptr; node = node->next.get()) {
            out.push_back(node->value);
        }
        return out;
    }

    void reverse()
    {
        // TODO: walk the chain moving each node behind the previous one.
    }

private:
    std::unique_ptr<Node> head_;
};

int main()
{
    List list;
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);
    CZ_CHECK_INT(list.sum(), 6);

    const std::vector<int> order = list.to_vector();
    CZ_CHECK(order.size() == 3 && order[0] == 3 && order[2] == 1);

    list.reverse();
    const std::vector<int> after = list.to_vector();
    CZ_CHECK(after.size() == 3 && after[0] == 1 && after[1] == 2 && after[2] == 3);
    CZ_CHECK_INT(list.sum(), 6);
    return cztest_summary();
}

// I AM NOT DONE
