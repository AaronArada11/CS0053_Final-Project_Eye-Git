#ifndef LIBRARY_LOGIC_H
#define LIBRARY_LOGIC_H

#include <vector>

#include "Book.h"

bool updateBook(std::vector<Book>& books);
bool removeBook(std::vector<Book>& books);
void searchBook(const std::vector<Book>& books);


// Returns true only when a complete, valid book was added.
bool addBook(std::vector<Book>& books);
void viewAllBooks(const std::vector<Book>& books);

#endif
