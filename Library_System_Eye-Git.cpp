#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "FileHandler.h"
#include "InputValidator.h"
#include "LibraryLogic.h"

using namespace std;

namespace {

string toUpper(string value) {
    transform(
        value.begin(),
        value.end(),
        value.begin(),
        [](unsigned char character) {
            return static_cast<char>(toupper(character));
        }
    );

    return value;
}

string toLower(string value) {
    transform(
        value.begin(),
        value.end(),
        value.begin(),
        [](unsigned char character) {
            return static_cast<char>(tolower(character));
        }
    );

    return value;
}

bool askConfirmation(const string& prompt) {
    while (true) {
        string answer = toUpper(getRequiredText(prompt));

        if (answer == "Y" || answer == "YES") {
            return true;
        }

        if (answer == "N" || answer == "NO") {
            return false;
        }

        cout << "Please enter Y or N.\n";
    }
}

Book* findBookById(vector<Book>& books, const string& id) {
    auto iterator = find_if(
        books.begin(),
        books.end(),
        [&id](const Book& book) {
            return book.id == id;
        }
    );

    if (iterator == books.end()) {
        return nullptr;
    }

    return &(*iterator);
}

bool containsIgnoreCase(
    const string& text,
    const string& searchTerm
) {
    return toLower(text).find(toLower(searchTerm)) != string::npos;
}

void displayBookTableHeader() {
    cout << left
         << setw(20) << "Book ID"
         << setw(35) << "Title"
         << setw(30) << "Author"
         << setw(20) << "Publication Year"
         << setw(10) << "Copies"
         << '\n';

    cout << string(115, '-') << '\n';
}

void displayBook(const Book& book) {
    cout << left
         << setw(20) << book.id
         << setw(35) << book.title
         << setw(30) << book.author
         << setw(20) << book.publicationYear
         << setw(10) << book.copies
         << '\n';
}

} // namespace

bool updateBook(vector<Book>& books) {
    cout << "\n=== Update Book ===\n";

    const string id = getRequiredText("Enter book ID: ");
    Book* book = findBookById(books, id);

    if (book == nullptr) {
        cout << "Book ID not found.\n";
        return false;
    }

    cout << "\nCurrent book details:\n";
    cout << "ID: " << book->id << '\n';
    cout << "Title: " << book->title << '\n';
    cout << "Author: " << book->author << '\n';
    cout << "Publication year: " << book->publicationYear << '\n';
    cout << "Copies: " << book->copies << "\n\n";

    Book updatedBook = *book;

    updatedBook.title = getRequiredText("Enter new title: ");
    updatedBook.author = getRequiredText("Enter new author: ");

    updatedBook.publicationYear = getValidInteger(
        "Enter new publication year: ",
        1450,
        2026
    );

    updatedBook.copies = getValidInteger(
        "Enter new number of copies: ",
        1,
        numeric_limits<int>::max()
    );

    if (!isValidBook(updatedBook)) {
        cout << "The updated book details are invalid.\n";
        return false;
    }

    cout << "\nUpdated book details:\n";
    cout << "ID: " << updatedBook.id << '\n';
    cout << "Title: " << updatedBook.title << '\n';
    cout << "Author: " << updatedBook.author << '\n';
    cout << "Publication year: "
         << updatedBook.publicationYear << '\n';
    cout << "Copies: " << updatedBook.copies << '\n';

    if (!askConfirmation("\nSave these changes? (Y/N): ")) {
        cout << "Update cancelled.\n";
        return false;
    }

    *book = updatedBook;

    cout << "Book updated successfully.\n";
    return true;
}

bool removeBook(vector<Book>& books) {
    cout << "\n=== Remove Book ===\n";

    const string id = getRequiredText("Enter book ID: ");

    auto iterator = find_if(
        books.begin(),
        books.end(),
        [&id](const Book& book) {
            return book.id == id;
        }
    );

    if (iterator == books.end()) {
        cout << "Book ID not found.\n";
        return false;
    }

    cout << "\nBook to remove:\n";
    cout << "ID: " << iterator->id << '\n';
    cout << "Title: " << iterator->title << '\n';
    cout << "Author: " << iterator->author << '\n';
    cout << "Publication year: "
         << iterator->publicationYear << '\n';
    cout << "Copies: " << iterator->copies << '\n';

    if (!askConfirmation(
            "\nAre you sure you want to remove this book? (Y/N): "
        )) {
        cout << "Book removal cancelled.\n";
        return false;
    }

    books.erase(iterator);

    cout << "Book removed successfully.\n";
    return true;
}

void searchBook(const vector<Book>& books) {
    cout << "\n=== Search Books ===\n";

    if (books.empty()) {
        cout << "The library inventory is empty.\n";
        return;
    }

    const string searchTerm = getRequiredText(
        "Enter a book title or author: "
    );

    bool bookFound = false;

    for (const Book& book : books) {
        const bool titleMatches =
            containsIgnoreCase(book.title, searchTerm);

        const bool authorMatches =
            containsIgnoreCase(book.author, searchTerm);

        if (titleMatches || authorMatches) {
            if (!bookFound) {
                cout << "\nSearch results:\n";
                displayBookTableHeader();
            }

            displayBook(book);
            bookFound = true;
        }
    }

    if (!bookFound) {
        cout << "No books matched \"" << searchTerm << "\".\n";
    }
}

// Main Program Module: Assigned to Member 1
// Handles the text-based menu and program integration.
int main() {
    vector<Book> libraryInventory;

    if (!loadBooksFromFile(libraryInventory)) {
        cerr << "Existing library data could not be loaded. "
             << "No changes were made.\n";
        return 1;
    }

    bool isRunning = true;

    while (isRunning) {
        cout << "\n=== Library Management System ===\n";
        cout << "1. Add Books\n";
        cout << "2. View Books\n";
        cout << "3. Search Books\n";
        cout << "4. Update Book Details\n";
        cout << "5. Remove Book\n";
        cout << "6. Exit\n";

        int choice = getValidMenuChoice(1, 6);

        // End cleanly if the input stream closes
        // (for example, Ctrl+Z on Windows).
        if (cin.eof() || cin.bad()) {
            cout << "\nInput closed. Exiting program...\n";
            break;
        }

        switch (choice) {
            case 1:
                if (addBook(libraryInventory)) {
                    if (!saveBooksToFile(libraryInventory)) {
                        cerr << "Warning: Added book could not be saved. "
                             << "It remains in memory; choose Exit to retry saving.\n";
                    }
                }
                break;

            case 2:
                viewAllBooks(libraryInventory);
                break;

            case 3:
                searchBook(libraryInventory);
                break;

            case 4:
                if (updateBook(libraryInventory)) {
                    if (!saveBooksToFile(libraryInventory)) {
                        cerr << "Warning: Updated book could not be saved.\n";
                    }
                }
                break;

            case 5:
                if (removeBook(libraryInventory)) {
                    if (!saveBooksToFile(libraryInventory)) {
                        cerr << "Warning: Removed book could not be saved.\n";
                    }
                }
                break;

            case 6:
                if (saveBooksToFile(libraryInventory)) {
                    cout << "Data saved. Exiting program...\n";
                } else {
                    cerr << "Warning: Data could not be saved. "
                         << "Exiting program...\n";
                }

                isRunning = false;
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    }

    return 0;
}