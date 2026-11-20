# Start here: from zero to your first C program

No programming experience is assumed. If you can install an app and type
commands in a terminal, you can do this. The first chapter asks you to fix ten
tiny programs; by the end you will have written variables, conditions, loops,
functions, arrays and a struct.

Budget 30–60 minutes per exercise at the start. Typing the code yourself
matters; copying it does not teach your fingers or your brain.

## 1. What you are installing

* **A C compiler** turns a `.c` text file into a program the computer runs.
* **A C++ compiler** does the same for `.cpp` files. It usually ships with the C
  compiler.
* **Git** downloads the exercise repository and lets `czlings reset` restore a
  file you broke.
* **Rust** is only needed to build the `czlings` runner. You will not write any
  Rust. It is one command to install and then you can forget about it.

You also need a text editor. [VS Code](https://code.visualstudio.com/) is a good
default; install the C/C++ extension if you want syntax highlighting.

## 2. Install the tools

### macOS

```sh
xcode-select --install          # C and C++ compilers, git
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
```

Close and reopen the terminal after the Rust installer, then check:

```sh
cc --version
cargo --version
```

### Linux (Debian / Ubuntu)

```sh
sudo apt update
sudo apt install build-essential git curl
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
```

### Windows

Use **WSL 2** with Ubuntu: open PowerShell as administrator and run
`wsl --install`, reboot, create a Linux user, then follow the Linux steps
*inside the Ubuntu terminal*. Do not work in `C:\`; clone the repository in your
Linux home directory so compilers and file watching behave normally.

Check that all three exist before continuing:

```sh
cc --version
c++ --version
git --version
cargo --version
```

If a command is "not found", the installer did not finish or the terminal was
not restarted. Fix that first; nothing below will work without it.

## 3. Get the exercises

```sh
git clone https://github.com/KoZsombat/czlings.git
cd czlings
cargo install --path . --force
make manifest
```

`make manifest` builds `info.toml`, the list of exercises the runner reads. If
`make` is unavailable, run `cat manifest.d/*.toml > info.toml` instead.

## 4. Terminal in one minute

The terminal starts in some folder. These three commands are enough:

| Command | Meaning |
| --- | --- |
| `pwd` | print the current folder |
| `ls` | list files here |
| `cd czlings` | enter the `czlings` folder (`cd ..` goes back up) |

You must be inside the repository folder for `czlings` to find the exercises.
The prompt usually shows the current folder; if not, `pwd`.

## 5. Your first exercise

```sh
czlings
```

The screen clears and shows something like:

```
b01_hello  exercises/00_beginner/b01_hello.c  [compile]

... compiler error ...
```

Open that file in your editor, fix the two missing semicolons, save the file.
`czlings` notices the save, recompiles, and prints `compiled successfully`.
Then delete the line

```c
// I AM NOT DONE
```

save again, and the runner moves to `b02`. That loop — read, edit, save, read
the result, delete the marker — is the whole tutorial.

## 6. Reading the output

There are two kinds of failure.

**Compiler error** — the program could not be built. The message has the form

```
exercises/00_beginner/b01_hello.c:14:5: error: expected ';' after expression
    printf("Hello, world!\n")
    ^
```

`file:line:column: error: what` plus a caret pointing at the spot. Fix the
first error, save, look again. Later errors are often consequences of the first
one.

**Failed checks** — the program built and ran, but a test did not hold:

```
  [FAIL] line 52: sum_to(5) == 10, expected 15
tests failed
```

The test tells you what it called and what it expected. Your job is to make the
function match the specification in the comment at the top of the file.

`czlings` shows only the current exercise. `czlings list` shows all of them and
which are done.

## 7. Commands you will use

| Command | What it does |
| --- | --- |
| `czlings` | watch mode: run the next unfinished exercise, rerun on save |
| `czlings run b03` | run one exercise by name or unique fragment |
| `czlings list` | all exercises and their status |
| `czlings hint b03` | one concrete hint, no full answer |
| `czlings reset b03` | restore the original file (undo your edits) |
| `czlings solution b03` | copy the reference solution over the file |
| `czlings verify` | build and run every reference solution |

Use `hint` before `solution`. Use `solution` only after you can explain why the
reference works, then `reset` and write it again from memory. Struggle is where
the learning happens; the hint is there to stop you from spinning.

## 8. The learning path

| Step | Folder | What it teaches |
| --- | --- | --- |
| 0 | `exercises/00_beginner` | printing, variables, arithmetic, `if`, loops, functions, arrays, strings, structs |
| 1 | `exercises/01_c_basics` | types, integer arithmetic, control flow, recursion, 2D arrays, C strings |
| 2 | `exercises/02_c_advanced` | pointers, `const`, structs and unions, `malloc`, linked lists, files, preprocessor, bit operations, undefined behaviour |
| 3 | `exercises/03_cpp_core` | references, classes, constructors/destructors, RAII, copy and move, operators, inheritance, virtual functions, exceptions |
| 4 | `exercises/04_cpp_advanced` | templates, STL, lambdas, iterators, smart pointers, data-structure capstones |

Do them in order. Chapter 2 is the gate: pointers and manual memory are what
separate "can write a script" from "understands C". Expect to be slow there.

## 9. Study habits that work

* Type the code; do not paste. Muscle memory is real.
* Change one thing at a time, then run.
* Predict the output before you run. Being wrong is the useful part.
* When a check fails, read the comment at the top of the file and the failing
  expression; the mismatch is the lesson.
* Break working code on purpose (swap `<` for `<=`, remove a `free`) and see
  what the compiler or the tests say.
* Keep a plain-text file of one-line lessons: "array indexes start at 0",
  "`sizeof` on a pointer is not the array size".
* After every exercise, say out loud what the function promises and why the
  fixed version keeps that promise.

## 10. When you are stuck

1. Re-read the task comment and the exact failing check.
2. Run `czlings hint <name>`.
3. Read the compiler error from the top; fix only the first one.
4. Search the standard library name you see (e.g. `snprintf`, `malloc`) in
   [cppreference](https://en.cppreference.com/w/).
5. Ask a person or an assistant with the exact command you ran, the exact
   output, and what you expected. "It doesn't work" cannot be answered; a
   compiler error can.

If an exercise file is so broken that you want a clean slate,
`czlings reset <name>` restores it from git.

## 11. After chapter 0

You will know how to declare variables, branch, loop, call functions, index
arrays, walk a string and group data in a struct. Chapter 1 turns that into
fluency with C's type system; chapter 2 explains the memory model that
TypeScript, Python and Java hide from you.

Optional background:

* [docs/ts-to-c-cpp.md](ts-to-c-cpp.md) — if you already know TypeScript or
  JavaScript, this maps what you know onto pointers, ownership and undefined
  behaviour.
* The repository README lists the BME VIK course mapping and how to add your
  own exercises from exam sheets.
