#include "LibraryLogic.h"

#include "InputValidator.h"

#include <iostream>
#include <limits>

bool addBook(std::vector<Book>& books) {
    std::cout << "\n=== Add Book ===\n";
    Book book{};

    while (true) {
        book.id = getRequiredText("Enter book ID: ");
        if (!std::cin) {
            return false;
        }
        if (!bookIdExists(books, book.id)) {
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
