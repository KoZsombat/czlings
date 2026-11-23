// p03_cpp_library -- C++ core project: multiple inheritance, RTTI, persistence.
//
// Prog2 lecture 06 is heterogeneous collections and pointer conversions;
// lecture 09 is multiple inheritance, casts and persistence. This project puts
// both together in a small library catalogue.
//
//   Item          abstract base: id, title, kind(), describe()
//   Borrowable    interface: borrow(), give_back(), is_borrowed()
//   Book          Item
//   Dvd           Item
//   AudioBook     Item AND Borrowable   <- multiple inheritance
//   Library       owns vector<unique_ptr<Item>>
//
// Implement:
//   Book/Dvd/AudioBook::kind() and describe()
//   AudioBook::borrow(), give_back(), is_borrowed()
//   Library::find(id), find_audiobook(id)
//   Library::report()
//   Library::save() / load(text)
//
// `find_audiobook` must use dynamic_cast: the vector stores Item pointers, so
// only RTTI can ask whether one is also an AudioBook. `save` writes one
// pipe-separated record per item:
//
//   book|The Hobbit|Tolkien|310
//   dvd|Alien|117
//   audio|Dune|Simon Vance|412|0
//
// The last audio field is 1 when borrowed, 0 otherwise. `load` parses the same
// format into a temporary Library and only replaces *this on success.
//
// From TS: `instanceof` checks a class at runtime. C++ uses dynamic_cast on a
// polymorphic type; a failed pointer cast yields nullptr. A class can also
// implement two independent interfaces by inheriting both.
#include "cztest.h"

#include <cstddef>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

class Item {
public:
    Item(int id, std::string title) : id_(id), title_(std::move(title)) {}
    virtual ~Item() = default;

    int id() const { return id_; }
    const std::string& title() const { return title_; }

    virtual std::string kind() const = 0;
    virtual std::string describe() const = 0;

private:
    int id_;
    std::string title_;
};

class Borrowable {
public:
    virtual ~Borrowable() = default;
    virtual bool borrow() = 0;
    virtual bool give_back() = 0;
    virtual bool is_borrowed() const = 0;
};

class Book : public Item {
public:
    Book(int id, std::string title, std::string author, int pages)
        : Item(id, std::move(title)), author_(std::move(author)), pages_(pages)
    {
    }

    std::string kind() const override
    {
        // TODO: return "book".
        return {};
    }

    std::string describe() const override
    {
        // TODO: "<title> by <author> (<pages> pages)".
        return {};
    }

    const std::string& author() const { return author_; }
    int pages() const { return pages_; }

private:
    std::string author_;
    int pages_;
};

class Dvd : public Item {
public:
    Dvd(int id, std::string title, int minutes)
        : Item(id, std::move(title)), minutes_(minutes)
    {
    }

    std::string kind() const override
    {
        // TODO: return "dvd".
        return {};
    }

    std::string describe() const override
    {
        // TODO: "<title> (<minutes> min)".
        return {};
    }

    int minutes() const { return minutes_; }

private:
    int minutes_;
};

class AudioBook : public Item, public Borrowable {
public:
    AudioBook(int id, std::string title, std::string narrator, int minutes)
        : Item(id, std::move(title)),
          narrator_(std::move(narrator)),
          minutes_(minutes)
    {
    }

    std::string kind() const override
    {
        // TODO: return "audiobook".
        return {};
    }

    std::string describe() const override
    {
        // TODO: "<title> narrated by <narrator> (<minutes> min)".
        return {};
    }

    bool borrow() override
    {
        // TODO: set borrowed_ and return true; return false if already out.
        (void)borrowed_;
        return false;
    }

    bool give_back() override
    {
        // TODO: clear borrowed_ and return true; return false if not borrowed.
        return false;
    }

    bool is_borrowed() const override
    {
        // TODO: return borrowed_.
        return false;
    }

    const std::string& narrator() const { return narrator_; }
    int minutes() const { return minutes_; }

private:
    std::string narrator_;
    int minutes_;
    bool borrowed_ = false;
};

class Library {
public:
    int open_book(const std::string& title, const std::string& author,
                  int pages)
    {
        const int id = next_id_++;
        items_.push_back(
            std::make_unique<Book>(id, title, author, pages));
        return id;
    }

    int open_dvd(const std::string& title, int minutes)
    {
        const int id = next_id_++;
        items_.push_back(std::make_unique<Dvd>(id, title, minutes));
        return id;
    }

    int open_audiobook(const std::string& title, const std::string& narrator,
                       int minutes)
    {
        const int id = next_id_++;
        items_.push_back(
            std::make_unique<AudioBook>(id, title, narrator, minutes));
        return id;
    }

    std::size_t size() const { return items_.size(); }

    Item* find(int id)
    {
        // TODO: return the item with that id, or nullptr.
        (void)id;
        return nullptr;
    }

    AudioBook* find_audiobook(int id)
    {
        // TODO: find the id and dynamic_cast the Item* to AudioBook*.
        (void)id;
        return nullptr;
    }

    std::string report() const
    {
        // TODO: one line per item: "<id> <kind> <describe>". For an AudioBook
        // put ", available" or ", borrowed" before the closing ')'.
        return {};
    }

    std::string save() const
    {
        // TODO: write the pipe-separated records described above.
        return {};
    }

    bool load(const std::string& text)
    {
        // TODO: parse into a temporary Library; replace *this only if every
        // line is valid.
        (void)text;
        return false;
    }

private:
    std::vector<std::unique_ptr<Item>> items_;
    int next_id_ = 1;
};

int main()
{
    Library library;
    const int book_id = library.open_book("The Hobbit", "Tolkien", 310);
    const int dvd_id = library.open_dvd("Alien", 117);
    const int audio_id =
        library.open_audiobook("Dune", "Simon Vance", 412);

    CZ_CHECK_INT((long)library.size(), 3);

    Item* book = library.find(book_id);
    CZ_CHECK(book != nullptr);
    if (book != nullptr) {
        const std::string kind = book->kind();
        const std::string description = book->describe();
        CZ_CHECK_STR(kind.c_str(), "book");
        CZ_CHECK_STR(description.c_str(), "The Hobbit by Tolkien (310 pages)");
    }

    Item* dvd = library.find(dvd_id);
    CZ_CHECK(dvd != nullptr);
    if (dvd != nullptr) {
        const std::string kind = dvd->kind();
        const std::string description = dvd->describe();
        CZ_CHECK_STR(kind.c_str(), "dvd");
        CZ_CHECK_STR(description.c_str(), "Alien (117 min)");
    }

    CZ_CHECK(library.find(999) == nullptr);

    AudioBook* audio = library.find_audiobook(audio_id);
    CZ_CHECK(audio != nullptr);
    CZ_CHECK(library.find_audiobook(book_id) == nullptr);
    if (audio != nullptr) {
        const std::string kind = audio->kind();
        const std::string description = audio->describe();
        CZ_CHECK_STR(kind.c_str(), "audiobook");
        CZ_CHECK_STR(description.c_str(),
                     "Dune narrated by Simon Vance (412 min)");
        CZ_CHECK(!audio->is_borrowed());
        CZ_CHECK(audio->borrow());
        CZ_CHECK(audio->is_borrowed());
        CZ_CHECK(!audio->borrow());
        CZ_CHECK(audio->give_back());
        CZ_CHECK(!audio->is_borrowed());
        CZ_CHECK(!audio->give_back());
    }

    Borrowable* loan = dynamic_cast<Borrowable*>(library.find(audio_id));
    CZ_CHECK(loan != nullptr);
    if (loan != nullptr) {
        CZ_CHECK(loan->borrow());
        CZ_CHECK(loan->is_borrowed());
        CZ_CHECK(loan->give_back());
    }
    CZ_CHECK(dynamic_cast<Borrowable*>(library.find(book_id)) == nullptr);

    const std::string saved = library.save();
    CZ_CHECK_STR(saved.c_str(),
                 "book|The Hobbit|Tolkien|310\n"
                 "dvd|Alien|117\n"
                 "audio|Dune|Simon Vance|412|0\n");

    Library loaded;
    CZ_CHECK(loaded.load(saved));
    CZ_CHECK_INT((long)loaded.size(), 3);
    CZ_CHECK(loaded.find_audiobook(3) != nullptr);
    const std::string report_text = loaded.report();
    CZ_CHECK_STR(report_text.c_str(),
                 "1 book The Hobbit by Tolkien (310 pages)\n"
                 "2 dvd Alien (117 min)\n"
                 "3 audiobook Dune narrated by Simon Vance (412 min, available)\n");

    Library bad;
    CZ_CHECK(!bad.load("book|Only a title|An author\n"));
    CZ_CHECK_INT((long)bad.size(), 0);
    CZ_CHECK(!library.load("not a record"));
    CZ_CHECK_INT((long)library.size(), 3);

    return cztest_summary();
}

// I AM NOT DONE
