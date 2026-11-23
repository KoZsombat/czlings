// p04_catalog -- C++ advanced project 2.
//
// A media catalog built around std::variant. Books and DVDs have different
// fields, but the catalog stores both in one vector and inspects them with
// std::visit, std::holds_alternative and std::get_if.
//
//   struct Book { std::string title; std::string author; int pages; };
//   struct Dvd  { std::string title; int minutes; };
//   using Media = std::variant<Book, Dvd>;
//
//   add(media)                 append (provided)
//   size()                     number of items (provided)
//   find(title)                first item with that title, or nullptr
//   index_of(title)            std::optional index, or std::nullopt
//   titles()                   all titles, alphabetically sorted
//   total_minutes()            sum of DVD minutes; books contribute 0
//   count_books()              number of Book alternatives
//   report()                   alphabetical one-line-per-item report:
//                                Book: "<title> by <author> (<pages> pages)"
//                                Dvd:  "<title> (<minutes> min)"
//
// From TS: a discriminated union is a tag plus a payload. std::variant stores
// the tag for you; std::visit runs the right branch, and std::get_if checks a
// branch without throwing.
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

class Catalog {
public:
    void add(Media media)
    {
        // TODO: append media to items_.
        (void)media;
    }

    std::size_t size() const
    {
        // TODO: return the number of items.
        return 0;
    }

    const Media* find(const std::string& title) const
    {
        // TODO: walk items_ and return the address of the first item whose
        // title matches, or nullptr. Use std::visit for the title.
        (void)title;
        return nullptr;
    }

    std::optional<std::size_t> index_of(const std::string& title) const
    {
        // TODO: return the 0-based index of the first matching title, or
        // std::nullopt.
        (void)title;
        return std::nullopt;
    }

    std::vector<std::string> titles() const
    {
        // TODO: collect every title and sort the result alphabetically.
        return {};
    }

    int total_minutes() const
    {
        // TODO: std::visit each item; return item.minutes for a Dvd and 0 for
        // a Book.
        return 0;
    }

    std::size_t count_books() const
    {
        // TODO: count items where std::holds_alternative<Book> is true.
        return 0;
    }

    std::string report() const
    {
        // TODO: sort pointers by title and format one line per item as shown
        // above. Book lines use pages, Dvd lines use minutes.
        return {};
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

// I AM NOT DONE
