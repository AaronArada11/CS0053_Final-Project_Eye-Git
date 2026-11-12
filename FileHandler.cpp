#include "FileHandler.h"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

bool loadBooksFromFile(std::vector<Book>& books, const std::string& filename) {
    std::error_code error;
    if (!std::filesystem::exists(filename, error)) {
        if (error) {
            std::cerr << "Could not check the library data file: " << error.message() << '\n';
            return false;
        }
        books.clear();
        return true;
    }

    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Could not open " << filename << " for reading.\n";
        return false;
    }

    std::vector<Book> loadedBooks;
    std::string line;
    std::size_t lineNumber = 0;
    while (std::getline(file, line)) {
        ++lineNumber;
        if (line.empty()) {
            continue;
        }

        Book book;
        std::istringstream parser(line);
        parser >> std::quoted(book.id) >> std::quoted(book.title) >>
            std::quoted(book.author) >> book.publicationYear >> book.copies;
        char extra;
        if (!parser || (parser >> extra) || !isValidBook(book)) {
            std::cerr << "Skipped invalid record on line " << lineNumber << ".\n";
            continue;
        }
        if (bookIdExists(loadedBooks, book.id)) {
            std::cerr << "Skipped duplicate book ID on line " << lineNumber << ".\n";
            continue;
        }
        loadedBooks.push_back(book);
    }

    if (file.bad()) {
        std::cerr << "Could not finish reading " << filename << ".\n";
        return false;
    }
    books = std::move(loadedBooks);
    return true;
}

bool saveBooksToFile(const std::vector<Book>& books, const std::string& filename) {
    const std::string temporaryFilename = filename + ".tmp";
    std::ofstream file(temporaryFilename, std::ios::trunc);
    if (!file) {
        std::cerr << "Could not open " << temporaryFilename << " for writing.\n";
        return false;
    }

    for (const Book& book : books) {
        if (!isValidBook(book)) {
            std::cerr << "Refused to save an invalid book record.\n";
            file.close();
            std::filesystem::remove(temporaryFilename);
            return false;
        }
        file << std::quoted(book.id) << ' ' << std::quoted(book.title) << ' '
             << std::quoted(book.author) << ' ' << book.publicationYear << ' '
             << book.copies << '\n';
    }

    file.close();
    if (!file) {
        std::cerr << "Could not finish writing " << temporaryFilename << ".\n";
        std::filesystem::remove(temporaryFilename);
        return false;
    }

    std::error_code error;
    std::filesystem::rename(temporaryFilename, filename, error);
    if (error) {
        std::cerr << "Could not replace " << filename << ": " << error.message() << '\n';
        std::filesystem::remove(temporaryFilename);
        return false;
    }
    return true;
}
