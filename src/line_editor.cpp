#include "line_editor.hpp"


LineEditor::LineEditor() {}

static struct termios orig_termios;

// Enable raw mode to read keypresses without waiting for newline
void LineEditor::enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) perror("tcgetattr");

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON); // Disable echo and canonical mode
    raw.c_cc[VMIN] = 1;  // Minimum number of bytes before read returns
    raw.c_cc[VTIME] = 0; // No timeout

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) perror("tcsetattr");
}

// Restore original terminal settings
void LineEditor::disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) perror("tcsetattr");
}

std::string LineEditor::readLine() {
    std::string buffer;
    enableRawMode();

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1) {
        if (c == '\n') {
            std::cout << "\n";
            break;
        } else if (c == '\t') {
            handleAutocomplete(buffer);
        } else if (c == 127) { // backspace
            if (!buffer.empty()) {
                buffer.pop_back();
                std::cout << "\b \b";
                std::cout.flush();
            }
        } else {
            buffer.push_back(c);
            std::cout << c;
            std::cout.flush();
        }
    }

    disableRawMode();
    return buffer;
}

void LineEditor::handleAutocomplete(std::string& buffer) {
    // Only complete first word (command)
    if (buffer.find(' ') != std::string::npos) return;

    std::vector<std::string> matches;
    for (auto& cmd : builtin) {  // fixed variable name
        if (cmd.compare(0, buffer.size(), buffer) == 0)
            matches.push_back(cmd);
    }

    if (matches.size() == 1) {
        std::string suffix = matches[0].substr(buffer.size());
        buffer += suffix;
        std::cout << suffix;
        std::cout.flush();
    } else if (!matches.empty()) {
        std::cout << "\n";
        for (auto& m : matches) std::cout << m << "  ";
        std::cout << "\n$ " << buffer;
        std::cout.flush();
    }
}
