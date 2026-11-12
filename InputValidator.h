#ifndef INPUT_VALIDATOR_H
#define INPUT_VALIDATOR_H

#include <iostream>
#include <sstream>
#include <string>

// Temporary implementation until Aaron's validation module is integrated.
inline int getValidMenuChoice(int minimum, int maximum) {
    std::string input;

    while (true) {
        std::cout << "Enter your choice (" << minimum << "-" << maximum << "): ";
        if (!std::getline(std::cin, input)) {
            return maximum;
        }

        std::istringstream stream(input);
        int choice;
        char extra;
        if ((stream >> choice) && !(stream >> extra) &&
            choice >= minimum && choice <= maximum) {
            return choice;
        }

        std::cout << "Invalid choice. Please enter a whole number from "
                  << minimum << " to " << maximum << ".\n";
    }
}

#endif
