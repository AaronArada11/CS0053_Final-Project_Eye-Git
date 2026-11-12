#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <vector>

struct Book {
    std::string id;
    std::string title;
    std::string author;
    int publicationYear;
    int copies;
};

bool bookIdExists(const std::vector<Book>& books, const std::string& id);
bool isValidBook(const Book& book);

#endif
