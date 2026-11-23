# Switching from TypeScript to C and C++

This is a standalone bridge, not a comparison table. Read it once end to end,
type the examples, and you will be able to read and write C17/C++17 at the level
the exercises require. The exercises themselves make no assumptions about your
background; this document exists to make the switch fast if you already program
in TypeScript/JavaScript. Every exercise header also ends with a short
**From TS:** note that names the TS/JS habit the exercise replaces.

What changes in one sentence: you lose the garbage collector, the dynamic type
system and the runtime, and you gain direct control over memory, layout and
lifetimes. Everything below follows from that.

## 1. Toolchain: there is no npm

| TypeScript | C / C++ |
| --- | --- |
| `node app.js`, `ts-node` | compile, then run the produced binary |
| `tsconfig.json` | compiler flags (`-std=`, `-Wall`, `-I`, `-O2`) |
| `npm install` | system packages / vendored headers; no central registry |
| `import x from "./x"` | `#include "x.h"` (textual) + link the other `.cpp` |
| type checking before run | compile errors; nothing type-checks at runtime |
| `console.log` | `printf`, `std::cout` |

```sh
# C
cc -std=c17 -Wall -Wextra main.c -o app && ./app
# C++
c++ -std=c++17 -Wall -Wextra main.cpp -o app && ./app
```

`cc`/`c++` are clang on macOS and gcc on most Linux installs; both accept these
flags. A C++ file is still compiled and linked by one command for a single-file
program. Multi-file projects compile each translation unit and link the objects;
that is what build tools (Make, CMake) automate.

## 2. The smallest program, and what each part means

```cpp
#include <cstdio>        // declares printf; textual inclusion, not an import

int main()               // entry point; returns the process exit status
{
    std::printf("hello\n");
    return 0;            // 0 = success, non-zero = failure
}
```

* `#include` pastes the contents of a header at that point. Headers declare
  things; the linker finds the definitions.
* Statements end with `;`. Blocks are `{ }`. There is no automatic semicolon
  insertion and no hoisting: declare before use.
* `main` returns `int`. `return 0;` is implicit in C++ but write it.
* Text in `//` or `/* */` is a comment.

Run it, then break it on purpose: remove the `;`, remove the `#include`, write
`main` as `void`. Read each compiler error. That skill is half of C++.

## 3. Types: no `number`, no `any`, no `undefined`

```cpp
int      count = 0;        // 32-bit on every target you will meet
unsigned mask  = 0u;       // wraps modulo 2^32 on overflow (defined)
long long big  = 0LL;      // at least 64 bits
std::size_t n  = 0;        // unsigned size type, result of sizeof
double   ratio = 0.5;      // IEEE-754 binary64, same as JS number
bool     ok    = true;
char     letter = 'A';     // one byte, NOT a one-character string
```

* `char` is a byte. Text is `char` arrays or `std::string`; UTF-8 by convention,
  not a language guarantee.
* `sizeof(T)` gives bytes. `sizeof(int)` is 4 here but the standard only
  guarantees 2 or more; do not hard-code layout assumptions.
* Integer division truncates: `7 / 2 == 3`, `-7 / 2 == -3` (C++11 and later).
  `%` is the remainder with the sign of the dividend. Use `double` when you mean
  fractions: `7 / 2.0`.
* Conversions are implicit and silent: `double` → `int` truncates, negative →
  `unsigned` becomes huge. `-Wconversion` shows them.
* Signed integer overflow is **undefined behaviour**, not wrap-around. Unsigned
  overflow is defined modulo 2^N.
* Initialization: `int x;` leaves it uninitialized (reading it is UB). Prefer
  `int x = 0;`, `int x{};` or `int x{0};`. `T x{}` is value-initialization.
* `auto` deduces from the initializer (like `const` inference, but a real type).
  It does not mean "any".
* `nullptr` is the null pointer. There is no `undefined`; an absent value is
  `nullptr`, a sentinel, or `std::optional<T>`.
* `void*` is untyped memory; unlike `any`, you cannot use it without a cast.

## 4. Values, references and why assignment surprises you

In TypeScript, `const b = a` for objects shares one object. In C and C++, every
variable (including a class instance) is its own object unless you explicitly
use a reference or pointer.

```cpp
int a = 1;
int b = a;      // copy
b = 2;          // a is still 1

std::string s = "hi";
std::string t = s;   // deep copy of the bytes
t += "!";            // s is still "hi"

std::string& r = s;  // reference: another name for s, no copy
r += "?";            // s is now "hi?"
std::string* p = &s; // pointer: stores s's address, may be nullptr
p->size();           // same as (*p).size()
```

* A reference (`T&`) must bind to an object at initialization, cannot be null
  and cannot be reseated. Use it for parameters that must exist and for
  avoiding copies.
* A pointer (`T*`) is an address, can be `nullptr`, can be reassigned, and can
  outlive its pointee. Dereferencing it is your responsibility.
* An array used in an expression decays to a pointer to its first element:
  inside a function taking `int arr[]`, `sizeof(arr)` is `sizeof(int*)`. Pass the
  length, or use `std::array`/`std::vector`.
* For function parameters: small trivially-copyable types by value, large or
  non-copyable ones by `const T&` (read-only) or `T&` (mutate). Return by value;
  copy elision and move make it cheap.

## 5. Control flow: nearly identical

```cpp
if (x > 0) { ... } else if (x == 0) { ... } else { ... }
int m = a > b ? a : b;
switch (kind) {
case 1:
    do_one();
    break;          // without break, execution falls through
default:
    do_default();
}
for (int i = 0; i < n; ++i) { ... }
while (condition) { ... }
do { ... } while (condition);
for (const auto& item : container) { ... }   // range-for, like for..of
```

* Conditions are integers/pointers in C; an `int` converts to `bool` (0 false).
  C++ has real `bool`. `if (ptr)` means "not null".
* `switch` cases fall through unless you `break`; that is a feature and a bug.
* `break` leaves the loop/switch, `continue` goes to the next iteration.
* `for..of` maps to range-for over anything with `begin()`/`end()`.
* No `for..in` over object keys; iterate a map explicitly.

## 6. Functions

```cpp
int add(int a, int b);              // declaration (prototype), needed before use
int add(int a, int b) { return a + b; }

double scale(double x, double factor = 2.0);   // default argument
int max(int, int);                             // overload
double max(double, double);                    // distinct function

void swap(int& a, int& b);          // mutate the caller's variables
bool parse(const std::string& text, int& out);  // const-ref in, ref out
```

* Declare before use. A prototype lets you define later or in another file.
* Overloading selects by parameter types/count at compile time; no runtime
  dispatch and no `any` fallback.
* Pass by value copies; pass by reference shares; pass by pointer shares and
  allows null. There is no optional parameter object; use defaults or overloads.
* Returning multiple values: return a `struct`, `std::pair`, `std::tuple`, or use
  out-parameters (`bool parse(const std::string&, int& out)`).
* `[[nodiscard]]` warns when a return value is ignored, useful for error codes.
* Recursion is not special, but stack space is finite (typically 8 MiB).

## 7. Strings and text

```cpp
const char* cstr = "hello";          // pointer to static NUL-terminated bytes
std::string s = "hello";             // owning, mutable, length-tracked
s += " world";
s.size();                            // 11, no terminator counted
const char* raw = s.c_str();         // valid while s lives and is unmodified
std::string_view view(s);            // non-owning (pointer, length) slice
```

* A C string is a `char` array ending in `'\0'`. `strlen` scans until it finds
  that byte; forgetting the terminator reads past the buffer.
* Buffer sizes include the terminator: `"abc"` needs 4 bytes.
* `std::string` copies on assignment and grows automatically; `+` concatenates.
* `std::string_view` avoids copies but does not own anything and does not keep
  the source alive. Never return a view of a local.
* Number ↔ text: `std::to_string(42)`, `std::stoi(s)`, `std::stod(s)`. The
  `sto*` functions throw on bad input. In C: `snprintf`, `sscanf`, `strtol`.
* `std::getline(std::cin, line)` reads a line. `std::cin >> x` reads a
  whitespace-separated token.
* No UTF-16 string type and no Unicode-aware indexing. `std::string` indexes
  bytes. Use a library (ICU, utf8cpp) when you need real Unicode operations.

## 8. Collections

```cpp
int raw[5] = {1, 2, 3, 4, 5};            // fixed size, no bounds checks
std::array<int, 5> fixed{1, 2, 3, 4, 5}; // fixed size, value semantics
std::vector<int> v{1, 2, 3};             // growable array, owns its buffer
v.push_back(4);
v.size(); v.capacity();
v[0];                                     // unchecked
v.at(10);                                 // throws std::out_of_range
for (int x : v) { ... }

std::map<std::string, int> counts;   // ordered by key, O(log n)
counts["word"]++;                    // inserts 0 if absent
std::unordered_map<std::string, int> hash;  // O(1) average, unordered
std::set<int> unique;                // ordered, no duplicates
std::pair<int, int> p{1, 2};         // p.first, p.second
std::tuple<int, std::string> t{1, "a"};  // std::get<0>(t)
```

* `vector` is the default sequence; `map`/`unordered_map` the default dictionary;
  `set`/`unordered_set` for membership.
* `operator[]` on `vector` and `map` does not range-check (`map::operator[]`
  inserts). Use `at`, `find`, `contains` (C++20 for map/set; C++17: `count`).
* Container growth invalidates pointers, references and iterators into it. This
  is the C++ equivalent of "the array was reallocated".
* Iterate with range-for; when you need the index, use `size_t` and `< size()`.
* `std::array` is a value type: copying copies the elements.

## 9. Classes: nominal, by value, with destructors

```cpp
class Rectangle {
public:
    Rectangle(double w, double h) : w_(w), h_(h) {}   // initializer list
    double area() const { return w_ * h_; }           // const: no mutation
    void scale(double f) { w_ *= f; h_ *= f; }

private:
    double w_ = 0;   // in-class initializer
    double h_ = 0;
};
```

* Members initialize in **declaration order**, regardless of the order in the
  initializer list. `-Wreorder` catches the discrepancy.
* `struct` and `class` differ only in default access (`public` vs `private`).
* `const` after the parameter list marks a method that does not mutate the
  object; only such methods can be called on a `const` object.
* Constructors run when an object is created, destructors (`~T()`) when it goes
  out of scope. There is no GC: the destructor is your cleanup hook.
* Copying is memberwise by default. If the class owns a resource, write the
  destructor, copy constructor and copy assignment (rule of three), and usually
  move operations too (rule of five).
* `explicit` on a one-argument constructor prevents accidental implicit
  conversions (`std::vector<int> v = 5;`).
* Inheritance is nominal: `struct Dog : Animal { ... };`. There is no structural
  typing and no `implements`; an interface is an abstract class with pure
  virtual functions (`virtual void speak() = 0;`).
* `virtual` enables dispatch through a base pointer/reference. Add `override` on
  overrides; a polymorphic base needs a virtual destructor.
* `enum class Color { Red, Green };` — scoped, no implicit int conversion.
* No `this` binding surprises: `this` is a pointer, always the current object.

## 10. Lifetime, ownership and undefined behaviour

```cpp
{
    std::string name = "Ada";   // constructed on the stack
}                               // destructor runs here, deterministically

int* bad()
{
    int local = 7;
    return &local;              // dangling: the frame is gone
}

std::unique_ptr<Widget> w = std::make_unique<Widget>();  // sole owner
std::shared_ptr<Widget> s = std::make_shared<Widget>();  // reference-counted
std::weak_ptr<Widget> weak = s;                          // observe, don't own
```

* Stack: function locals, freed at scope exit. Heap: `malloc`/`free` in C,
  `new`/`delete` and containers in C++. Heap objects live until released.
* Prefer values and containers. When you need heap polymorphism or shared
  ownership, use `unique_ptr` (single owner, movable, no overhead) and
  `shared_ptr` (refcounted; cycles leak).
* Never mix `new[]` with `delete` or `malloc` with `free`; never double-free.
* These are undefined behaviour: out-of-bounds access, uninitialized reads,
  dangling/null dereference, signed overflow, division by zero, use after free,
  breaking strict aliasing, modifying a `const` object.
* UB may appear to work. The optimizer is allowed to assume it cannot happen.
  Run `CZ_SAN=1 czlings run <name>` in this repository to get AddressSanitizer
  and UBSan reports with stack traces.
* Returning a reference/pointer/view to a local is invalid. Returning by value
  is fine (copy elision).
* A moved-from object is valid but unspecified: destroy it or assign to it.
  Do not read its contents.

## 11. Errors: return codes, exceptions, optional

```cpp
// C: return status, out-parameter on success
bool read_count(const char* path, int* out);

// C++: throw on failure, catch at a boundary
struct Parse_error : std::runtime_error {
    using std::runtime_error::runtime_error;
};

int parse_or_throw(const std::string& text)
{
    if (text.empty()) { throw Parse_error("empty input"); }
    return std::stoi(text);
}

std::optional<int> try_parse(const std::string& text);  // absence without throw
```

* In C, every fallible function returns a status and sets `errno`; ignoring it is
  a bug.
* In C++, throw for exceptional conditions (I/O, allocation, invalid external
  input). Destructors run during unwinding, which is how RAII cleans up.
* Do not use exceptions for expected branches; do not throw from a destructor.
* `noexcept` promises no exception; violating it calls `std::terminate`.
* `std::optional<T>` is a value that may be empty — closer to `T | undefined`
  than to a nullable pointer, but it is still a value type.
* `std::variant<A, B>` is a closed tagged union, not a dynamic object with
  arbitrary shape.
* C++17 has no `std::expected`; use `optional`, a status enum, or a result
  struct. (C++23 adds `std::expected`.)

## 12. Generics, lambdas, and the standard library

```cpp
template <typename T>
T max_of(T a, T b) { return a > b ? a : b; }

std::vector<int> v{3, 1, 2};
std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; });

int limit = 10;
auto under = [limit](int x) { return x < limit; };  // capture by value
auto bump  = [&limit]() { ++limit; };               // capture by reference
```

* Templates are instantiated at compile time: one function per type, no type
  erasure, no reflection. Definitions must be visible where used, so they live
  in headers.
* A lambda is a compiler-generated class with captured members. Capturing by
  reference stores addresses; a lambda that outlives the local dangles. No GC
  keeps captures alive.
* `std::function` stores any callable (type erasure, possible allocation).
* Standard algorithms (`<algorithm>`, `<numeric>`): `sort`, `find_if`,
  `count_if`, `accumulate`, `transform`, `any_of`, `all_of`, `min_element`.
  They take iterators, not containers.
* Iterators are generalized pointers: `begin()`/`end()`, `++`, `*`. A range is
  half-open: `[begin, end)`.
* `constexpr` moves computation to compile time; `static_assert` fails the
  build with a message. Use them for invariants.
* `std::function`, `std::optional`, `std::variant` are in `<functional>`,
  `<optional>`, `<variant>`.

## 13. Namespaces, headers, separate compilation

```cpp
// math_utils.h
#pragma once
int add(int a, int b);          // declaration only

// math_utils.cpp
#include "math_utils.h"
int add(int a, int b) { return a + b; }

// main.cpp
#include "math_utils.h"
int main() { return add(1, 2) == 3 ? 0 : 1; }
```

```sh
c++ -std=c++17 -Wall -Wextra math_utils.cpp main.cpp -o app
```

* Each `.cpp` is a translation unit compiled independently; the linker resolves
  declarations to definitions. `undefined reference to 'add'` means the
  definition was not compiled or not linked.
* A header included twice in one translation unit must not define anything
  twice: use `#pragma once` or an include guard. Functions defined in headers
  must be `inline` (or templates).
* `namespace ns { ... }` groups names; access with `ns::name`. Prefer qualified
  names over `using namespace`.
* `static` at file scope gives internal linkage (file-private); `extern`
  declares a definition from another translation unit.
* There are no modules in C++17 (C++20 modules exist but are not assumed here).

## 14. Side-by-side cookbook

```ts
// TypeScript
const xs = [1, 2, 3];
const doubled = xs.map(x => x * 2);
const total = xs.reduce((a, b) => a + b, 0);
const byId = new Map<string, number>([["a", 1]]);
const found = byId.get("a") ?? 0;
```

```cpp
// C++17
std::vector<int> xs{1, 2, 3};
std::vector<int> doubled;
for (int x : xs) { doubled.push_back(x * 2); }

std::vector<int> doubled2(xs.size());
std::transform(xs.begin(), xs.end(), doubled2.begin(),
               [](int x) { return x * 2; });

int total = std::accumulate(xs.begin(), xs.end(), 0);

std::map<std::string, int> by_id{{"a", 1}};
int found = 0;
if (auto it = by_id.find("a"); it != by_id.end()) { found = it->second; }
```

| Task | TypeScript | C++17 |
| --- | --- | --- |
| formatted string | `` `${name}: ${n}` `` | `name + ": " + std::to_string(n)` or `snprintf` |
| array length | `xs.length` | `xs.size()` |
| add element | `xs.push(x)` | `xs.push_back(x)` |
| remove by value | `xs = xs.filter(y => y !== x)` | `xs.erase(std::remove(xs.begin(), xs.end(), x), xs.end())` |
| sort copy | `[...xs].sort((a,b) => a-b)` | `std::sort(xs.begin(), xs.end())` (in place) |
| dictionary | `new Map<K,V>()` | `std::map<K,V>` / `unordered_map` |
| membership | `set.has(x)` | `set.count(x) != 0` |
| absent value | `T \| undefined` | `std::optional<T>` |
| throw | `throw new Error(...)` | `throw std::runtime_error(...)` |
| catch | `try { } catch (e) { }` | `try { } catch (const std::exception& e) { }` |
| class field | `constructor(private x: number) {}` | member + initializer list |
| interface | `interface I { f(): void }` | abstract class with pure virtual `f` |
| union type | `A \| B` | `std::variant<A, B>` |
| closure | `() => x` | `[x]() { return x; }` |
| generic | `<T>(x: T) => T` | `template <typename T> T f(T x)` |
| async | `Promise<T>` | not a language feature; threads/futures, different model |

## 15. Tooling: compiler flags, debugging, sanitizers

```sh
c++ -std=c++17 -Wall -Wextra -Wpedantic -g main.cpp -o app   # development
c++ -std=c++17 -O2 main.cpp -o app                          # release
```

* `-Wall -Wextra` enable the useful warnings. Warnings are bugs you have not
  hit yet; fix them.
* `-g` keeps debug info for `lldb`/`gdb`: `lldb ./app`, `break main`, `run`,
  `next`, `print x`, `bt`.
* `-fsanitize=address,undefined` instruments memory and UB checks at runtime.
* `-O0` for debugging, `-O2` for speed; behaviour differences between them are
  usually UB.
* `-I dir` adds an include directory, `-L dir -lname` link paths/libraries.
* `CMake` or `Make` for multi-file projects; you do not need them for the
  exercises.
* There is no `npm test`. Tests in this repository are plain `main()` programs
  returning 0 on success.

## 16. Porting checklist

When you turn TS into C++, do these in order:

1. Decide the data representation: `number` → `int`/`long long`/`double`;
   object literal → `struct`/`class`; `T[]` → `std::vector<T>`; `Record` →
   `std::map`/`unordered_map`; `T | undefined` → `std::optional<T>` or `nullptr`.
2. Make ownership explicit: who allocates, who frees, which object owns the
   buffer. Prefer values; use `unique_ptr` for polymorphism; `shared_ptr` only
   when ownership is genuinely shared.
3. Replace closures with lambdas only where they do not escape; otherwise pass a
   function object or `std::function`.
4. Replace exceptions-from-anywhere with a deliberate policy: exceptions at the
   boundary, status codes/`optional` for ordinary failures.
5. Copy vs reference: function parameters `const T&` for read-only, `T&` to
   mutate, `T` for small values.
6. Compile with `-Wall -Wextra` and fix every warning before debugging logic.
7. Test with sanitizers; a clean ASan/UBSan run is a baseline, not a bonus.
8. Only then optimize; measure first.

## 17. What to unlearn

* **GC**: objects die at scope exit or when the last owner dies. Leaks are real.
* **Dynamic typing**: types are fixed at compile time; conversions are explicit
  in the type system but implicit at the syntax level.
* **`undefined`/`null` everywhere**: nullability is a pointer property or
  `optional`; dereferencing null is UB/crash.
* **Exceptions are always available and cheap**: C has none; C++ exceptions are
  for exceptional paths and destructors must not throw.
* **Closures capture by GC**: capture by reference is an address; lifetime is
  your problem.
* **Async/await**: there is no event loop. Concurrency is threads/atomics/
  futures, a different model entirely.
* **Structural typing**: class compatibility is by declared inheritance, not by
  shape.
* **npm packages**: dependencies are system libraries or vendored source.

## 18. Two-week plan

| Day | Work |
| --- | --- |
| 1 | Sections 1–4; compile, break and fix hello world; `exercises/00_beginner` b01–b04 |
| 2 | Sections 5–6; b05–b07; write `sum_to`, `max3` yourself |
| 3 | Sections 7–8; b08–b10; then `01_c_basics` c01–c04 |
| 4–5 | Finish `01_c_basics`; reread section 4 before pointers |
| 6–8 | Section 10 + `02_c_advanced` c13–c18 (pointers, const, structs) |
| 9–10 | c19–c26 (unions, malloc, linked list, files, preprocessor, bits) |
| 11–12 | Section 9 + `03_cpp_core` cpp01–cpp08 (classes, RAII, copy) |
| 13–14 | cpp09–cpp18 (operators, inheritance, virtual, exceptions, move) |
| after | Section 12 + `04_cpp_advanced` at your own pace |

If a chapter takes twice as long, that is normal. Pointer arithmetic and move
semantics are where TS intuition stops helping; slow down there.

After this document plus the exercises you can: read a C/C++ codebase, write
single- and small multi-file programs, manage memory deliberately, design a
class with correct copy/move/destruction, use the STL, and debug with warnings
and sanitizers. That is the switch.
