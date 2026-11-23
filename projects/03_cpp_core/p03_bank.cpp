// p03_bank: a small bank ledger in C++17. C++ core capstone.
//
// It combines the C++ core and advanced chapters: classes with invariants,
// virtual dispatch, RAII and unique_ptr, exceptions, std::optional, STL
// containers and algorithms.
//
// Level 1 -- accounts
//   Account::deposit()       validate, credit, record a Transaction
//   Account::withdraw()      validate, check funds, debit, record
//   CheckingAccount / SavingsAccount override monthly_fee()
//
// Level 2 -- the bank
//   Bank::balance_of()       std::optional lookup
//   Bank::transfer()         find both accounts, withdraw then deposit
//   Bank::month_end()        apply every account's monthly fee
//
// Level 3 -- reporting
//   Bank::report_by_balance()  sort by balance descending, id ascending,
//                              and format "<id> <owner> <kind> <amount>"
//
// Ownership: Bank owns every Account through std::unique_ptr. Accounts are
// polymorphic, so the base destructor must stay virtual; deleting through a
// base pointer without it is undefined behaviour.
//
// Money is stored as integer cents. format_cents() is provided so the report
// never depends on floating-point rounding.
//
// From TS: a class with private fields and a Map of accounts is close, but
// here the base class needs a virtual destructor, ownership is explicit in
// unique_ptr, and invalid operations throw instead of returning undefined.
#include "cztest.h"

#include <algorithm>
#include <iomanip>
#include <memory>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

enum class TxKind { Deposit, Withdraw, Fee };

struct Transaction {
    TxKind kind;
    long long cents;
    std::string note;
};

/* Provided: format integer cents as a decimal amount, e.g. 9850 -> "98.50". */
std::string format_cents(long long cents)
{
    // TODO: print the amount as dollars and two decimal digits, e.g.
    // 9850 -> "98.50" and -5 -> "-0.05". Use integer arithmetic.
    (void)cents;
    return {};
}

class Account {
public:
    Account(int id, std::string owner, long long balance_cents)
        : balance_(balance_cents), id_(id), owner_(std::move(owner))
    {
    }

    virtual ~Account() = default;

    int id() const { return id_; }
    const std::string& owner() const { return owner_; }
    long long balance() const { return balance_; }
    const std::vector<Transaction>& history() const { return history_; }

    virtual std::string kind() const = 0;
    virtual long long monthly_fee() const = 0;

    void deposit(long long cents, const std::string& note)
    {
        // TODO: throw std::invalid_argument for cents <= 0; otherwise add the
        // amount to balance_ and record a TxKind::Deposit transaction.
        (void)cents;
        (void)note;
    }

    virtual void withdraw(long long cents, const std::string& note)
    {
        // TODO: throw std::invalid_argument for cents <= 0, throw
        // std::runtime_error when cents > balance_, otherwise subtract the
        // amount and record a TxKind::Withdraw transaction.
        (void)cents;
        (void)note;
    }

    void apply_monthly_fee()
    {
        // TODO: take monthly_fee(), subtract it from balance_, and record a
        // TxKind::Fee transaction when the fee is positive.
    }

protected:
    void record(TxKind kind, long long cents, const std::string& note)
    {
        // TODO: append Transaction{kind, cents, note} to history_.
        (void)kind;
        (void)cents;
        (void)note;
    }

    long long balance_;

private:
    int id_;
    std::string owner_;
    std::vector<Transaction> history_;
};

class CheckingAccount : public Account {
public:
    CheckingAccount(int id, std::string owner, long long balance_cents)
        : Account(id, std::move(owner), balance_cents)
    {
    }

    std::string kind() const override
    {
        // TODO: return "checking".
        return {};
    }

    long long monthly_fee() const override
    {
        // TODO: a checking account costs 150 cents per month.
        return 0;
    }
};

class SavingsAccount : public Account {
public:
    SavingsAccount(int id, std::string owner, long long balance_cents)
        : Account(id, std::move(owner), balance_cents)
    {
    }

    std::string kind() const override
    {
        // TODO: return "savings".
        return {};
    }

    long long monthly_fee() const override
    {
        // TODO: a savings account costs 250 cents per month.
        return 0;
    }
};

class Bank {
public:
    template <typename T, typename... Args>
    T& open(int id, Args&&... args)
    {
        static_assert(std::is_base_of<Account, T>::value,
                      "Bank::open requires an Account subclass");
        accounts_.push_back(
            std::make_unique<T>(id, std::forward<Args>(args)...));
        return static_cast<T&>(*accounts_.back());
    }

    Account* find(int id)
    {
        // TODO: return the account with that id, or nullptr.
        (void)id;
        return nullptr;
    }

    const Account* find(int id) const
    {
        // TODO: same lookup for a const Bank.
        (void)id;
        return nullptr;
    }

    std::optional<long long> balance_of(int id) const
    {
        // TODO: return the balance when find(id) succeeds, otherwise
        // std::nullopt.
        (void)id;
        return std::nullopt;
    }

    void transfer(int from_id, int to_id, long long cents,
                  const std::string& note)
    {
        // TODO: throw std::invalid_argument for from_id == to_id, throw
        // std::runtime_error when either account is missing, then withdraw
        // from the source and deposit into the destination with the same note.
        (void)from_id;
        (void)to_id;
        (void)cents;
        (void)note;
    }

    void month_end()
    {
        // TODO: call apply_monthly_fee() on every account.
    }

    long long total_balance() const
    {
        // TODO: sum every account's balance.
        return 0;
    }

    std::vector<std::string> report_by_balance() const
    {
        // TODO: collect const Account* pointers, sort by balance descending
        // and id ascending, then format each as
        //   "<id> <owner> <kind> <format_cents(balance)>".
        return {};
    }

private:
    std::vector<std::unique_ptr<Account>> accounts_;
};

int main()
{
    Bank bank;
    CheckingAccount& alice = bank.open<CheckingAccount>(1, "Alice", 10000);
    SavingsAccount& bob = bank.open<SavingsAccount>(2, "Bob", 5000);

    const std::string alice_kind = alice.kind();
    const std::string bob_kind = bob.kind();
    CZ_CHECK_STR(alice_kind.c_str(), "checking");
    CZ_CHECK_STR(bob_kind.c_str(), "savings");
    CZ_CHECK_INT(alice.balance(), 10000);
    CZ_CHECK_INT((long)alice.history().size(), 0);

    /* ---- Level 1: deposits, withdrawals, validation ------------------- */
    alice.deposit(2500, "salary");
    CZ_CHECK_INT(alice.balance(), 12500);
    CZ_CHECK_INT((long)alice.history().size(), 1);
    if (alice.history().size() == 1) {
        CZ_CHECK(alice.history()[0].kind == TxKind::Deposit);
        CZ_CHECK_INT(alice.history()[0].cents, 2500);
        CZ_CHECK_STR(alice.history()[0].note.c_str(), "salary");
    }

    alice.withdraw(500, "groceries");
    CZ_CHECK_INT(alice.balance(), 12000);
    CZ_CHECK_INT((long)alice.history().size(), 2);
    if (alice.history().size() == 2) {
        CZ_CHECK(alice.history()[1].kind == TxKind::Withdraw);
        CZ_CHECK_STR(alice.history()[1].note.c_str(), "groceries");
    }

    bool threw = false;
    try {
        alice.deposit(0, "bad");
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    CZ_CHECK(threw);

    threw = false;
    try {
        alice.withdraw(-1, "bad");
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    CZ_CHECK(threw);

    threw = false;
    try {
        alice.withdraw(999999, "too much");
    } catch (const std::runtime_error&) {
        threw = true;
    }
    CZ_CHECK(threw);
    CZ_CHECK_INT(alice.balance(), 12000);
    CZ_CHECK_INT((long)alice.history().size(), 2);

    /* ---- Level 2: transfers and optional lookup ----------------------- */
    bank.transfer(1, 2, 2000, "rent");
    CZ_CHECK_INT(alice.balance(), 10000);
    CZ_CHECK_INT(bob.balance(), 7000);
    CZ_CHECK_INT((long)alice.history().size(), 3);
    CZ_CHECK_INT((long)bob.history().size(), 1);
    if (alice.history().size() == 3) {
        CZ_CHECK(alice.history()[2].kind == TxKind::Withdraw);
        CZ_CHECK_STR(alice.history()[2].note.c_str(), "rent");
    }
    if (bob.history().size() == 1) {
        CZ_CHECK(bob.history()[0].kind == TxKind::Deposit);
        CZ_CHECK_STR(bob.history()[0].note.c_str(), "rent");
    }

    threw = false;
    try {
        bank.transfer(1, 99, 100, "missing");
    } catch (const std::runtime_error&) {
        threw = true;
    }
    CZ_CHECK(threw);

    threw = false;
    try {
        bank.transfer(1, 1, 100, "self");
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    CZ_CHECK(threw);
    CZ_CHECK_INT(alice.balance(), 10000);

    const std::optional<long long> found = bank.balance_of(1);
    CZ_CHECK(found.has_value() && *found == 10000);
    CZ_CHECK(!bank.balance_of(99).has_value());

    /* ---- Level 3: fees and report ------------------------------------- */
    bank.month_end();
    CZ_CHECK_INT(alice.balance(), 9850);
    CZ_CHECK_INT(bob.balance(), 6750);
    CZ_CHECK_INT((long)alice.history().size(), 4);
    CZ_CHECK_INT((long)bob.history().size(), 2);
    if (alice.history().size() == 4) {
        CZ_CHECK(alice.history()[3].kind == TxKind::Fee);
        CZ_CHECK_INT(alice.history()[3].cents, 150);
    }
    if (bob.history().size() == 2) {
        CZ_CHECK(bob.history()[1].kind == TxKind::Fee);
        CZ_CHECK_INT(bob.history()[1].cents, 250);
    }
    CZ_CHECK_INT(bank.total_balance(), 16600);

    const std::string formatted_total = format_cents(16600);
    const std::string formatted_negative = format_cents(-5);
    CZ_CHECK_STR(formatted_total.c_str(), "166.00");
    CZ_CHECK_STR(formatted_negative.c_str(), "-0.05");

    const std::vector<std::string> report = bank.report_by_balance();
    CZ_CHECK_INT((long)report.size(), 2);
    if (report.size() == 2) {
        CZ_CHECK_STR(report[0].c_str(), "1 Alice checking 98.50");
        CZ_CHECK_STR(report[1].c_str(), "2 Bob savings 67.50");
    }

    return cztest_summary();
}

// I AM NOT DONE
