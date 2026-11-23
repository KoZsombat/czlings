// p03_cpp_library -- C++ core project. Reference solution.
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

    std::string kind() const override { return "book"; }

    std::string describe() const override
    {
        return title() + " by " + author_ + " (" + std::to_string(pages_) +
               " pages)";
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

    std::string kind() const override { return "dvd"; }

    std::string describe() const override
    {
        return title() + " (" + std::to_string(minutes_) + " min)";
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

    std::string kind() const override { return "audiobook"; }

    std::string describe() const override
    {
        return title() + " narrated by " + narrator_ + " (" +
               std::to_string(minutes_) + " min)";
    }

    bool borrow() override
    {
        if (borrowed_) {
            return false;
        }
        borrowed_ = true;
        return true;
    }

    bool give_back() override
    {
        if (!borrowed_) {
            return false;
        }
        borrowed_ = false;
        return true;
    }

    bool is_borrowed() const override { return borrowed_; }

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
        for (const auto& item : items_) {
            if (item->id() == id) {
                return item.get();
            }
        }
        return nullptr;
    }

    AudioBook* find_audiobook(int id)
    {
        Item* item = find(id);
        if (item == nullptr) {
            return nullptr;
        }
        return dynamic_cast<AudioBook*>(item);
    }

    std::string report() const
    {
        std::ostringstream out;
        for (const auto& item : items_) {
            out << item->id() << ' ' << item->kind() << ' ';
            if (const auto* audio =
                    dynamic_cast<const AudioBook*>(item.get())) {
                out << audio->title() << " narrated by " << audio->narrator()
                    << " (" << audio->minutes() << " min, "
                    << (audio->is_borrowed() ? "borrowed" : "available")
                    << ")\n";
            } else {
                out << item->describe() << '\n';
            }
        }
        return out.str();
    }

    std::string save() const
    {
        std::ostringstream out;
        for (const auto& item : items_) {
            if (const auto* book = dynamic_cast<const Book*>(item.get())) {
                out << "book|" << book->title() << '|' << book->author()
                    << '|' << book->pages() << '\n';
            } else if (const auto* dvd =
                           dynamic_cast<const Dvd*>(item.get())) {
                out << "dvd|" << dvd->title() << '|' << dvd->minutes()
                    << '\n';
            } else if (const auto* audio =
                           dynamic_cast<const AudioBook*>(item.get())) {
                out << "audio|" << audio->title() << '|'
                    << audio->narrator() << '|' << audio->minutes() << '|'
                    << (audio->is_borrowed() ? 1 : 0) << '\n';
            }
        }
        return out.str();
    }

    bool load(const std::string& text)
    {
        Library temporary;
        std::istringstream input(text);
        std::string line;

        while (std::getline(input, line)) {
            if (line.empty()) {
                continue;
            }
            std::istringstream fields(line);
            std::string type;
            if (!std::getline(fields, type, '|')) {
                return false;
            }

            if (type == "book") {
                std::string title;
                std::string author;
                std::string pages_text;
                if (!std::getline(fields, title, '|') ||
                    !std::getline(fields, author, '|') ||
                    !std::getline(fields, pages_text, '|')) {
                    return false;
                }
                int pages = 0;
                try {
                    pages = std::stoi(pages_text);
                } catch (...) {
                    return false;
                }
                if (pages <= 0) {
                    return false;
                }
                temporary.open_book(title, author, pages);
            } else if (type == "dvd") {
                std::string title;
                std::string minutes_text;
                if (!std::getline(fields, title, '|') ||
                    !std::getline(fields, minutes_text, '|')) {
                    return false;
                }
                int minutes = 0;
                try {
                    minutes = std::stoi(minutes_text);
                } catch (...) {
                    return false;
                }
                if (minutes <= 0) {
                    return false;
                }
                temporary.open_dvd(title, minutes);
            } else if (type == "audio") {
                std::string title;
                std::string narrator;
                std::string minutes_text;
                std::string borrowed_text;
                if (!std::getline(fields, title, '|') ||
                    !std::getline(fields, narrator, '|') ||
                    !std::getline(fields, minutes_text, '|') ||
                    !std::getline(fields, borrowed_text, '|')) {
                    return false;
                }
                int minutes = 0;
                int borrowed = 0;
                try {
                    minutes = std::stoi(minutes_text);
                    borrowed = std::stoi(borrowed_text);
                } catch (...) {
                    return false;
                }
                if (minutes <= 0 || (borrowed != 0 && borrowed != 1)) {
                    return false;
                }
                const int id =
                    temporary.open_audiobook(title, narrator, minutes);
                if (borrowed != 0) {
                    AudioBook* audio = temporary.find_audiobook(id);
                    if (audio == nullptr || !audio->borrow()) {
                        return false;
                    }
                }
            } else {
                return false;
            }
        }

        *this = std::move(temporary);
        return true;
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
