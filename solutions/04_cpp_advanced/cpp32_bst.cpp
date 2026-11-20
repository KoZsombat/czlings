// A binary search tree of std::unique_ptr<Node>: insert, contains, in-order
// walk, height. The destructor is the compiler's: each node frees its subtree.
//
// Coming from TypeScript: you would use a class with nullable child fields and
// trust the GC. Here `std::unique_ptr<Node>` is the nullable link and the
// ownership tree mirrors the data tree, so no delete is written anywhere.
#include "cztest.h"

#include <algorithm>
#include <memory>
#include <vector>

struct Node {
    explicit Node(int k) : key(k) {}
    int key;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
};

class Bst {
public:
    void insert(int key) { insert_at(root_, key); }

    bool contains(int key) const
    {
        const Node* node = root_.get();
        while (node != nullptr) {
            if (key == node->key) {
                return true;
            }
            node = key < node->key ? node->left.get() : node->right.get();
        }
        return false;
    }

    std::vector<int> in_order() const
    {
        std::vector<int> out;
        walk(root_, out);
        return out;
    }

    int height() const { return height_of(root_.get()); }

private:
    static void insert_at(std::unique_ptr<Node>& slot, int key)
    {
        if (!slot) {
            slot = std::make_unique<Node>(key);
            return;
        }
        if (key < slot->key) {
            insert_at(slot->left, key);
        } else if (key > slot->key) {
            insert_at(slot->right, key);
        }
    }

    static void walk(const std::unique_ptr<Node>& node, std::vector<int>& out)
    {
        if (!node) {
            return;
        }
        walk(node->left, out);
        out.push_back(node->key);
        walk(node->right, out);
    }

    static int height_of(const Node* node)
    {
        if (node == nullptr) {
            return 0;
        }
        return 1 + std::max(height_of(node->left.get()), height_of(node->right.get()));
    }

    std::unique_ptr<Node> root_;
};

int main()
{
    Bst tree;
    for (int key : {5, 3, 8, 1, 4, 7, 9}) {
        tree.insert(key);
    }

    const std::vector<int> order = tree.in_order();
    const std::vector<int> expected{1, 3, 4, 5, 7, 8, 9};
    CZ_CHECK(order == expected);
    CZ_CHECK(tree.contains(4) && !tree.contains(6));
    CZ_CHECK_INT(tree.height(), 3);

    tree.insert(5);
    CZ_CHECK_INT(tree.in_order().size(), 7);
    return cztest_summary();
}
