#include <iostream>
#include <vector>

#include "FileHandler.h"
#include "ConsoleUI.h"
#include "InputValidator.h"
#include "LibraryLogic.h"

using namespace std;

// Main Program Module: Assigned to Member 1
// Handles the text-based menu and program integration.
int main() {
    vector<Book> libraryInventory;

    if (!loadBooksFromFile(libraryInventory)) {
        cerr << "Existing library data could not be loaded. "
             << "No changes were made.\n";
        return 1;
    }

    bool isRunning = true;

    while (isRunning) {
        displayPanel("Library Management System", {
            "1. Add Books", "2. View Books", "3. Search Books",
            "4. Update Book Details", "5. Remove Book", "6. Exit"
        });

        try {
            int choice = getValidMenuChoice(1, 6);

            // End cleanly if the input stream closes
            // (for example, Ctrl+Z on Windows).
            if (cin.eof() || cin.bad()) {
                cout << "\nInput closed. Exiting program...\n";
                break;
            }

            switch (choice) {
                case 1:
                    if (addBook(libraryInventory)) {
                        if (!saveBooksToFile(libraryInventory)) {
                            cerr << "Warning: Added book could not be saved. "
                                 << "It remains in memory; choose Exit to retry saving.\n";
                        }
                    }
                    break;

                case 2:
                    viewAllBooks(libraryInventory);
                    break;

                case 3:
                    searchBook(libraryInventory);
                    break;

                case 4:
                    if (updateBook(libraryInventory)) {
                        if (!saveBooksToFile(libraryInventory)) {
                            cerr << "Warning: Updated book could not be saved.\n";
                        }
                    }
                    break;

                case 5:
                    if (removeBook(libraryInventory)) {
                        if (!saveBooksToFile(libraryInventory)) {
                            cerr << "Warning: Removed book could not be saved.\n";
                        }
                    }
                    break;

                case 6:
                    displayPanel("Exit", {"1. Save and exit", "0. Cancel"});
                    getValidInteger("Choose an option: ", 1, 1);
                    if (!cin) {
                        cout << "\nInput closed. Exiting program...\n";
                        isRunning = false;
                        break;
                    }
                    if (saveBooksToFile(libraryInventory)) {
                        cout << "Data saved. Exiting program...\n";
                    } else {
                        cerr << "Warning: Data could not be saved. "
                             << "Exiting program...\n";
                    }

                    isRunning = false;
                    break;

                default:
                    cout << "Invalid choice. Please try again.\n";
                    break;
            }
        } catch (const InputCancelled&) {
            cout << "Action cancelled. Returning to the main menu.\n";
        }
    }

    return 0;
}
