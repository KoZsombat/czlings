# czlings

Learn **C17 and C++17** by fixing small broken programs. Rustlings-style: a
watcher runs the next exercise, you edit the file until its checks pass, delete
`// I AM NOT DONE`, and it moves on. Hints and reference solutions are included.

**78 exercises, starting from zero.** No prior programming experience is
assumed; chapter 0 starts at "print a line of text". The later chapters reach
BME VIK computer-engineering exam level (pointers, manual memory, classes, RAII,
templates, STL, data structures).

## Who it is for

| You are… | Start at |
| --- | --- |
| completely new to programming | [docs/00-start-here.md](docs/00-start-here.md), then `exercises/00_beginner` |
| a TypeScript/JavaScript developer | [docs/ts-to-c-cpp.md](docs/ts-to-c-cpp.md), then `exercises/01_c_basics` |
| preparing for a BME VIK Prog1/Prog2 exam | chapters 1–4, see the course mapping below |
| here for data structures | chapters 2–4 (`c22`, `c31`, `c32`, `cpp30`–`cpp34`) |

## Quick start

```sh
git clone https://github.com/KoZsombat/czlings.git
cd czlings
cargo install --path . --force   # build the runner once
make manifest                    # info.toml from manifest.d/
czlings                          # watch mode
```

Requirements: a C compiler (`cc`), a C++ compiler (`c++`), git, and Rust (only
to build the runner — you never write Rust). Per-OS install instructions are in
[docs/00-start-here.md](docs/00-start-here.md).

Without installing: `cargo run --release -- watch`.

## Commands

| Command | What it does |
| --- | --- |
| `czlings` / `czlings watch` | Clear the screen, run the next unfinished exercise, re-run on every save. |
| `czlings run [NAME]` | Compile and run one exercise (name or unique substring, e.g. `c14`). |
| `czlings list` | All exercises with `done`/`todo` status. |
| `czlings hint [NAME]` | Print that exercise's hint. |
| `czlings reset [NAME]` | `git checkout` the exercise file (undo your edits). |
| `czlings solution [NAME]` | Copy the reference solution over the exercise. |
| `czlings verify` | Build and run **every** reference solution. |
| `czlings check` | Validate `info.toml`, file layout, and marker lines. |

For the memory chapters, build with sanitizers:

```sh
CZ_SAN=1 czlings run c22_linked_list
```

This adds `-fsanitize=address,undefined`, reporting leaks, use-after-free and
out-of-bounds accesses at the point they happen. A clean run is part of the
exercise.

## How an exercise works

```
exercises/02_c_advanced/c21_dynamic_string.c   <- you edit this
solutions/02_c_advanced/c21_dynamic_string.c   <- reference answer
harness/cztest.h                               <- CZ_CHECK* macros
manifest.d/20_c_advanced.toml                  <- order, mode, hint
```

* `mode = "test"`: compile and execute; exit status 0 means pass. Tests are
  ordinary `main()` code using `CZ_CHECK`, `CZ_CHECK_INT`, `CZ_CHECK_STR`,
  `CZ_CHECK_NEAR`, `CZ_CHECK_MEM`.
* `mode = "compile"`: the file only has to compile (used where the build itself
  is the lesson, e.g. the preprocessor or first syntax exercise).
* An exercise is *complete* when it passes **and** its file no longer contains
  `// I AM NOT DONE`.

`czlings reset` needs a git repository with at least one commit, which is why
the tutorial ships with one.

## Curriculum

| Chapter | Directory | Exercises | Topics |
| --- | --- | ---: | --- |
| 0. First steps | `exercises/00_beginner` | 10 | printing, variables, arithmetic, `if`, loops, functions, arrays, strings, structs |
| 1. C basics | `exercises/01_c_basics` | 12 | types, `sizeof`, integer division/overflow, control flow, recursion, 2D arrays, C strings |
| 2. C core | `exercises/02_c_advanced` | 18 | pointers, `const`, structs, unions, enums, `malloc`, linked lists, files, preprocessor, bit operations, `qsort`, undefined behaviour |
| 3. C++ core | `exercises/03_cpp_core` | 18 | references, classes, constructors/destructors, RAII, rule of three/five, operator overloading, inheritance, polymorphism, exceptions, move semantics |
| 4. C++ advanced | `exercises/04_cpp_advanced` | 20 | templates, STL containers/algorithms, lambdas, iterators, smart pointers, `optional`/`variant`, data-structure and OOP capstones |

Mapping to BME VIK:

* **Programozás alapjai 1 (C)** — chapters 0–2. Everything a Prog1 ZH asks for:
  pointer arithmetic, string handling, structs, dynamic arrays, file I/O, bit
  manipulation, preprocessor.
* **Programozás alapjai 2 / Programozás (C++)** — chapters 3–4: class design,
  RAII, copy/move, inheritance and virtual dispatch, templates, STL.
* **Adatszerkezetek és algoritmusok** — linked list (`c22`, `cpp31`), BST
  (`cpp32`), dynamic array (`cpp30`), matrix (`cpp34`), sorting/searching
  (`c29`, `cpp24`).

## Documentation

* [docs/00-start-here.md](docs/00-start-here.md) — zero-experience setup,
  terminal basics, how to read compiler errors, study plan.
* [docs/ts-to-c-cpp.md](docs/ts-to-c-cpp.md) — optional: TypeScript/JavaScript
  mental model mapped onto C/C++ (value vs reference semantics, no GC, UB,
  templates vs erased generics).

## Adding exercises / exams

Drop the skeleton in `exercises/<chapter>/<name>.c[pp]`, a full working copy in
`solutions/<chapter>/<name>.c[pp]`, and a fragment in `manifest.d/`:

```toml
[[exercises]]
name = "c41_exam_pointers"
path = "exercises/02_c_advanced/c41_exam_pointers.c"
mode = "test"
hint = "Dereference before you compare; the array decays to a pointer."
```

Then `make manifest && czlings verify`. Exam sheets become exercises the same
way: one exercise per task, with the tests encoding the specification.

## Rules for content

* No stdin, argv, clocks, randomness (a fixed seed is fine), threads or network.
* Tests are deterministic and independent of machine endianness/word size beyond
  what the exercise teaches.
* Skeleton files stay small and teach exactly one idea; hints are one concrete
  step, not the answer.
* Comments explain C/C++ semantics on their own terms — no "in language X this
  would be…" framing.
