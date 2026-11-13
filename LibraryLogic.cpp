#include "LibraryLogic.h"

#include "InputValidator.h"
#include "ConsoleUI.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

namespace {
void displayBookDetails(const std::string& title, const Book& book) {
    displayPanel(title, {
        "ID: " + book.id,
        "Title: " + book.title,
        "Author: " + book.author,
        "Publication year: " + std::to_string(book.publicationYear),
        "Copies: " + std::to_string(book.copies)
    });
}

// Compare without case or extra whitespace, while preserving entered text.
std::string duplicateKey(const std::string& text) {
    std::string key;
    bool pendingSpace = false;
    for (unsigned char character : text) {
        if (std::isspace(character)) {
            pendingSpace = !key.empty();
        } else {
            if (pendingSpace) {
                key += ' ';
            }
            key += static_cast<char>(std::tolower(character));
            pendingSpace = false;
        }
    }
    return key;
}

bool duplicateIdExists(const std::vector<Book>& books, const std::string& id) {
    const std::string key = duplicateKey(id);
    return std::any_of(books.begin(), books.end(), [&](const Book& book) {
        return duplicateKey(book.id) == key;
    });
}
}

bool addBook(std::vector<Book>& books) {
    displayPanel("Add Book");
    cout << "(press 0 to cancel)\n";
    Book book{};

    while (true) {
        book.id = getRequiredText("Enter book ID: ");
        if (!std::cin) {
            return false;
        }
        if (!duplicateIdExists(books, book.id)) {
            break;
        }
        std::cout << "Book ID already exists. Enter a unique ID.\n";
    }

    book.title = getRequiredText("Enter title: ");
    if (!std::cin) {
        return false;
    }
    book.author = getRequiredText("Enter author: ");
    if (!std::cin) {
        return false;
    }
    book.publicationYear = getValidInteger("Enter publication year: ", 1450, 2026);
    if (!std::cin) {
        return false;
    }
    book.copies = getValidInteger("Enter number of copies: ", 1,
                                  std::numeric_limits<int>::max());
    if (!std::cin || !isValidBook(book)) {
        return false;
    }

    books.push_back(book);
    std::cout << "Book added successfully.\n";
    return true;
}

void viewAllBooks(const std::vector<Book>& books) {
    displayPanel("View Books");
    if (books.empty()) {
        std::cout << "The library inventory is empty.\n";
        return;
    }

    // Expand text columns so long values remain complete and aligned.
    std::size_t idWidth = 10;
    std::size_t titleWidth = 25;
    std::size_t authorWidth = 20;
    for (const Book& book : books) {
        idWidth = std::max(idWidth, book.id.size() + 2);
        titleWidth = std::max(titleWidth, book.title.size() + 2);
        authorWidth = std::max(authorWidth, book.author.size() + 2);
    }

    const auto previousFlags = std::cout.flags();
    std::cout << std::left
              << std::setw(static_cast<int>(idWidth)) << "Book ID"
              << std::setw(static_cast<int>(titleWidth)) << "Title"
              << std::setw(static_cast<int>(authorWidth)) << "Author"
              << std::setw(18) << "Publication Year"
              << "Copies\n"
              << std::string(idWidth + titleWidth + authorWidth + 28, '-') << '\n';

    for (const Book& book : books) {
        std::cout << std::setw(static_cast<int>(idWidth)) << book.id
                  << std::setw(static_cast<int>(titleWidth)) << book.title
                  << std::setw(static_cast<int>(authorWidth)) << book.author
                  << std::setw(18) << book.publicationYear
                  << book.copies << '\n';
    }
    std::cout.flags(previousFlags);
    std::cout << "Total book records: " << books.size() << '\n';
}

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
        if (!cin) {
            return false;
        }

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
    displayPanel("Update Book");
    cout << "(press 0 to cancel)\n";

    if (books.empty()) {
        cout << "The library inventory is empty.\n";
        return false;
    }

    const string id = getRequiredText("Enter book ID: ");
    if (!cin) {
        return false;
    }
    Book* book = findBookById(books, id);

    if (book == nullptr) {
        cout << "Book ID not found.\n";
        return false;
    }

    displayBookDetails("Current book details", *book);

    Book updatedBook = *book;

    displayPanel("What would you like to update?", {
        "1. Title", "2. Author", "3. Publication Year", "4. Copies", "5. Cancel"
    });
    const int field = getValidMenuChoice(1, 5);
    if (!cin) {
        return false;
    }
    switch (field) {
        case 1:
            updatedBook.title = getRequiredText("Enter new title: ");
            break;
        case 2:
            updatedBook.author = getRequiredText("Enter new author: ");
            break;
        case 3:
            updatedBook.publicationYear = getValidInteger(
                "Enter new publication year: ", 1450, 2026);
            break;
        case 4:
            updatedBook.copies = getValidInteger(
                "Enter new number of copies: ", 1, numeric_limits<int>::max());
            break;
        case 5:
            cout << "Update cancelled.\n";
            return false;
    }
    if (!cin) {
        return false;
    }

    if (!isValidBook(updatedBook)) {
        cout << "The updated book details are invalid.\n";
        return false;
    }

    displayBookDetails("Updated book details", updatedBook);

    if (!askConfirmation("\nSave these changes? (Y/N): ")) {
        cout << "Update cancelled.\n";
        return false;
    }

    *book = updatedBook;

    cout << "Book updated successfully.\n";
    return true;
}

bool removeBook(vector<Book>& books) {
    displayPanel("Remove Book");
    cout << "(press 0 to cancel)\n";

    if (books.empty()) {
        cout << "The library inventory is empty.\n";
        return false;
    }

    const string id = getRequiredText("Enter book ID: ");
    if (!cin) {
        return false;
    }

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

    displayBookDetails("Book to remove", *iterator);
    displayPanel("Removal Options", {
        "1. Remove a number of copies", "2. Remove the entire book record", "3. Cancel"
    });
    const int action = getValidMenuChoice(1, 3);
    if (!cin) {
        return false;
    }
    if (action == 3) {
        cout << "Book removal cancelled.\n";
        return false;
    }

    int quantity = iterator->copies;
    if (action == 1) {
        quantity = getValidInteger("Enter copies to remove: ", 1, iterator->copies);
        if (!cin) {
            return false;
        }
    }
    cout << "Copies to remove: " << quantity << '\n';
    if (quantity == iterator->copies) {
        cout << "This will delete the entire book record.\n";
    }
    if (!askConfirmation("Confirm removal? (Y/N): ")) {
        cout << "Book removal cancelled.\n";
        return false;
    }
    if (quantity == iterator->copies) {
        books.erase(iterator);
        cout << "Book removed successfully.\n";
    } else {
        iterator->copies -= quantity;
        cout << "Copies removed successfully. Remaining copies: " << iterator->copies << '\n';
    }
    return true;
}

void searchBook(const vector<Book>& books) {
    displayPanel("Search Books");
    cout << "(press 0 to cancel)\n";

    if (books.empty()) {
        cout << "The library inventory is empty.\n";
        return;
    }

    const string searchTerm = getRequiredText(
        "Enter a book title or author: "
    );
    if (!cin) {
        return;
    }

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
