# czlings

A rustlings-style, exercise-driven tutorial for **C17 and C++17**, aimed at the
material covered in BME VIK computer-engineering courses (*Programozás alapjai
1/2*, *Adatszerkezetek és algoritmusok*, and the C/C++ parts of later systems
courses).

You get small broken programs. You fix them until the tests pass, delete the
`// I AM NOT DONE` line, and the watcher moves you to the next one. Reference
solutions and hints are included.

> **Coming from TypeScript?** Read [docs/ts-to-c-cpp.md](docs/ts-to-c-cpp.md)
> first. It maps the mental model you already have (values vs. references, the
> heap, closures, `undefined`) onto pointers, objects, RAII and undefined
> behaviour. Every exercise also carries a `// Coming from TypeScript:` note
> where the two languages disagree.

## Requirements

- A C compiler (`cc`, gcc or clang) and a C++ compiler (`c++`, g++ or clang++)
  with C17 / C++17 support.
- Rust (stable) **only to build the `czlings` runner**. You never write Rust.

```sh
cargo install --path . --force   # installs the `czlings` binary
make manifest                    # builds info.toml from manifest.d/
czlings                          # start watching the first exercise
```

Alternatively run it without installing: `cargo run --release -- watch`.

## Commands

| Command | What it does |
| --- | --- |
| `czlings` / `czlings watch` | Clear the screen, run the next unfinished exercise, re-run on every save. |
| `czlings run [NAME]` | Compile and run one exercise (name or unique substring). |
| `czlings list` | All exercises with `done`/`todo` status. |
| `czlings hint [NAME]` | Print that exercise's hint. |
| `czlings reset [NAME]` | `git checkout` the exercise file (undo your damage). |
| `czlings solution [NAME]` | Copy the reference solution over the exercise. |
| `czlings verify` | Build and run **every** reference solution. |
| `czlings check` | Validate `info.toml`, file layout, and marker lines. |

Build with sanitizers for the memory chapters:

```sh
CZ_SAN=1 czlings run c21_dynamic_string
```

This adds `-fsanitize=address,undefined`, which reports leaks, use-after-free
and out-of-bounds accesses at the point they happen.

## How an exercise works

```
exercises/01_c_basics/c03_arithmetic.c   <- you edit this
solutions/01_c_basics/c03_arithmetic.c   <- reference answer
harness/cztest.h                         <- CZ_CHECK* macros
manifest.d/10_c_basics.toml              <- metadata + hint
```

* `mode = "test"`: the file is compiled and executed; exit status 0 means pass.
  Tests are ordinary `main()` code using `CZ_CHECK`, `CZ_CHECK_INT`,
  `CZ_CHECK_STR`, `CZ_CHECK_NEAR`, `CZ_CHECK_MEM`.
* `mode = "compile"`: the file only has to compile (used for topics whose
  observable behaviour is the build itself, e.g. the preprocessor).
* An exercise is *complete* when it passes **and** its file no longer contains
  `// I AM NOT DONE`.

`czlings reset` needs a git repository with at least one commit, which is why
the tutorial ships with one.

## Curriculum

| Chapter | Directory | Exercises | Topics |
| --- | --- | ---: | --- |
| 1. C basics | `exercises/01_c_basics` | 12 | types, operators, control flow, functions, recursion, arrays, strings |
| 2. C core | `exercises/02_c_advanced` | 18 | pointers, `const`, structs, unions, enums, `malloc`, linked lists, files, preprocessor, bit operations, `qsort`, undefined behaviour |
| 3. C++ core | `exercises/03_cpp_core` | 18 | references, classes, constructors/destructors, RAII, rule of three/five, operator overloading, inheritance, polymorphism, exceptions, move semantics |
| 4. C++ advanced | `exercises/04_cpp_advanced` | 20 | templates, STL containers/algorithms, lambdas, iterators, smart pointers, `optional`/`variant`, data-structure and OOP capstones |

Mapping to BME VIK:

* **Programozás alapjai 1 (C)** — chapters 1–2. Everything a Prog1 ZH asks for:
  pointer arithmetic, string handling, structs, dynamic arrays, file I/O, bit
  manipulation, preprocessor.
* **Programozás alapjai 2 / Programozás (C++)** — chapters 3–4: class design,
  RAII, copy/move, inheritance and virtual dispatch, templates, STL.
* **Adatszerkezetek és algoritmusok** — the linked-list, BST, dynamic-array and
  hash/sort exercises in chapters 2–4.

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

Then `make manifest && czlings verify`. Exam sheets can be turned into exercises
this way: one exercise per exam task, tests encode the specification.

## Rules for content

* No stdin, argv, clocks, randomness (a fixed seed is fine), threads or network.
* Tests must be deterministic and independent of machine endianness/word size
  beyond what the exercise teaches.
* Skeleton files stay under ~70 lines and teach exactly one idea.
