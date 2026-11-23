// p04_cpp_pipeline -- C++ advanced project. Reference solution.
#include "cztest.h"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

struct Event {
    int sequence;
    std::string name;
    int value;
};

class EventLog {
public:
    void push(std::string name, int value)
    {
        events_.push_back(Event{next_sequence_++, std::move(name), value});
    }

    std::size_t size() const { return events_.size(); }

    class FilteredRange {
    public:
        class iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = Event;
            using difference_type = std::ptrdiff_t;
            using pointer = const Event*;
            using reference = const Event&;

            iterator() = default;

            iterator(const std::vector<Event>* events, std::size_t index,
                     const std::function<bool(const Event&)>* predicate)
                : events_(events), index_(index), predicate_(predicate)
            {
                skip();
            }

            reference operator*() const { return (*events_)[index_]; }
            pointer operator->() const { return &(*events_)[index_]; }

            iterator& operator++()
            {
                ++index_;
                skip();
                return *this;
            }

            iterator operator++(int)
            {
                iterator copy = *this;
                ++*this;
                return copy;
            }

            friend bool operator==(const iterator& left,
                                   const iterator& right)
            {
                return left.events_ == right.events_ &&
                       left.index_ == right.index_;
            }

            friend bool operator!=(const iterator& left,
                                   const iterator& right)
            {
                return !(left == right);
            }

        private:
            void skip()
            {
                while (events_ != nullptr && index_ < events_->size() &&
                       predicate_ != nullptr &&
                       !(*predicate_)((*events_)[index_])) {
                    ++index_;
                }
            }

            const std::vector<Event>* events_ = nullptr;
            std::size_t index_ = 0;
            const std::function<bool(const Event&)>* predicate_ = nullptr;
        };

        iterator begin() const
        {
            return iterator(&events_, 0, &predicate_);
        }

        iterator end() const
        {
            return iterator(&events_, events_.size(), &predicate_);
        }

    private:
        friend class EventLog;

        FilteredRange(const std::vector<Event>& events,
                      std::function<bool(const Event&)> predicate)
            : events_(events), predicate_(std::move(predicate))
        {
        }

        const std::vector<Event>& events_;
        std::function<bool(const Event&)> predicate_;
    };

    FilteredRange filtered(std::function<bool(const Event&)> predicate) const
    {
        return FilteredRange(events_, std::move(predicate));
    }

private:
    std::vector<Event> events_;
    int next_sequence_ = 1;
};

class Observer {
public:
    virtual ~Observer() = default;
    virtual void on_event(const Event& event) = 0;
};

class EventBus {
public:
    void subscribe(Observer* observer)
    {
        if (observer == nullptr) {
            return;
        }
        if (std::find(observers_.begin(), observers_.end(), observer) ==
            observers_.end()) {
            observers_.push_back(observer);
        }
    }

    void unsubscribe(Observer* observer)
    {
        observers_.erase(
            std::remove(observers_.begin(), observers_.end(), observer),
            observers_.end());
    }

    void publish(const Event& event)
    {
        for (Observer* observer : observers_) {
            if (observer != nullptr) {
                observer->on_event(event);
            }
        }
    }

private:
    std::vector<Observer*> observers_;
};

class ThresholdObserver : public Observer {
public:
    explicit ThresholdObserver(int threshold) : threshold_(threshold) {}

    void on_event(const Event& event) override
    {
        ++total_;
        if (event.value >= threshold_) {
            ++matches_;
        }
    }

    int matches() const { return matches_; }
    int total() const { return total_; }

private:
    int threshold_;
    int matches_ = 0;
    int total_ = 0;
};

int main()
{
    EventLog log;
    log.push("boot", 5);
    log.push("warn", 15);
    log.push("error", 25);
    log.push("fatal", 35);

    CZ_CHECK_INT((long)log.size(), 4);

    std::vector<std::string> names;
    for (const Event& event :
         log.filtered([](const Event& item) { return item.value >= 20; })) {
        names.push_back(event.name);
    }
    CZ_CHECK_INT((long)names.size(), 2);
    if (names.size() == 2) {
        CZ_CHECK_STR(names[0].c_str(), "error");
        CZ_CHECK_STR(names[1].c_str(), "fatal");
    }

    const auto high = log.filtered(
        [](const Event& item) { return item.value >= 20; });
    CZ_CHECK_INT((long)std::distance(high.begin(), high.end()), 2);
    const int total = std::accumulate(
        high.begin(), high.end(), 0,
        [](int sum, const Event& event) { return sum + event.value; });
    CZ_CHECK_INT(total, 60);

    const auto all_events = log.filtered([](const Event&) { return true; });
    CZ_CHECK_INT((long)std::distance(all_events.begin(), all_events.end()),
                 4);
    const auto no_events = log.filtered([](const Event&) { return false; });
    CZ_CHECK_INT((long)std::distance(no_events.begin(), no_events.end()), 0);

    ThresholdObserver high_observer(20);
    ThresholdObserver all_observer(0);
    EventBus bus;
    bus.subscribe(&high_observer);
    bus.subscribe(&all_observer);
    bus.subscribe(&high_observer);

    bus.publish(Event{1, "boot", 5});
    bus.publish(Event{2, "warn", 15});
    bus.publish(Event{3, "error", 25});
    bus.publish(Event{4, "fatal", 35});

    CZ_CHECK_INT(high_observer.matches(), 2);
    CZ_CHECK_INT(high_observer.total(), 4);
    CZ_CHECK_INT(all_observer.matches(), 4);
    CZ_CHECK_INT(all_observer.total(), 4);

    bus.unsubscribe(&all_observer);
    bus.publish(Event{5, "late", 30});
    CZ_CHECK_INT(high_observer.matches(), 3);
    CZ_CHECK_INT(high_observer.total(), 5);
    CZ_CHECK_INT(all_observer.total(), 4);

    bus.unsubscribe(&high_observer);
    bus.publish(Event{6, "after", 40});
    CZ_CHECK_INT(high_observer.total(), 5);

    return cztest_summary();
}
