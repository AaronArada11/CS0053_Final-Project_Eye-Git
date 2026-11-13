#include "InputValidator.h"

#include <algorithm>
#include <cctype>
#include <sstream>

namespace {
std::string trim(const std::string& value) {
    const auto first = std::find_if_not(value.begin(), value.end(), [](unsigned char character) {
        return std::isspace(character);
    });
    const auto last = std::find_if_not(value.rbegin(), value.rend(), [](unsigned char character) {
        return std::isspace(character);
    }).base();
    return first < last ? std::string(first, last) : std::string();
}
}

int getValidInteger(const std::string& prompt, int minimum, int maximum,
                    std::istream& input, std::ostream& output) {
    std::string line;
    while (true) {
        output << prompt;
        if (!std::getline(input, line)) {
            output << "Input ended unexpectedly.\n";
            return minimum;
        }

        if (trim(line) == "0") {
            throw InputCancelled{};
        }
        std::istringstream parser(line);
        int value;
        char extra;
        if ((parser >> value) && !(parser >> extra) && value >= minimum && value <= maximum) {
            return value;
        }
        output << "Enter a whole number from " << minimum << " to " << maximum << ".\n";
    }
}

int getValidMenuChoice(int minimum, int maximum) {
    return getValidInteger("Choose an option: ", minimum, maximum);
}

std::string getRequiredText(const std::string& prompt, std::istream& input,
                            std::ostream& output) {
    std::string line;
    while (true) {
        output << prompt;
        if (!std::getline(input, line)) {
            output << "Input ended unexpectedly.\n";
            return {};
        }

        line = trim(line);
        if (line == "0") {
            throw InputCancelled{};
        }
        if (!line.empty()) {
            return line;
        }
        output << "This field cannot be blank.\n";
    }
}
