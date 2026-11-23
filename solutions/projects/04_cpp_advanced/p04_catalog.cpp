// p04_catalog -- C++ advanced project 2. Reference solution.
#include "cztest.h"

#include <algorithm>
#include <cstddef>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

struct Book {
    std::string title;
    std::string author;
    int pages;
};

struct Dvd {
    std::string title;
    int minutes;
};

using Media = std::variant<Book, Dvd>;

static std::string title_of(const Media& media)
{
    return std::visit([](const auto& item) { return item.title; }, media);
}

class Catalog {
public:
    void add(Media media) { items_.push_back(std::move(media)); }

    std::size_t size() const { return items_.size(); }

    const Media* find(const std::string& title) const
    {
        for (const Media& media : items_) {
            if (title_of(media) == title) {
                return &media;
            }
        }
        return nullptr;
    }

    std::optional<std::size_t> index_of(const std::string& title) const
    {
        for (std::size_t i = 0; i < items_.size(); ++i) {
            if (title_of(items_[i]) == title) {
                return i;
            }
        }
        return std::nullopt;
    }

    std::vector<std::string> titles() const
    {
        std::vector<std::string> result;
        result.reserve(items_.size());
        for (const Media& media : items_) {
            result.push_back(title_of(media));
        }
        std::sort(result.begin(), result.end());
        return result;
    }

    int total_minutes() const
    {
        int total = 0;
        for (const Media& media : items_) {
            total += std::visit(
                [](const auto& item) -> int {
                    using Type = std::decay_t<decltype(item)>;
                    if constexpr (std::is_same_v<Type, Dvd>) {
                        return item.minutes;
                    } else {
                        (void)item;
                        return 0;
                    }
                },
                media);
        }
        return total;
    }

    std::size_t count_books() const
    {
        return static_cast<std::size_t>(
            std::count_if(items_.begin(), items_.end(), [](const Media& media) {
                return std::holds_alternative<Book>(media);
            }));
    }

    std::string report() const
    {
        std::vector<const Media*> ordered;
        ordered.reserve(items_.size());
        for (const Media& media : items_) {
            ordered.push_back(&media);
        }
        std::sort(ordered.begin(), ordered.end(),
                  [](const Media* left, const Media* right) {
                      return title_of(*left) < title_of(*right);
                  });

        std::string out;
        for (const Media* media : ordered) {
            std::visit(
                [&out](const auto& item) {
                    using Type = std::decay_t<decltype(item)>;
                    if constexpr (std::is_same_v<Type, Book>) {
                        out += item.title + " by " + item.author + " (" +
                               std::to_string(item.pages) + " pages)\n";
                    } else {
                        out += item.title + " (" +
                               std::to_string(item.minutes) + " min)\n";
                    }
                },
                *media);
        }
        return out;
    }

private:
    std::vector<Media> items_;
};

int main()
{
    Catalog catalog;
    catalog.add(Book{"The Hobbit", "Tolkien", 310});
    catalog.add(Dvd{"Alien", 117});
    catalog.add(Book{"Dune", "Herbert", 412});
    catalog.add(Dvd{"Arrival", 116});

    CZ_CHECK_INT((long)catalog.size(), 4);
    CZ_CHECK_INT((long)catalog.count_books(), 2);
    CZ_CHECK_INT(catalog.total_minutes(), 233);

    const std::optional<std::size_t> dune = catalog.index_of("Dune");
    CZ_CHECK(dune.has_value() && *dune == 2);
    CZ_CHECK(!catalog.index_of("Missing").has_value());

    const Media* alien = catalog.find("Alien");
    CZ_CHECK(alien != nullptr);
    if (alien != nullptr) {
        const Dvd* dvd = std::get_if<Dvd>(alien);
        CZ_CHECK(dvd != nullptr);
        if (dvd != nullptr) {
            CZ_CHECK_INT(dvd->minutes, 117);
        }
        CZ_CHECK(std::get_if<Book>(alien) == nullptr);
    }

    const Media* hobbit = catalog.find("The Hobbit");
    CZ_CHECK(hobbit != nullptr);
    if (hobbit != nullptr) {
        const Book* book = std::get_if<Book>(hobbit);
        CZ_CHECK(book != nullptr);
        if (book != nullptr) {
            CZ_CHECK_STR(book->author.c_str(), "Tolkien");
            CZ_CHECK_INT(book->pages, 310);
        }
    }
    CZ_CHECK(catalog.find("Missing") == nullptr);

    const std::vector<std::string> titles = catalog.titles();
    CZ_CHECK_INT((long)titles.size(), 4);
    if (titles.size() == 4) {
        CZ_CHECK_STR(titles[0].c_str(), "Alien");
        CZ_CHECK_STR(titles[1].c_str(), "Arrival");
        CZ_CHECK_STR(titles[2].c_str(), "Dune");
        CZ_CHECK_STR(titles[3].c_str(), "The Hobbit");
    }

    const std::string report_text = catalog.report();
    CZ_CHECK_STR(report_text.c_str(),
                 "Alien (117 min)\n"
                 "Arrival (116 min)\n"
                 "Dune by Herbert (412 pages)\n"
                 "The Hobbit by Tolkien (310 pages)\n");

    return cztest_summary();
}
