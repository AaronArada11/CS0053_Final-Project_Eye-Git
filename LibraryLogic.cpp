#include "LibraryLogic.h"

#include "InputValidator.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>

namespace {
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
    std::cout << "\n=== Add Book ===\n";
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
    std::cout << "\n=== View Books ===\n";
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
