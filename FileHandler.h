#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "Book.h"

#include <string>
#include <vector>

bool loadBooksFromFile(std::vector<Book>& books,
                       const std::string& filename = "library_data.txt");
bool saveBooksToFile(const std::vector<Book>& books,
                     const std::string& filename = "library_data.txt");

#endif
