#include <iostream>
#include <vector>
#include "Book.h"
#include "InputValidator.h"
#include "FileHandler.h"
#include "LibraryLogic.h"

using namespace std;

// Main Program Module: Assigned to Member 1
// Handles the text-based menu and program integration.
int main() {
    vector<Book> libraryInventory;
    
    // Load existing records on startup
    if (!loadBooksFromFile(libraryInventory)) {
        cerr << "Existing library data could not be loaded. No changes were made.\n";
        return 1;
    }
    
    bool isRunning = true;
    
    while (isRunning) {
        cout << "\n=== Library Management System ===\n";
        cout << "1. Add Books\n";
        cout << "2. View Books\n";
        cout << "3. Search Books\n";
        cout << "4. Update Book Details\n";
        cout << "5. Remove Book\n";
        cout << "6. Exit\n";
        
        int choice = getValidMenuChoice(1, 6);
        
        switch (choice) {
            case 1:
                addBook(libraryInventory);
                if (!saveBooksToFile(libraryInventory)) {
                    cerr << "Warning: Changes could not be saved.\n";
                }
                break;
            case 2:
                viewAllBooks(libraryInventory);
                break;
            case 3:
                searchBook(libraryInventory);
                break;
            case 4:
                updateBook(libraryInventory);
                if (!saveBooksToFile(libraryInventory)) {
                    cerr << "Warning: Changes could not be saved.\n";
                }
                break;
            case 5:
                removeBook(libraryInventory);
                if (!saveBooksToFile(libraryInventory)) {
                    cerr << "Warning: Changes could not be saved.\n";
                }
                break;
            case 6:
                // Save records before exiting
                if (saveBooksToFile(libraryInventory)) {
                    cout << "Data saved. Exiting program...\n";
                } else {
                    cerr << "Warning: Data could not be saved. Exiting program...\n";
                }
                isRunning = false;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    }
    
    return 0;
}
