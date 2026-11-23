// p04_word_index -- C++ advanced project 1.
//
// An inverted index built with the STL: std::map, std::set, std::vector,
// algorithms, lambdas and string streams. It revisits the word-count capstone
// but adds line numbers and ranked queries.
//
//   add_line(line)          index every [A-Za-z] word, lowercased, under the
//                           1-based line number; punctuation separates words
//   lines_for(word)         sorted line numbers for word (empty when absent)
//   top_words(limit)        (word, line count) pairs, line count descending,
//                           word ascending on a tie
//   size()                  number of distinct words (provided)
//   report()                one "word: 1, 2, 3" line per word, map order
//
// Example:
//   add_line("The cat and the hat.");
//   add_line("The CAT sat.");
//   lines_for("cat") -> {1, 2}
//   report() starts "and: 1\ncat: 1, 2\nhat: 1\n..."
//
// From TS: a Map<string, Set<number>> is close. Here std::map keeps keys
// sorted, std::set keeps line numbers sorted, and the comparator decides the
// ranking tie-break.
#include "cztest.h"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

class WordIndex {
public:
    void add_line(const std::string& line)
    {
        // TODO: increment line_number_, split on whitespace with
        // std::istringstream, keep only isalpha bytes, lowercase them, and
        // insert line_number_ into index_[word].
        (void)line;
        (void)line_number_;
    }

    std::vector<int> lines_for(const std::string& word) const
    {
        // TODO: find word in index_ and copy its set into a vector. Return an
        // empty vector when the word is absent.
        (void)word;
        return {};
    }

    std::vector<std::pair<std::string, int>>
    top_words(std::size_t limit) const
    {
        // TODO: build (word, number of lines) pairs from index_, sort by count
        // descending then word ascending, and truncate to limit.
        (void)limit;
        return {};
    }

    std::size_t size() const
    {
        // TODO: return the number of distinct words.
        return 0;
    }

    std::string report() const
    {
        // TODO: for every map entry, write "word:" followed by the line
        // numbers separated by ", " and a newline. Example:
        //   "and: 1\ncat: 1, 2\nhat: 1\n"
        return {};
    }

private:
    std::map<std::string, std::set<int>> index_;
    int line_number_ = 0;
};

int main()
{
    WordIndex index;
    index.add_line("The cat and the hat.");
    index.add_line("The CAT sat.");
    index.add_line("A dog and a cat.");

    CZ_CHECK_INT((long)index.size(), 7);

    const std::vector<int> cat_lines = index.lines_for("cat");
    CZ_CHECK_INT((long)cat_lines.size(), 3);
    if (cat_lines.size() == 3) {
        CZ_CHECK_INT(cat_lines[0], 1);
        CZ_CHECK_INT(cat_lines[1], 2);
        CZ_CHECK_INT(cat_lines[2], 3);
    }

    const std::vector<int> the_lines = index.lines_for("the");
    CZ_CHECK_INT((long)the_lines.size(), 2);
    if (the_lines.size() == 2) {
        CZ_CHECK_INT(the_lines[0], 1);
        CZ_CHECK_INT(the_lines[1], 2);
    }

    CZ_CHECK(index.lines_for("dog").size() == 1);
    CZ_CHECK(index.lines_for("missing").empty());

    const std::vector<std::pair<std::string, int>> top = index.top_words(3);
    CZ_CHECK_INT((long)top.size(), 3);
    if (top.size() == 3) {
        CZ_CHECK_STR(top[0].first.c_str(), "cat");
        CZ_CHECK_INT(top[0].second, 3);
        CZ_CHECK_STR(top[1].first.c_str(), "and");
        CZ_CHECK_INT(top[1].second, 2);
        CZ_CHECK_STR(top[2].first.c_str(), "the");
        CZ_CHECK_INT(top[2].second, 2);
    }
    CZ_CHECK(index.top_words(0).empty());

    const std::string report_text = index.report();
    CZ_CHECK_STR(report_text.c_str(),
                 "a: 3\n"
                 "and: 1, 3\n"
                 "cat: 1, 2, 3\n"
                 "dog: 3\n"
                 "hat: 1\n"
                 "sat: 2\n"
                 "the: 1, 2\n");

    return cztest_summary();
}

// I AM NOT DONE
