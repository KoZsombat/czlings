// Capstone: read words from a std::istringstream, count them in a std::map,
// then emit a report ordered by count (descending) with ties broken
// alphabetically by a custom comparator. Only [a-z] letters reach the map.
//
// Coming from TypeScript: splitting, lowercasing and Map iteration are a few
// method calls with no fixed order guarantee. Here the stream extraction skips
// whitespace, std::map is already sorted by key, and the tie-break order is
// whatever your comparator says — so the report is byte-for-byte deterministic.
#include "cztest.h"

#include <algorithm>
#include <cctype>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

std::map<std::string, int> count_words(const std::string& text)
{
    std::istringstream stream(text);
    std::map<std::string, int> counts;
    std::string token;
    while (stream >> token) {
        std::string word;
        for (char ch : token) {
            const unsigned char byte = static_cast<unsigned char>(ch);
            if (std::isalpha(byte)) {
                word += static_cast<char>(std::tolower(byte));
            }
        }
        if (!word.empty()) {
            ++counts[word];
        }
    }
    return counts;
}

std::vector<std::pair<std::string, int>> ranked(const std::map<std::string, int>& counts)
{
    std::vector<std::pair<std::string, int>> items(counts.begin(), counts.end());
    std::sort(items.begin(), items.end(),
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                  if (a.second != b.second) {
                      return a.second > b.second;
                  }
                  return a.first < b.first;
              });
    return items;
}

std::string report(const std::string& text)
{
    const std::vector<std::pair<std::string, int>> items = ranked(count_words(text));
    std::string out;
    for (const auto& [word, count] : items) {
        out += word;
        out += ' ';
        out += std::to_string(count);
        out += '\n';
    }
    return out;
}

int main()
{
    const std::string text = "The cat and the hat, the CAT!";

    const std::map<std::string, int> counts = count_words(text);
    CZ_CHECK_INT(counts.at("the"), 3);
    CZ_CHECK_INT(counts.size(), 4);

    const std::vector<std::pair<std::string, int>> items = ranked(counts);
    CZ_CHECK(items[1].first == "cat" && items[1].second == 2);
    CZ_CHECK(items[2].first == "and" && items[3].first == "hat");

    const std::string summary = report(text);
    CZ_CHECK_STR(summary.c_str(), "the 3\ncat 2\nand 1\nhat 1\n");
    return cztest_summary();
}
