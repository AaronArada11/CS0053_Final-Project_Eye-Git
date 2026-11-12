#ifndef LIBRARY_LOGIC_H
#define LIBRARY_LOGIC_H

#include <vector>

#include "Book.h"

using namespace std;

bool updateBook(vector<Book>& books);
bool removeBook(vector<Book>& books);
void searchBook(const vector<Book>& books);


// Returns true only when a complete, valid book was added.
bool addBook(std::vector<Book>& books);
void viewAllBooks(const std::vector<Book>& books);

#endif
