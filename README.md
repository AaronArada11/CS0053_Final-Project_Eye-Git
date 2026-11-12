# Library Management System

A console-based C++ application for librarians and bookkeepers to manage a local book inventory. The program stores book records in `library_data.txt`, reloads them at startup, validates user input, and saves changes after inventory updates.

## Required features

The menu provides these six user-facing options:

1. Add a book with an ID, title, author, publication year, and copy count.
2. View all books in a formatted table.
3. Search for books by title or author.
4. Update an existing book by its ID.
5. Remove a book by its ID after confirmation.
6. Save the inventory and exit.

## Current status

Member 3's components are present:

- `Book.h` and `Book.cpp` define and validate the shared `Book` record.
- `InputValidator.h` and `InputValidator.cpp` reject blank text, invalid numbers, and out-of-range menu choices.
- `FileHandler.h` and `FileHandler.cpp` load and save book records.
- `Member3Tests.cpp` tests validation, missing files, malformed records, duplicate IDs, and file round trips.
- `Library_System_Eye-Git.cpp` uses Member 1's menu, loads records at startup, and saves on Exit. Options 1–5 currently display placeholders.

The complete application still needs these teammate-owned files:

- `LibraryLogic.h` with declarations for `addBook`, `viewAllBooks`, `searchBook`, `updateBook`, and `removeBook`.
- `LibraryLogic.cpp` with the implementations of those five operations.

The current menu application compiles without these files. The five book operations will be connected when their modules are available.

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

The Member 3 test program does not depend on the missing `LibraryLogic` module.

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic \
  Book.cpp InputValidator.cpp FileHandler.cpp Member3Tests.cpp \
  -o member3-tests

./member3-tests
```

The tests print warnings for deliberately invalid records. A successful run exits without an assertion failure.

## Build the current menu application

In PowerShell, compile and run the menu with Member 3's modules:

```powershell
g++ -std=c++17 -Wall -Wextra -Wpedantic Library_System_Eye-Git.cpp Book.cpp InputValidator.cpp FileHandler.cpp -o library.exe
.\library.exe
```

## Build the complete application

After the team adds `LibraryLogic.h` and `LibraryLogic.cpp`, compile the program with:

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

1. Add and test all functions in `LibraryLogic.cpp`.
2. Compile and run the complete application.
3. Test valid input, invalid input, an empty inventory, duplicate IDs, and missing IDs.
4. Run `clang-format` on all C++ files.
5. Run `clang-tidy` or Cppcheck if either tool is available.
6. Record expected and actual results for at least five test cases.
7. Complete the AI interaction log and group contribution table.

## Git workflow

Create one feature branch per member. Commit one reviewable change at a time, push the branch, and open a pull request into `main`. Another teammate must review the pull request before it is merged.
