#include <iostream>
#include "InputValidator.h"

using namespace std;

// Main Program Module: Assigned to Member 1
// Handles the text-based menu and program integration.
int main() {
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
        // End cleanly if the input stream closes (for example, Ctrl+Z on Windows).
        if (cin.eof() || cin.bad()) {
            cout << "\nInput closed. Exiting program...\n";
            break;
        }
        
        switch (choice) {
            case 1:
                cout << "Add Books feature is not yet available.\n";
                break;
            case 2:
                cout << "View Books feature is not yet available.\n";
                break;
            case 3:
                cout << "Search Books feature is not yet available.\n";
                break;
            case 4:
                cout << "Update Book Details feature is not yet available.\n";
                break;
            case 5:
                cout << "Remove Book feature is not yet available.\n";
                break;
            case 6:
                // Connect the file-saving module during integration.
                cout << "Exiting program...\n";
                isRunning = false;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    }
    
    return 0;
}
