#ifndef INPUT_VALIDATOR_H
#define INPUT_VALIDATOR_H

#include <iostream>
#include <string>
#include <exception>

// Cancels the current menu action without accepting a partial value.
class InputCancelled : public std::exception {
public:
    const char* what() const noexcept override { return "Action cancelled."; }
};

int getValidInteger(const std::string& prompt, int minimum, int maximum,
                    std::istream& input = std::cin,
                    std::ostream& output = std::cout);
int getValidMenuChoice(int minimum, int maximum);
std::string getRequiredText(const std::string& prompt,
                            std::istream& input = std::cin,
                            std::ostream& output = std::cout);

#endif
