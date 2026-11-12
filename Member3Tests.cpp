#include "Book.h"
#include "FileHandler.h"
#include "InputValidator.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>

namespace {
void testValidation() {
    std::istringstream numbers("letters\n7\n3\n");
    std::ostringstream numberOutput;
    assert(getValidInteger("Choice: ", 1, 6, numbers, numberOutput) == 3);
    assert(numberOutput.str().find("whole number") != std::string::npos);

    std::istringstream text("   \n  The Hobbit  \n");
    std::ostringstream textOutput;
    assert(getRequiredText("Title: ", text, textOutput) == "The Hobbit");
    assert(textOutput.str().find("cannot be blank") != std::string::npos);
}

void testPersistence() {
    const std::filesystem::path path =
        std::filesystem::temp_directory_path() / "library_member3_test.txt";
    std::filesystem::remove(path);

    std::vector<Book> books{{"978-0", "A \"Quoted\" Title", "Author Name", 2020, 3}};
    assert(saveBooksToFile(books, path.string()));

    std::vector<Book> loaded;
    assert(loadBooksFromFile(loaded, path.string()));
    assert(loaded.size() == 1);
    assert(loaded[0].id == books[0].id);
    assert(loaded[0].title == books[0].title);
    assert(loaded[0].author == books[0].author);
    assert(loaded[0].publicationYear == books[0].publicationYear);
    assert(loaded[0].copies == books[0].copies);

    books[0].copies = 4;
    assert(saveBooksToFile(books, path.string()));
    assert(loadBooksFromFile(loaded, path.string()));
    assert(loaded[0].copies == 4);
    std::filesystem::remove(path);
}

void testMissingAndInvalidFiles() {
    const std::filesystem::path path =
        std::filesystem::temp_directory_path() / "library_member3_invalid_test.txt";
    std::filesystem::remove(path);

    std::vector<Book> books{{"old", "Old Title", "Old Author", 2000, 1}};
    assert(loadBooksFromFile(books, path.string()));
    assert(books.empty());

    {
        std::ofstream file(path);
    }
    books.push_back({"old", "Old Title", "Old Author", 2000, 1});
    assert(loadBooksFromFile(books, path.string()));
    assert(books.empty());

    {
        std::ofstream file(path);
        file << "invalid row\n";
        file << "\"id-1\" \"Valid Title\" \"Valid Author\" 2024 2\n";
        file << "\"id-1\" \"Duplicate\" \"Other Author\" 2023 1\n";
        file << "\"id-2\" \"Bad Year\" \"Author\" 1200 1\n";
    }

    assert(loadBooksFromFile(books, path.string()));
    assert(books.size() == 1);
    assert(books[0].id == "id-1");
    std::filesystem::remove(path);
}
}

int main() {
    testValidation();
    testPersistence();
    testMissingAndInvalidFiles();
}
