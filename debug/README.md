# Debugging Evidence: Closed Input in Confirmation

Investigated with GNU GDB 15.2 on November 13, 2026. This folder reproduces a real earlier bug after its correction; it does not claim the bug was first discovered in this debugger session. No library inventory is loaded or modified by these programs.

## 6. Debugger Evidence (GDB / VS Code Debugger)

*(Required by Section 7 & 11: Document at least one real bug investigated with a debugger)*

- **Incorrect Behavior:** When input closed at the Y/N confirmation, the original `askConfirmation()` repeatedly requested an answer instead of cancelling. We reproduced this earlier bug in `debug/confirmation_bug.cpp` using the empty `eof_input.txt` file. GDB reached the same breakpoint three times without new input; we deliberately stopped the buggy process after the third iteration.
- **Breakpoint Location:** Line 30 inside `askConfirmation()` in `debug/confirmation_bug.cpp`, immediately before checking whether the answer is Y/YES. For the corrected version, line 30 in `debug/confirmation_fixed.cpp` stops at the added `if (!cin)` guard.
- **Inspected Variable Values:** On the first buggy iteration, `attempt = 1`, `answer = ""`, `inputEof = true`, `inputFailed = true`, and `inputBad = false`. On subsequent breakpoint hits, `attempt` increased to 2 and then 3 while EOF and failure stayed true. These diagnostic Boolean variables capture `cin.eof()`, `cin.fail()`, and `cin.bad()`.
- **Root Cause:** `getRequiredText()` returned an empty string when input ended. The original confirmation loop checked only for Y/YES or N/NO, so the empty response triggered another iteration. The failed input stream could not supply another answer, causing the loop to continue indefinitely.
- **Code Correction:** Added the following guard immediately after reading the confirmation response. It returns false when input closes, cancelling the operation. The same correction is present in the working application's `LibraryLogic.cpp`.

  ```cpp
  if (!cin) {
      return false;
  }
  ```

- **Retest Result:** Recompiled the corrected reproduction and repeated the empty-file input under GDB. The debugger entered the guard, returned false on the first iteration, and the process printed `Confirmation result: cancelled` before exiting normally. Separate retests with Y and N printed `Confirmation result: accepted` and `Confirmation result: cancelled`, respectively. All three retests exited with code 0 within the five-second timeout. Actual evidence is in `bug-gdb-log.txt`, `fixed-gdb-log.txt`, and `retest-log.txt`.
- **Screenshot:** Screenshots were captured manually in VS Code and reviewed in the group report. They show buggy attempts 1 and 2 with failed stream flags, the corrected guard, and the terminal cancellation result. The three-iteration trace is recorded separately in `bug-gdb-log.txt`. Screenshot images are in the report rather than this folder; the instructions below allow the team to reproduce them.

## Original behavior and reproduction

The earlier `askConfirmation()` in commit `7df62bd` checked only for Y/YES or N/NO. When input closed, `getRequiredText()` returned an empty string, and the confirmation loop kept retrying the failed input stream.

`confirmation_bug.cpp` isolates that original loop and its uppercase helper. It links the project's actual `InputValidator.cpp`. The attempt counter and stream-state variables are diagnostic additions so they can be inspected reliably in GDB. A file is connected to `cin` for deterministic input. The empty `eof_input.txt` reproduces end-of-input without needing a terminal-specific keyboard shortcut. This is a focused reproduction, not the entire original application.

## Breakpoint and actual inspected values

- Bug breakpoint: `confirmation_bug.cpp:30`, immediately after reading input and capturing its state.
- Fixed breakpoint: `confirmation_fixed.cpp:30`, on the added `if (!cin)` guard.
- Inspected variables: `attempt`, `answer`, `inputEof`, `inputFailed`, `inputBad`.
- GDB backtraces show the call from `main()` into `askConfirmation()`.

| State | Bug iteration 1 | Bug iteration 2 | Bug iteration 3 | Fixed iteration 1 |
| --- | --- | --- | --- | --- |
| `attempt` | 1 | 2 | 3 | 1 |
| `answer` | empty string | empty string | not printed | empty string |
| `inputEof` (`cin.eof()`) | true | true | true | true |
| `inputFailed` (`cin.fail()`) | true | true | true | true |
| `inputBad` (`cin.bad()`) | false | not printed | not printed | false |
| Outcome | loop continues | loop continues | deliberately stopped by GDB | returns false and exits normally |

Actual raw evidence is saved in `bug-gdb-log.txt` and `fixed-gdb-log.txt`. The buggy process was deliberately killed after the third breakpoint to avoid an endless run; it did not recover by itself. The fixed log records stepping through the guard and `return false`, followed by a normal process exit.

## Root cause and correction

The confirmation loop did not check stream failure. An empty answer matched neither a positive nor a negative response, so the loop retried even though EOF made further input impossible. Clearing the stream would not provide new data; the action should cancel instead.

Before:

```cpp
string answer = toUpper(getRequiredText(prompt));
if (answer == "Y" || answer == "YES") {
    return true;
}
```

After:

```cpp
string answer = toUpper(getRequiredText(prompt));
if (!cin) {
    return false;
}
if (answer == "Y" || answer == "YES") {
    return true;
}
```

The guard also exists in the working application's `LibraryLogic.cpp`. These reproduction programs only return a confirmation result. Application-level preservation of inventory is checked separately by `tests/test_book_operations.py`; it is not inferred from this small reproduction.

## Exact retest results

Both reproductions compiled using C++17, `-Wall -Wextra -Wpedantic -g -O0` without compiler warnings. The fixed version was additionally run with three input files, with a five-second timeout per run:

| Input | Expected result | Actual result |
| --- | --- | --- |
| Empty `eof_input.txt` | Cancel and exit normally | `Input ended unexpectedly.` then `Confirmation result: cancelled`; exit code 0 |
| `yes_input.txt`: Y | Accept and exit normally | `Confirmation result: accepted`; exit code 0 |
| `no_input.txt`: N | Cancel and exit normally | `Confirmation result: cancelled`; exit code 0 |

All three completed within the timeout. See `retest-log.txt` for actual output. Debugger logs are authentic text evidence. The user also captured VS Code screenshots of the original bug and corrected version for the group report.

## Capture VS Code screenshots for the report

1. Open this project folder in VS Code. Open `debug/confirmation_bug.cpp` and click the gutter at line 30 to set a breakpoint.
2. In Run and Debug, select **Debug confirmation bug (isolated)** and press F5. The pre-launch task builds only this reproduction and its validator dependency.
3. At the breakpoint, expand Locals or add `attempt`, `answer`, `inputEof`, `inputFailed`, and `inputBad` to Watch. Capture the editor, breakpoint, variable values, and call stack together.
4. Press Continue (F5) twice, stopping at the breakpoint each time. Capture the increasing attempt number with the failed stream still set. Then press Stop (Shift+F5); do not leave the buggy loop running without a breakpoint.
5. Open `debug/confirmation_fixed.cpp`, set a breakpoint at line 30, and select **Debug confirmation fix (isolated)**. Press F5 and capture the same values at the guard.
6. Step Over (F10) to see the guard enter `return false`. Continue and capture `Confirmation result: cancelled` and the normal process exit in the terminal/debug output.

Caption the screenshots as a reproduction of the earlier bug and verification of the existing correction. Every student should be able to explain the stream flags and why returning false is appropriate.

## Reproduce the command-line evidence

Run these commands from the repository root, with MinGW's `bin` directory on PATH:

```powershell
g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 debug/confirmation_bug.cpp InputValidator.cpp -o debug/confirmation-bug.exe
g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 debug/confirmation_fixed.cpp InputValidator.cpp -o debug/confirmation-fixed.exe
gdb --batch -x debug/bug.gdb debug/confirmation-bug.exe
gdb --batch -x debug/fixed.gdb debug/confirmation-fixed.exe
.\debug\confirmation-fixed.exe debug/eof_input.txt
.\debug\confirmation-fixed.exe debug/yes_input.txt
.\debug\confirmation-fixed.exe debug/no_input.txt
```

The GDB scripts are repeatable and stop the buggy run after three breakpoint hits. Compiled executables are ignored by the project's `*.exe` rule. Build the main application using its explicit source list; compiling every `.cpp` recursively would include these separate `main()` functions.
