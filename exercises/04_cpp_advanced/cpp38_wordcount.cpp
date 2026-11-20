// Capstone: extract words from a std::istringstream, count them in a std::map,
// then emit a report ordered by count (descending) with ties broken
// alphabetically. Only [a-z] letters reach the map.
//
// Note: stream extraction skips whitespace, std::map is key-sorted, and the
// comparator fixes the tie-break, so the report is deterministic.
//
// Task: count_words lowercases and drops non-letters ("The" and "the," become
// one word); ranked orders by count descending, then word ascending; report
// joins them as one "word count\n" line per entry.
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
        // TODO: keep letters only and lowercase them before counting.
        ++counts[token];
    }
    return counts;
}

std::vector<std::pair<std::string, int>> ranked(const std::map<std::string, int>& counts)
{
    std::vector<std::pair<std::string, int>> items(counts.begin(), counts.end());
    std::sort(items.begin(), items.end(),
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                  // TODO: highest count first, alphabetically on a tie.
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

// I AM NOT DONE
