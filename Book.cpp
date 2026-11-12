#include "Book.h"

#include <algorithm>
#include <cctype>

namespace {
bool isBlank(const std::string& value) {
    return std::all_of(value.begin(), value.end(), [](unsigned char character) {
        return std::isspace(character);
    });
}
}

bool bookIdExists(const std::vector<Book>& books, const std::string& id) {
    return std::any_of(books.begin(), books.end(), [&id](const Book& book) {
        return book.id == id;
    });
}

bool isValidBook(const Book& book) {
    return !book.id.empty() && !isBlank(book.id) && !book.title.empty() &&
           !isBlank(book.title) && !book.author.empty() && !isBlank(book.author) &&
           book.publicationYear >= 1450 && book.publicationYear <= 2026 &&
           book.copies > 0;
}
