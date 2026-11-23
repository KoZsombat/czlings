// p04_word_index -- C++ advanced project 1. Reference solution.
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
        ++line_number_;
        std::istringstream stream(line);
        std::string token;
        while (stream >> token) {
            std::string word;
            for (char ch : token) {
                if (std::isalpha(static_cast<unsigned char>(ch)) != 0) {
                    word.push_back(
                        static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
                }
            }
            if (!word.empty()) {
                index_[word].insert(line_number_);
            }
        }
    }

    std::vector<int> lines_for(const std::string& word) const
    {
        const auto found = index_.find(word);
        if (found == index_.end()) {
            return {};
        }
        return std::vector<int>(found->second.begin(), found->second.end());
    }

    std::vector<std::pair<std::string, int>>
    top_words(std::size_t limit) const
    {
        std::vector<std::pair<std::string, int>> counts;
        counts.reserve(index_.size());
        for (const auto& entry : index_) {
            counts.emplace_back(entry.first,
                                static_cast<int>(entry.second.size()));
        }
        std::sort(counts.begin(), counts.end(),
                  [](const std::pair<std::string, int>& left,
                     const std::pair<std::string, int>& right) {
                      if (left.second != right.second) {
                          return left.second > right.second;
                      }
                      return left.first < right.first;
                  });
        if (counts.size() > limit) {
            counts.resize(limit);
        }
        return counts;
    }

    std::size_t size() const { return index_.size(); }

    std::string report() const
    {
        std::ostringstream out;
        for (const auto& entry : index_) {
            out << entry.first << ':';
            bool first = true;
            for (int line : entry.second) {
                out << (first ? " " : ", ") << line;
                first = false;
            }
            out << '\n';
        }
        return out.str();
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
