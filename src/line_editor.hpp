#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include "builtins.hpp"
#include "path.hpp"
#include <algorithm>

class LineEditor {
public:
    LineEditor() = default;

    // Reads a full line from the terminal, handling backspace, tab, etc.
    std::string readLine();

private:

    std::vector<std::string> builtin {"echo", "exit", "type"};

    // Autocomplete cache
    static std::string last_autocomplete_buffer;
    static std::vector<std::string> last_autocomplete_matches;
    static bool tabWasLastPress;
    static int autocomplete_index;

    // Terminal original settings
    static struct termios orig_termios;

    void handleAutocomplete(std::string& buffer);
    void enableRawMode();
    void disableRawMode();
};
