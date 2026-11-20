# From TypeScript to C and C++

You already program. What changes is the machine model: C and C++ expose the
things a JavaScript engine hides (memory, layout, ownership, dispatch). This
chapter is the mental bridge; the exercises then drill each idea.

## 1. There is no runtime

TypeScript runs on a JIT with garbage collection, dynamic property lookup and a
single `number` type. C and C++ are compiled ahead of time to machine code:

| TypeScript | C / C++ |
| --- | --- |
| syntax/type error | compile error |
| `throw`, rejected promise | exception (C++) or error code (C) |
| `undefined is not a function` | crash or **undefined behaviour** |
| GC frees objects | you free them, or a destructor does |
| `number` is IEEE-754 double | `int`, `long`, `float`, `double`, ... |
| objects live on the heap | values live on the stack, unless you ask for the heap |

Three failure classes, not one:

1. **Compile error** — the program never exists. Has a file:line and often a
   `note:` explaining the real cause.
2. **Runtime failure** — the program dies, usually a signal: null dereference,
   stack overflow, assertion.
3. **Undefined behaviour (UB)** — the standard imposes no requirements. It may
   "work" in your test, print garbage, or delete your files at `-O2`. This is
   the one TS has no equivalent for.

## 2. Values and references

In TS, primitives are copied and objects are shared through references. In C
and C++ *every* object is a value by default, and sharing is explicit.

```cpp
int a = 1, b = a;      // two ints, independent
b = 2;                 // a is still 1

std::string s = "hi";
std::string t = s;     // deep copy: two buffers
t += "!";              // s is still "hi"

std::string& r = s;    // alias: no copy, cannot be reseated
std::string* p = &s;   // address: may be null, may be reseated
```

* A **reference** (`T&`) is another name for an existing object. It must be
  initialized, cannot be null, and cannot point elsewhere later.
* A **pointer** (`T*`) holds an address. It can be null, can be reassigned, and
  can outlive the object it points to — that is how dangling pointers happen.
* An **array** in an expression decays to a pointer to its first element, so
  `sizeof(arr)` inside a function that took `int arr[]` is `sizeof(int*)`, not
  the array size. `std::array`/`std::vector` do not decay.

Objects in TS are reference-counted by the GC; a C++ class instance is a value
whose lifetime is a lexical scope:

```cpp
{
    std::string name = "Ada";   // constructed
}                               // destructor runs here, deterministically
```

## 3. Memory: stack, heap, and no GC

Every function call gets a stack frame. Locals live there and die at the closing
brace. The heap is manual (`malloc`/`free` in C, `new`/`delete` and containers in
C++) and lives until you release it.

* Returning a pointer to a local is the classic bug: the frame is gone, the
  pointer dangles.
* `malloc`/`new` without the matching `free`/`delete` leaks. A long-running
  server leaks until it dies; a short exercise just hides the bug.
* C++ replaces "remember to free" with **RAII**: a destructor runs when the
  object leaves scope, so a wrapper type frees its resource automatically.
  `std::string`, `std::vector`, `std::unique_ptr` are such wrappers.
* `std::unique_ptr<T>` is the closest thing to a linear reference: exactly one
  owner, transfer with `std::move`. `std::shared_ptr<T>` is reference-counted,
  semantically closest to a JS object reference (but cycles still leak).
* C has no destructors. Every `malloc` needs a matching `free` on every path,
  including early returns and failures. That asymmetry is most of why C is
  considered harder to write safely.

TS code that recurses 100k deep works; C++ will overflow the (typically 8 MiB)
stack. Recursion depth is a real resource.

## 4. Types, integers and overflow

`number` disappears. Pick a width and signedness:

```cpp
int          x = 42;        // at least 16 bits, 32 on every platform you'll use
unsigned     u = 42u;       // wraps modulo 2^32 on overflow (defined)
long long    big = 0LL;     // at least 64 bits
std::size_t  n = 0;         // unsigned, result type of sizeof / container size
double       d = 0.1;       // same IEEE-754 binary64 as JS number
```

* Signed overflow is **undefined**, not wrap-around. `INT_MAX + 1` may be
  negative, may be optimized away, may trap. Use unsigned arithmetic when you
  want modular wrap, and check bounds before adding.
* Conversions happen silently: `int` to `double`, `int` to `unsigned` (negative
  becomes huge), `double` to `int` (truncates). `-Wconversion` exposes them.
* `char` is one byte (8 bits on every target you'll meet) and may be signed or
  unsigned — do not assume. Text is bytes, usually UTF-8, with no built-in
  Unicode semantics.
* There is no `undefined`. A missing value is `nullptr` (pointer), a sentinel,
  or `std::optional<T>` in C++. Reading an uninitialized local is UB, not
  `undefined`.
* There is no `any`. `void*` exists in C but converts to nothing useful without
  a cast; C++ has templates and `std::variant` instead.

## 5. Undefined behaviour

These are UB in C/C++ and have no TS analogue:

* reading an uninitialized variable;
* indexing outside an array or `std::vector` with `operator[]` (no bounds check);
* dereferencing `nullptr` or a dangling pointer;
* signed integer overflow, division by zero;
* modifying a `const` object, breaking strict aliasing;
* using an object after its lifetime ended (moved-from objects are the legal
  exception, but only in their documented state).

The compiler may assume UB never happens. A loop that would overflow can be
deleted, a null check can be removed, a buffer overrun can overwrite the return
address. Tests that pass in a debug build can fail at `-O2`.

Run the memory exercises with sanitizers:

```sh
CZ_SAN=1 czlings run c22_linked_list
```

AddressSanitizer reports use-after-free, out-of-bounds and leaks with a stack
trace at the moment of the access. Treat any report as a real bug, not noise.

## 6. Strings

Two different things share the name "string":

* **C string**: `char[]` terminated by a `'\0'` byte. `strlen` scans until the
  terminator, so forgetting it reads past the buffer. Buffer size includes the
  terminator: `"abc"` needs 4 bytes.
* **`std::string`** (C++): owns a byte buffer, tracks length, copies on
  assignment, grows automatically. Still bytes, not UTF-16 code units.
  `s.length()` excludes the terminator; `s.c_str()` gives a temporary
  `const char*` valid only while `s` lives and is unmodified.

`std::string_view` is a non-owning `(pointer, length)` pair — like a slice, with
no GC to keep the underlying buffer alive. Dangling views are a common bug.

## 7. Build model: translation units and linking

`#include "x.h"` is a textual copy-paste by the preprocessor, not a module
import. Each `.c`/`.cpp` compiles independently into an object file; the linker
then resolves symbols.

* A **declaration** (`int add(int, int);`) promises a definition exists.
* A **definition** allocates storage or provides the body.
* `undefined reference to 'add'` is a *link* error: the declaration was visible,
  the definition was not compiled or not linked.
* Headers are included by many translation units, so they contain declarations,
  inline functions, templates and include guards (`#pragma once`).
* `static` at file scope means internal linkage (the TS equivalent of a
  module-private binding); `extern` declares a definition living elsewhere.
* `main` is the entry point; its return value becomes the process exit status.
  `argc`/`argv` are the command-line arguments — no `process.argv`.

Arcane GCC/Clang errors are usually one root cause plus notes. Read the first
error, not the last.

## 8. Functions and generics

* Functions must be declared before use (no hoisting). A prototype at the top of
  the file fixes order.
* Overloading selects by parameter types and count; defaults fill in missing
  trailing arguments. Ambiguous calls are compile errors.
* Templates are compile-time: `template <typename T> T max(T, T)` generates a
  separate function per `T`. No type erasure, no reflection, no runtime generic
  metadata. Code must be in headers so each translation unit can instantiate it.
* Function pointers (`int (*cmp)(const void*, const void*)`) are first-class
  but capture nothing; C++ lambdas add captures (see §11).
* Namespaces group names: `std::string`, `my::util::parse`. Prefer qualified
  names to `using namespace`.

## 9. Objects

```cpp
class Rectangle {
public:
    Rectangle(double w, double h) : w_(w), h_(h) {}   // initializer list
    double area() const { return w_ * h_; }           // const: does not mutate
private:
    double w_, h_;
};
```

* Members initialize in **declaration order**, not in the order written in the
  initializer list.
* A `const` member function can be called on a `const` object; it promises not
  to mutate members.
* Copying is memberwise by default; if the class owns a raw resource you must
  write the copy constructor, copy assignment and destructor (rule of three),
  plus move operations for efficiency (rule of five).
* Inheritance is nominal, unlike TS structural typing. `virtual` enables dynamic
  dispatch through a base pointer; without it you get the static type's
  function. A base class used polymorphically needs a virtual destructor.
* `struct` and `class` differ only in default access (`public` vs `private`).
* There are no interfaces; use an abstract class with pure virtual functions.

## 10. Errors

C reports failure with return values and `errno`; ignoring them is a bug. C++
adds exceptions:

* throw on exceptional, unpreventable failure (allocation, I/O, invalid input
  from outside);
* do not use exceptions for ordinary control flow;
* destructors run while the stack unwinds — that is how RAII releases resources
  on the error path;
* `noexcept` marks functions that cannot throw; violating it terminates;
* the *strong exception guarantee* means an operation either succeeds or leaves
  the object unchanged;
* `std::optional<T>` models "may be absent" without exceptions, the way
  `T | undefined` does — but it is a value, not a reference.

## 11. Lambdas and closures

```cpp
int base = 10;
auto add = [base](int x) { return base + x; };   // capture by value (copy)
auto inc = [&base](int x) { base += x; };        // capture by reference
std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; });
```

A lambda is a compiler-generated class with captured members. Capturing by
reference stores an address: if the lambda outlives the local, it dangles. JS
closures are kept alive by the GC; C++ closures are not. `std::function` stores
any callable but may allocate and adds indirect-call overhead.

## 12. Cheat sheet

| TypeScript | C / C++ |
| --- | --- |
| `let x = 1` | `int x = 1;` |
| `const x = 1` | `const int x = 1;` |
| `{ a: 1, b: 2 }` | `struct S { int a, b; }; S s{1, 2};` |
| `number` | `int`, `long long`, `double` |
| `string` | `char[]` (C) / `std::string` (C++) |
| `T[]` | `T arr[N]`, `std::vector<T>` |
| `Record<K, V>` / `Map` | `std::map<K, V>`, `std::unordered_map` |
| `T \| null` | `T*` with `nullptr`, `std::optional<T>` |
| `===` | `==` (operator semantics, not identity) |
| `console.log` | `printf`, `std::cout` |
| template literal | `snprintf`, `std::ostringstream` |
| `try/catch` | exceptions (C++), error codes (C) |
| `JSON.parse` | manual parsing, `std::istringstream` |
| GC | RAII, `std::unique_ptr`, `std::shared_ptr` |
| `npm test` | `czlings verify` |
| `process.argv` | `argc`, `argv` |
| `for..of` | range-for (C++), index loop (C) |

## 13. How to work the exercises

1. Read the task comment and the failing checks.
2. Run `czlings` (watch mode) and keep it open; it recompiles on save.
3. Read the first compiler error, fix that, repeat. Warnings matter: they are
   often latent UB.
4. When all checks pass, delete `// I AM NOT DONE`.
5. For memory exercises, rerun with `CZ_SAN=1` and require a clean run.

Hints are deliberately one step short of the answer. `czlings solution <name>`
exists for when you are truly stuck; read it, then `czlings reset <name>` and
write it yourself.
