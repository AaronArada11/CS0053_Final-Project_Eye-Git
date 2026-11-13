#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <string>

// Match the main menu's borders, expanding to keep longer text aligned.
inline void displayPanel(
    const std::string& title,
    std::initializer_list<std::string> lines = {}) {
    std::size_t width = std::max<std::size_t>(27, title.size());
    for (const auto& line : lines) {
        width = std::max(width, line.size());
    }
    const std::string border(width + 6, '=');
    const auto row = [width](const std::string& text) {
        std::cout << "|| " << text << std::string(width - text.size(), ' ') << " ||\n";
    };
    std::cout << '\n' << border << '\n';
    const std::size_t padding = (width - title.size()) / 2;
    row(std::string(padding, ' ') + title);
    std::cout << border << '\n';
    for (const auto& line : lines) {
        row(line);
    }
    if (lines.size() != 0) {
        std::cout << border << '\n';
    }
}

#endif
