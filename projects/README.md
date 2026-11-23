# Projects

Seventeen projects distributed after the chapters, then a BME-style final ZH.
Each project is one `czlings` exercise: the file contains the public API, the
tests that define the behaviour, and `TODO` stubs. **Write every implementation
from zero.** Only the type/class declarations, the minimal safe boilerplate
(constructors, accessors, the test `main`) and the tests are given; the
implementation bodies are yours.

```sh
czlings list                 # projects are mixed into the normal order
czlings run p00_temperature  # compile and run one project
czlings hint  p00_receipt    # one concrete step
```

Without installing the runner: `cargo run --release -- run p00_temperature`.
The reference implementations are under `solutions/projects/`; run
`czlings solution <name>` only after your own attempt.

## Toolchain: Windows, macOS, Linux

The project code is standard C17/C++17: no Windows API, no POSIX-only calls, and
no OS-specific paths. `tmpfile()`, `qsort`, `std::variant` and the rest behave
the same on all three systems. The runner itself expects a gcc-style compiler.

| System | Setup |
| --- | --- |
| macOS | `xcode-select --install`; Apple clang provides `cc` and `c++`. |
| Linux (Debian/Ubuntu) | `sudo apt install build-essential`; `cc` and `c++` are symlinks to gcc/g++. |
| Linux (Fedora) | `sudo dnf groupinstall "Development Tools"`. |
| Linux (Arch) | `sudo pacman -S base-devel`. |
| Windows (recommended) | WSL 2 + Ubuntu, then use the Linux setup. Clone in the Linux home, not in `C:\`. |
| Windows (native) | MSYS2/MinGW-w64 UCRT. Add `mingw64\bin` to `PATH`, then set `CC=gcc` and `CXX=g++` (PowerShell: `$env:CC="gcc"; $env:CXX="g++"`). |

On Windows, `make manifest` may be unavailable. In Git Bash/WSL use
`cat manifest.d/*.toml > info.toml`; in PowerShell use
`Get-Content manifest.d\*.toml | Set-Content info.toml`. Then run
`cargo run --release -- verify`.

## Order

| After chapter | Project | File | Focus |
| --- | --- | --- | --- |
| 0. First steps | `p00_temperature` | `projects/00_beginner/p00_temperature.c` | functions, `if`, loops, arrays, strings |
| 0. First steps | `p00_receipt` | `projects/00_beginner/p00_receipt.c` | structs, arrays, integer money |
| 1. C basics | `p01_gradebook` | `projects/01_c_basics/p01_gradebook.c` | arrays, functions, bounds, average |
| 1. C basics | `p01_tictactoe` | `projects/01_c_basics/p01_tictactoe.c` | 2D arrays, nested loops, control flow |
| 2. C core | `p02_log_analyzer` | `projects/02_c_advanced/p02_log_analyzer.c` | malloc/realloc, strings, qsort, bit masks, stdio |
| 2. C core | `p02_contact_book` | `projects/02_c_advanced/p02_contact_book.c` | linked list, ownership, unlink-and-free |
| 2. C core | `p02_c_matrix` | `projects/02_c_advanced/p02_c_matrix.c` | dynamic 2D matrix, add/multiply/transpose |
| 2. C core | `p02_c_bst` | `projects/02_c_advanced/p02_c_bst.c` | binary search tree, recursion, post-order free |
| 2. C core | `p02_c_state_machine` | `projects/02_c_advanced/p02_c_state_machine.c` | state machine, config parsing, quoted escapes |
| 2. C core | `p02_c_backtracking` | `projects/02_c_advanced/p02_c_backtracking.c` | n-queens, recursive path search |
| 2. C core | `p02_c_expression` | `projects/02_c_advanced/p02_c_expression.c` | recursive descent, grammar, error codes |
| 3. C++ core | `p03_bank` | `projects/03_cpp_core/p03_bank.cpp` | classes, virtual dispatch, RAII, exceptions, STL |
| 3. C++ core | `p03_matrix` | `projects/03_cpp_core/p03_matrix.cpp` | rule of five, operators, bounds checks |
| 3. C++ core | `p03_cpp_library` | `projects/03_cpp_core/p03_cpp_library.cpp` | multiple inheritance, `dynamic_cast`, persistence |
| 4. C++ advanced | `p04_word_index` | `projects/04_cpp_advanced/p04_word_index.cpp` | map/set, algorithms, lambdas, streams |
| 4. C++ advanced | `p04_catalog` | `projects/04_cpp_advanced/p04_catalog.cpp` | `std::variant`, `std::visit`, `optional` |
| 4. C++ advanced | `p04_cpp_pipeline` | `projects/04_cpp_advanced/p04_cpp_pipeline.cpp` | custom iterator, `std::function`, Observer |
| Final | ZH tasks | `projects/final_exam/` | timed exam: RLE, linked list, polynomial |

## How to work a project

1. Read the file header. It states the exact contract and the edge cases the
   tests check.
2. Implement one function, run the project, and fix the first failure.
3. Repeat until every check passes. Delete the `// I AM NOT DONE` line.
4. Only then compare with `solutions/projects/...`.

The test `main` is the specification. Do not edit it; if a check seems wrong,
re-read the header contract.

`czlings reset` restores a file with `git checkout`, so commit the project files
once before relying on it.

## Coverage

See [PROG1_PROG2_COVERAGE.md](PROG1_PROG2_COVERAGE.md) for a lecture-by-lecture
map of Prog1 and Prog2 topics to exercises and projects, plus the parts that are
course-specific (labs, NHF, official ZH wording).

## Final exam

`projects/final_exam/` holds the ZH: a theory sheet plus three timed coding
tasks. See [final_exam/README.md](final_exam/README.md) for the rules, points
and suggested order.
