# Library Management System

A console-based C++ application for librarians and bookkeepers to manage a local book inventory. The program stores book records in `library_data.txt`, reloads them at startup, validates user input, and saves changes after inventory updates.

## Required features

The menu provides these six user-facing options:

1. Add a book with an ID, title, author, publication year, and copy count.
2. View all books in a formatted table.
3. Search for books by title or author.
4. Update one selected field of an existing book by its ID, with confirmation.
5. Remove a chosen number of copies or an entire book record after confirmation.
6. Save the inventory and exit.

## Current status

Member 3's components are present:

- `Book.h` and `Book.cpp` define and validate the shared `Book` record.
- `InputValidator.h` and `InputValidator.cpp` reject blank text, invalid numbers, and out-of-range menu choices.
- `FileHandler.h` and `FileHandler.cpp` load and save book records.
- `Member3Tests.cpp` tests validation, missing files, malformed records, duplicate IDs, and file round trips.
- `LibraryLogic.h` and `LibraryLogic.cpp` provide Member 4's `addBook` operation: unique IDs, required text, valid years, and positive copy counts. ID duplicate checks ignore letter case, leading/trailing whitespace, and repeated internal whitespace, including for books loaded at startup. A duplicate ID prompts for a replacement value. Titles, authors, publication years, and copy counts may repeat across books.
- `viewAllBooks` displays all five book fields in a table with expanding text columns and a record count, or an empty-inventory message when there are no books.
- `LibraryLogic.cpp` also contains Member 2's search, update, and remove operations, moved from the main source file during integration improvements. Update and Remove return immediately for an empty inventory. Update changes one selected field; Remove supports a quantity or the entire record, with confirmation and cancellation.
- `Library_System_Eye-Git.cpp` routes all six menu choices, loads records at startup, and saves after successful changes and on Exit.
- `tests/test_book_operations.py` runs 17 automated integration tests with additional subcases using temporary inventories. It covers field updates, copy removal, cancellation, validation, closed input, and existing add/view/search behavior.

All six menu options are connected. Borrowed/available status tracking is outside the agreed scope. Author names accept punctuation and spaces; blank names are rejected.

Actions open directly without a Continue/Cancel screen. Add, Search, Update, and Remove show `(press 0 to cancel)` beneath their headings. Enter `0` at a text, number, submenu, or confirmation prompt to discard the unfinished action and return to the main menu. The exact text `0` is reserved for cancellation and cannot be stored as a field value. View displays the inventory and returns immediately. Exit offers `1. Save and exit` and `0. Cancel`. Earlier completed changes are preserved.

Screen headings, book detail panels, and Update/Remove submenus use the main menu's `=` and `||` border style through `ConsoleUI.h`. Panels expand for longer text. After this presentation change, all 13 integration tests passed again; a console preview also confirmed aligned borders with a long title.

## Book record

Each `Book` stores these fields:

| Field | Type | Rule |
| --- | --- | --- |
| `id` | `std::string` | Required and unique |
| `title` | `std::string` | Required |
| `author` | `std::string` | Required |
| `publicationYear` | `int` | From 1450 through 2026 |
| `copies` | `int` | Greater than zero |

## Data storage

The application uses `library_data.txt` in the current working directory. Each line contains quoted text fields followed by the publication year and copy count.

```text
"978-0-7432-7356-5" "The Great Gatsby" "F. Scott Fitzgerald" 1925 4
```

The loader handles these cases:

- A missing or empty file starts with an empty inventory.
- A malformed record is skipped with a line-number warning.
- A duplicate ID is skipped after the first valid record.
- An invalid year or copy count is skipped.
- A read failure leaves the current in-memory inventory unchanged.

The saver writes to `library_data.txt.tmp` before replacing the main data file. A failed write does not truncate the last saved inventory.

## Requirements

- A C++17-compatible compiler
- `g++` or `clang++`
- A terminal or the Visual Studio Code integrated terminal

## Build and test Member 3's modules

The Member 3 test program does not depend on the `LibraryLogic` module.

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic \
  Book.cpp InputValidator.cpp FileHandler.cpp Member3Tests.cpp \
  -o member3-tests

./member3-tests
```

The tests print warnings for deliberately invalid records. A successful run exits without an assertion failure.

## Build the current menu application

In PowerShell, compile and run all application modules:

```powershell
g++ -std=c++17 -Wall -Wextra -Wpedantic Library_System_Eye-Git.cpp Book.cpp InputValidator.cpp FileHandler.cpp LibraryLogic.cpp -o library.exe
.\library.exe
```

## Build the complete application

Compile all application modules with:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic \
  Library_System_Eye-Git.cpp Book.cpp InputValidator.cpp \
  FileHandler.cpp LibraryLogic.cpp -o library-system
```

Run it with:

```bash
./library-system
```

On Windows with MinGW, use `library-system.exe` as the output name and run `library-system.exe`.

## Example session

```text
=== Library Management System ===
1. Add Books
2. View Books
3. Search Books
4. Update Book Details
5. Remove Book
6. Exit
Choose an option: letters
Enter a whole number from 1 to 6.
Choose an option: 6
=================================
||            Exit             ||
=================================
|| 1. Save and exit             ||
|| 0. Cancel                    ||
=================================
Choose an option: 1
Data saved. Exiting program...
```

## Module responsibilities

| Module | Owner | Responsibility |
| --- | --- | --- |
| `Library_System_Eye-Git.cpp` | Member 1 | Menu loop and feature routing |
| `LibraryLogic.cpp` add and view functions | Member 4 | Add and display book records |
| `LibraryLogic.cpp` search, update, and remove functions | Member 2 | Search and modify inventory records |
| `Book.*` | Member 3 | Shared record structure and record validation |
| `FileHandler.*` | Member 3 | Persistent file loading and saving |
| `InputValidator.*` | Member 3 | Text and numeric input validation |
| `Member3Tests.cpp` | Member 3 | Validation and persistence tests |

## Suggested remaining checks

Before the final submission, the team must:

1. Review the integrated operations and repeat the tests after further changes.
2. Compile and run the complete application.
3. Test valid input, invalid input, an empty inventory, duplicate IDs, and missing IDs.
4. Run `clang-format` on all C++ files.
5. Run `clang-tidy` or Cppcheck if either tool is available.
6. Record expected and actual results for at least five test cases.
7. Complete the AI interaction log and group contribution table.

## Git workflow

Create one feature branch per member. Commit one reviewable change at a time, push the branch, and open a pull request into `main`. Another teammate must review the pull request before it is merged.

## Automated book operation tests

Python 3 is needed only for this test runner, not for running the C++ application. Build a fresh executable first, then run:

```powershell
python tests/test_book_operations.py .\library.exe
```

The runner uses temporary directories and does not modify your working `library_data.txt`. Close any running application/debug session before rebuilding `library.exe`; Windows prevents replacing an executable that is in use.

## 5. Before / After Code Refactoring

Required by Sections 7 and 10: at least three specific code improvements. Implemented and tested on November 13, 2026, on `feature/book-operation-improvements`, starting from commit `7df62bd`. Improvements 1-3 change validation or functionality; Improvement 4 is structural refactoring. Original book operations were contributed by Members 2 and 4; these changes integrate and improve them.

### Improvement 1: Empty Inventory Checks for Update and Remove

- **Before:** Both operations requested an ID even when no book records existed, then reported that the ID was not found.

```cpp
const string id = getRequiredText("Enter book ID: ");
```

- **After:** Both operations check the inventory before requesting an ID.

```cpp
if (books.empty()) {
    cout << "The library inventory is empty.\n";
    return false;
}
```

- **Reason:** Avoids unnecessary input and clearly explains why the operation cannot proceed.
- **Verification:** With an empty inventory, selecting Update and Remove produced two empty-inventory messages and no ID prompt.

### Improvement 2: Selective Book Detail Updates

- **Before:** Updating one field required re-entering all four editable values.

```cpp
updatedBook.title = getRequiredText("Enter new title: ");
updatedBook.author = getRequiredText("Enter new author: ");
updatedBook.publicationYear = getValidInteger(
    "Enter new publication year: ", 1450, 2026);
updatedBook.copies = getValidInteger(
    "Enter new number of copies: ", 1, numeric_limits<int>::max());
```

- **After:** A validated submenu selects Title, Author, Publication Year, Copies, or Cancel. A copy of the original record is changed and assigned back only after confirmation. For example, the title branch is:

```cpp
const int field = getValidMenuChoice(1, 5);
// Inside switch (field):
case 1:
    updatedBook.title = getRequiredText("Enter new title: ");
    break;
```

- **Reason:** Reduces repeated input and prevents accidental changes to unrelated fields.
- **Verification:** Separate tests changed each field and compared the complete saved record, confirming that the other fields stayed unchanged. Cancel, declined confirmation, blank text, invalid numbers, and both year boundaries were checked.

### Improvement 3: Partial Copy Removal

- **Before:** Confirmation always removed the entire record, regardless of its copy count.

```cpp
books.erase(iterator);
```

- **After:** Users choose a quantity, the entire record, or Cancel. Quantities must be between 1 and the current copy count. After confirmation, the operation either reduces copies or deletes the record when no copies would remain.

```cpp
if (quantity == iterator->copies) {
    books.erase(iterator);
    cout << "Book removed successfully.\n";
} else {
    iterator->copies -= quantity;
    cout << "Copies removed successfully. Remaining copies: " << iterator->copies << '\n';
}
```

- **Reason:** Supports inventory adjustments without deleting and recreating records. The zero-copy boundary cannot leave an invalid book record.
- **Verification:** Removing 2 of 5 copies saved 3 copies and survived a restart. Removing all 5 copies, or choosing entire-record removal, deleted the record. Invalid quantities, cancellation, and declined confirmation preserved inventory correctly.

### Improvement 4: Separation of Book Operations from the Main Module

- **Before:** `Library_System_Eye-Git.cpp` contained `searchBook`, `updateBook`, `removeBook`, and their helpers alongside `main()`. Add and View were already in `LibraryLogic.cpp`.
- **After:** All five book operations and their private helpers are implemented in `LibraryLogic.cpp`, with declarations in `LibraryLogic.h`. The main module handles menu routing and storage orchestration.
- **Reason:** Keeps business logic together and gives the main module one clear responsibility. Moving functions does not itself require a change to their public signatures.
- **Verification:** All modules compiled together with C++17, `-Wall`, `-Wextra`, and `-Wpedantic` without compiler warnings. Regression tests verified Add, View, case-insensitive Search, and persistence alongside the intentionally changed Update and Remove workflows.

### Improvement 5: Cancellation During Book Operations

- **Before:** Add and Search had no way to cancel after selecting the operation. Update and Remove offered cancellation only at specific stages, so users could not consistently return to the main menu while entering details.
- **After:** Added `(press 0 to cancel)` beneath the Add, Search, Update, and Remove headings. Entering `0` at a text, numeric, submenu, or confirmation prompt cancels the unfinished operation and returns to the main menu without applying partial changes. Exit also offers `0. Cancel`. Shared input validators detect cancellation, and the main loop handles it through `InputCancelled`.
- **Reason:** Gives users a consistent way to leave an operation, avoids forcing them to finish unwanted input, and centralizes cancellation handling. This is a usability and control-flow improvement rather than purely structural refactoring.
- **Verification:** Automated tests covered cancellation at every Add input stage, during Search/Update/Remove, and at Exit. Saved records remained unchanged after cancellation; all 17 integration test methods passed.

### Additional Correction: Closed Input During Confirmation

- **Before:** `askConfirmation()` retried indefinitely if input closed, because the failed stream could never produce Y or N.
- **After:** Confirmation checks `if (!cin)` and returns false. Update and Remove also check for closed input before changing inventory.
- **Reason:** Prevents an endless confirmation loop and avoids applying incomplete edits.
- **Verification:** Eight input-truncation subcases completed within the test runner's five-second timeout; the saved record stayed unchanged in every case. These are automated regression checks, not debugger evidence.

The bug was subsequently reproduced and investigated with GDB in the isolated [`debug/`](debug/README.md) folder. That folder contains original/fixed reproductions, actual breakpoint and variable logs, exact retest output, and instructions for capturing VS Code screenshots. The debugger showed repeated EOF/failure states in the original loop and a normal return through the corrected guard.

### Recorded Test Results

Tests ran against a fresh temporary executable so the running local `library.exe` did not need to be overwritten. Each test used a separate temporary inventory. Unless specified otherwise, the initial record was ID `B1`, title `The Hobbit`, author `J. R. R. Tolkien`, year `1937`, copies `5`.

| Case and input | Expected result | Actual result |
| --- | --- | --- |
| Empty inventory: Update, Remove, Exit | Two empty-inventory messages; no ID prompt; empty saved file | Two `The library inventory is empty.` messages; no `Enter book ID:`; saved file empty |
| Update Title to `Revised Title`; confirm Y | Only title changes | Saved title `Revised Title`; ID, author, year, copies unchanged |
| Update Author to `O'Connor`; confirm Y | Punctuation accepted; only author changes | Saved author `O'Connor`; other fields unchanged |
| Update Year to `1450`; confirm Y | Lower year boundary accepted | Saved year `1450`; other fields unchanged |
| Update Copies to `1`; confirm Y | Positive lower boundary accepted | Saved copies `1`; other fields unchanged |
| Update submenu: `letters`, `6`, then `3`; years `1449`, `2027`, `2026` | Invalid submenu/year input rejected; final year 2026 accepted | Range messages shown; saved year `2026` |
| Update Title with spaces then `The Hobbit`; Update Copies with `letters`, `-2`, `-1`, then `5` | Blank/invalid values rejected; valid replacements accepted | Blank/range messages shown; original record preserved |
| Update Cancel, or decline a proposed title change | Original record retained | `Update cancelled.`; complete saved record unchanged |
| Remove 2 copies from 5; confirm Y; restart and View | Record retained with 3 copies | `Remaining copies: 3`; saved copies `3`; restarted View displayed `The Hobbit` and 1 record |
| Remove quantity: `letters`, `-2`, `-1`, `6`, then `1`; confirm Yes | Four invalid quantities rejected; 4 copies remain | Four `Enter a whole number from 1 to 5.` messages; saved copies `4` |
| Remove quantity 5 of 5, or choose entire record; confirm Y | Record deleted; no zero-copy record | Entire-record warning and `Book removed successfully.`; saved file empty |
| Remove Cancel, decline quantity removal, or decline entire record removal | No inventory change | `Book removal cancelled.` in all three subcases; complete saved record unchanged |
| Update and Remove ID `missing` | Two missing-ID messages; no changes | Two `Book ID not found.` messages; complete saved record unchanged |
| Removal confirmation `maybe`, then N | Re-prompt, then cancel | `Please enter Y or N.` followed by cancellation; complete saved record unchanged |
| Close input at eight Update/Remove prompts | Exit without hanging or saving partial changes | All eight subcases exited within timeout; original saved record unchanged |
| Add duplicate ID `b1`, replace with `B2`, author `O'Connor`; Search `hOBb`; View | Duplicate rejected; valid book added; original title found; 2 records | Duplicate-ID message; B2 saved; Search displayed `The Hobbit`; View reported 2 records |

Automated result: **13 test methods passed**, including the subcases above. Existing `Member3Tests.cpp` also completed with exit code 0; its three skipped-record warnings were expected for deliberately malformed or duplicate input. `clang-format`, `clang-tidy`, and Cppcheck were not found on PATH during this check; no formatter or static-analyzer evidence is claimed here. The separate GDB investigation is documented in `debug/README.md`.

### Cancellation Follow-up

The simplified `0` cancellation flow passed all **17 integration test methods**, including cancellation at all five Add prompts, ten Search/Update/Remove input stages, whitespace-padded cancellation, and cancelling Exit. Tests confirmed that actions open directly without the old Continue screen or `/cancel` instruction. Saved records remained unchanged when an unfinished action was cancelled. Tests used temporary data only.
