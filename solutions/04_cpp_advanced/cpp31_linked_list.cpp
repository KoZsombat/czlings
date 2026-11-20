// A singly linked list where every node is owned by exactly one
// std::unique_ptr: no manual delete, and the whole chain is freed when the head
// pointer is destroyed.
//
// Coming from TypeScript: `{ value, next }` objects are collected by the GC,
// and a cycle is a leak. Here ownership is in the type: the list owns the head,
// each node owns the next, and reverse() is a loop of moves that never copies a
// node.
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
        node->next = std::move(head_);
        head_ = std::move(node);
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
        std::unique_ptr<Node> previous;
        while (head_) {
            std::unique_ptr<Node> rest = std::move(head_->next);
            head_->next = std::move(previous);
            previous = std::move(head_);
            head_ = std::move(rest);
        }
        head_ = std::move(previous);
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
