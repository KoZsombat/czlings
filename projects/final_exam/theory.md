# Theory — 10 points

Answer without a compiler. Each true/false question is worth 1 point. The
short-answer section is for revision, not scored.

## True or false

1. After `int *const p = &x;`, the pointer `p` can be reassigned to point at
   another `int`.
2. `malloc` returns memory whose bytes are initialised to zero.
3. `free(NULL)` is undefined behaviour.
4. A `qsort` comparator must describe a consistent total order.
5. In C, `sizeof(char)` is always 1.
6. A C++ destructor can be declared `virtual`.
7. Deleting an object through a base pointer whose destructor is not virtual is
   undefined behaviour.
8. `std::unique_ptr<T>` can be copied.
9. `std::move(x)` itself moves the bytes of `x`.
10. `std::vector::push_back` can invalidate existing iterators.

## Short answers

A. State the rule of three and the rule of five. Why do they exist?
B. When is `std::optional<T>` clearer than a sentinel value such as `-1` or
   `nullptr`?
C. What is the difference between `std::endl` and `'\n'`?
D. Why can a `const` member function modify a `mutable` member?

---

## Answers

1. **False.** `int *const p` is a const pointer: the pointer is fixed, but
   `*p` can change. Read declarations right to left.
2. **False.** `malloc` returns uninitialised storage. `calloc` zeroes it.
3. **False.** `free(NULL)` is a no-op, explicitly allowed by the standard.
4. **True.** The comparator must be antisymmetric and transitive; otherwise
   `qsort` and `bsearch` have no defined order to work with.
5. **True.** `sizeof(char) == 1` by definition. `CHAR_BIT` may be larger than
   8 on exotic machines, but one `char` is still one byte.
6. **True.** A destructor can be virtual, and a polymorphic base should have a
   virtual destructor.
7. **True.** The base subobject is destroyed, the derived part is not, and the
   behaviour is undefined. Declare `virtual ~Base() = default;`.
8. **False.** `unique_ptr` is move-only. Copying would create two owners.
9. **False.** `std::move` is a cast to an rvalue reference. The move
   constructor or move assignment does the actual transfer.
10. **True.** Reallocation moves the elements, so every pointer, reference and
    iterator into the old buffer is invalidated.

A. **Rule of three:** if a class needs a destructor, copy constructor or copy
   assignment, it usually needs all three. **Rule of five** adds the move
   constructor and move assignment. They exist because a raw resource must be
   released once, copied deeply, and transferred on move.

B. `std::optional<T>` makes "no value" a type-level state instead of reserving
   a magic value. It is clearer when every `T` value is legal, e.g.
   `std::optional<int>` for a lookup that may miss.

C. `std::endl` writes `'\n'` **and** flushes the stream. `'\n'` only writes the
   newline; the buffer flushes when it fills or when the stream is closed.
   Flushing every line is usually unnecessary and slower.

D. `mutable` tells the compiler that the member is part of the object's
   observable state but may change during a logically const operation, e.g. a
   cache or a mutex.
