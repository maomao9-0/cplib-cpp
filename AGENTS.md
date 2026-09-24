# cplib-cpp

A C++20 competitive-programming library intended to be included as a third-party
dependency. Prioritise correctness, speed, maintainability, and clear interfaces
over short code for manual transcription. Longer implementations are welcome
when they improve performance or clarity.

## Code and documentation

- Put reusable, self-contained headers in `library/`, using `#pragma once` and
  namespace `maomao90`. Follow nearby naming and formatting conventions.
- Document public APIs in their headers with Doxygen comments. Use
  `library/tree/static_top_tree.hpp` as the style reference: summary, detailed
  behaviour, then a separate `Runs in \f$O(...)\f$ time.` paragraph before tags.
- Use `@tparam`, `@param`, `@return`, `@pre`, `@throws`, `@note`, and `@see` where
  relevant. State indexing, edge cases, supported types, and numeric limits.
  Keep complexity out of summaries and precondition tags.
- `docs/` is deprecated; do not add documentation there. Do not hand-edit
  generated `html/` or `latex/` output.
- Make unsupported configurations explicit; avoid silent fallbacks that
  unexpectedly change performance guarantees.

## Verification

- Add drivers in `verify/yosupo/` for applicable Library Checker problems,
  using a `PROBLEM` URL. For functionality without a matching problem, add
  deterministic assertions and independent reference checks in
  `verify/unit_test/`, then solve the a+b problem.
- Run relevant drivers with `oj-verify run path/to/test.test.cpp` (pass files,
  not directories). Include existing regression tests when changing shared code.
- Keep tests reproducible and cover boundary cases and optimised code paths.
  Report which checks ran and any limitations; distinguish local official-test
  verification from remote judge submissions.
