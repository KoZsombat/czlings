# Final ZH — BME VIK style

This is the end-of-course exam. It is deliberately larger and less guided than
the projects: two C tasks, one C++ task, and a theory sheet. Work under exam
conditions.

## Rules

- **Time:** 120 minutes.
- **Points:** 50 total. Pass mark: 25 points (50%).
- **Closed book**, except one handwritten A4 cheat sheet.
- No internet, AI tools, or communication. Use only the C17/C++17 standard
  library and the compiler.
- Run one task at a time: `czlings run zh1_rle`, `czlings run zh2_list`,
  `czlings run zh3_polynomial`.
- Do not edit the test `main`. Delete `// I AM NOT DONE` only after all checks
  pass.
- This branch does not contain reference solutions. They live on the `work`
  branch under `solutions/projects/final_exam/`; `czlings solution` there
  reveals one.

## Structure

| Part | Points | What it checks |
| --- | ---: | --- |
| Theory (`theory.md`) | 10 | C/C++ semantics, ownership, UB, STL |
| Task 1 (`zh1_rle.c`) | 10 | pointer arithmetic, C strings, capacity checks |
| Task 2 (`zh2_list.c`) | 15 | linked list, `Node **`, unlink-and-free, reverse |
| Task 3 (`zh3_polynomial.cpp`) | 15 | operator overloading, const correctness, `std::vector` |

## Suggested order

1. Theory, 15 minutes: answer everything you know, mark the rest.
2. Task 2, 30 minutes: sorted insert, duplicate removal, reverse.
3. Task 3, 30 minutes: `+=`, `*`, `operator()`, `operator<<`.
4. Task 1, 25 minutes: one run at a time, check `cap` before every write.
5. Review, 20 minutes: rerun all three tasks and recheck the theory answers.

## Grading (BME scale)

| Points | Grade |
| ---: | --- |
| 0–24 | 1 (fail) |
| 25–29 | 2 (pass) |
| 30–34 | 3 (satisfactory) |
| 35–39 | 4 (good) |
| 40–50 | 5 (excellent) |

## After the exam

- Rerun each task with `CZ_SAN=1 czlings run zh2_list` on Linux, macOS, or WSL
  (the runner uses gcc-style flags) to check for leaks and use-after-free.
- Compare with `solutions/projects/final_exam/` on the `work` branch.
- Re-read the theory explanations in [theory.md](theory.md).
