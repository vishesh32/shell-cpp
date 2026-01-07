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
    LineEditor();

    // Reads a full line from the terminal, handling backspace, tab, etc.
    std::string readLine();

private:

    void handleAutocomplete(std::string& buffer);
    void enableRawMode();
    void disableRawMode();
};
