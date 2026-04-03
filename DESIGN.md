# ft_ls Design Page

## Goal
Recreate the system `ls` behavior with support for:
- `-l`
- `-R`
- `-a`
- `-r`
- `-t`

Constraints from the subject:
- Output should be as identical as possible to system `ls`.
- No multi-column format is required when `-l` is not used.
- No ACL / extended attribute support is required.
- For `-R`, recursion should be designed from the beginning to avoid architecture issues later.

## Current Architecture (What Exists)

### Entry Point
- `src/main.c`
  - Parses combined flags from argv.
  - Calls `ls_dir()` for each provided argument (or `.` by default).
  - Tracks number of path arguments via `flags.dir_nbr`.

### Data Model
- `src/structs.h`
  - `t_flags`: runtime options (`l`, `R`, `a`, `r`, `t`) + `dir_nbr`.
  - `t_entry`: entry name + `struct stat`.
  - `t_entry_data`: dynamic array of entries + count.

### Parsing / Collection
- `src/parser.c`
  - `parse_flags()` supports `l/a/R/r/t`.
  - `store_entries()` opens a directory, reads entries with `readdir`, and stores `stat` + name.

### Sorting
- `src/sorter.c`
  - Merge sort implementation.
  - Supports default name sort, `-r`, `-t`, and combined `-tr`.

### Output
- `src/printer.c`
  - Prints names in one-line flow (not column mode).
  - Honors hidden filtering via `-a`.
  - `-l` hook exists (`print_l_flag`) but is not implemented yet.

### Recursion
- `src/ls.c`
  - `ls_dir()` collects, sorts, prints, frees.
  - `ls_subdir()` exists but is currently empty.

## Requirement Coverage Matrix

| Requirement | Status | Notes |
|---|---|---|
| `-a` | Partial | Hidden file filtering works in print stage; collection always includes hidden entries. |
| `-r` | Partial | Reverse ordering implemented in sorter, but parity with exact `ls` tie-break behavior needs validation. |
| `-t` | Partial | Time-based sorting implemented via `st_mtime`; full parity (nanosecond tie-breaks and exact fallback behavior) not confirmed. |
| `-l` | Not done | Long-format printing is still TODO. |
| `-R` | Not done | Recursion stub exists (`ls_subdir`) but no traversal yet. |
| Identical display | Partial | Basic names print; long format, error text parity, spacing/padding, and directory section formatting are incomplete. |
| Non-`-l` no columns | Done | Current output is linear text, not multi-column. |
| Ignore ACL/xattr | Done by scope | No ACL/xattr output logic in main flow (subject-compliant). |

## Major Gaps To Close

1. Implement correct `-l` formatting:
- File type + permissions string.
- Link count.
- Owner/group name resolution.
- Size.
- Date format matching `ls` behavior.
- Symlink target display (`name -> target`) when applicable.
- `total` line for directory long listing.

2. Implement robust `-R` traversal:
- Recurse into subdirectories while skipping `.` and `..`.
- Preserve ordering rules under `-t/-r` before recursive descent.
- Print directory headers exactly where expected.
- Avoid recursion loops and path-concatenation bugs.

3. Handle file arguments vs directory arguments correctly:
- `ls file dir` should print file entries first (sorted), then directory blocks.
- Per-path error handling for inaccessible or missing targets.

4. Tighten output parity with system `ls`:
- Error messages format (`ft_ls: ...`).
- Newline spacing between multiple directory sections.
- Stable tie-break behavior in sort.

5. Build integration cleanup:
- `src/ls.c` currently exists but is not part of `Makefile` `SRC` list.
- Remove debug prints from runtime output (e.g., sort debug messages, flag echo in `main`).

## Implementation Plan (Recommended Order)

### Phase 1: Make Core Flow Correct
- Add `src/ls.c` to build.
- Remove debug prints that break expected output.
- Introduce path classification (file vs dir vs invalid) before listing.

### Phase 2: Implement `-l`
- Build permission rendering helper.
- Add uid/gid display via `getpwuid` / `getgrgid` with numeric fallback.
- Add timestamp formatter compatible with `ls` style.
- Add symlink handling (`lstat` + `readlink`) and `total` line.

### Phase 3: Implement `-R`
- Implement recursive descent with skip rules (`.`/`..`).
- Ensure output layout and headers match expected format across multi-dir input.

### Phase 4: Match Edge Cases
- Sorting tie-break verification against system `ls`.
- Hidden entries + long format interactions.
- Error behavior and permission-denied scenarios.

### Phase 5: Validation
- Create a small test matrix against `/bin/ls`:
  - Flags: none, `-a`, `-l`, `-t`, `-r`, combos (`-la`, `-ltr`, `-R`, `-Ral`).
  - Targets: regular files, dirs, symlinks, hidden, missing path, unreadable dir.

## Suggested Acceptance Criteria
- For all required flags and combinations, output includes all expected information with no missing fields.
- Ordering and recursive traversal are consistent with system `ls` for tested cases.
- No extra debug output is printed.
- Program handles invalid paths and permission errors without crashing.

## Current Risk Notes
- Dynamic array growth and path joins in `store_entries()` should be reviewed for memory safety and leaks.
- Recursion and formatting are the two biggest parity risks for evaluation.
