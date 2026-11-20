// std::map keeps keys sorted (red-black tree), std::unordered_map hashes them,
// std::set is a sorted set of unique keys.
//
// Note: std::map iterates in key order no matter what you inserted first, which
// is what makes output deterministic. unordered_map has no defined iteration
// order at all.
#include "cztest.h"

#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

std::map<std::string, int> word_counts(const std::vector<std::string>& words)
{
    std::map<std::string, int> counts;
    for (const std::string& word : words) {
        ++counts[word];
    }
    return counts;
}

std::set<std::string> unique_words(const std::vector<std::string>& words)
{
    return std::set<std::string>(words.begin(), words.end());
}

int lookup(const std::unordered_map<std::string, int>& table, const std::string& key)
{
    const auto it = table.find(key);
    return it == table.end() ? -1 : it->second;
}

int main()
{
    const std::vector<std::string> words{"the", "cat", "the", "hat"};
    const std::map<std::string, int> counts = word_counts(words);
    CZ_CHECK_INT(counts.size(), 3);
    CZ_CHECK_INT(counts.at("the"), 2);
    CZ_CHECK_STR(counts.begin()->first.c_str(), "cat");

    const std::set<std::string> unique = unique_words(words);
    CZ_CHECK(unique.size() == 3 && unique.count("cat") == 1);

    const std::unordered_map<std::string, int> table{{"one", 1}, {"two", 2}};
    CZ_CHECK(lookup(table, "two") == 2 && lookup(table, "three") == -1);
    return cztest_summary();
}
