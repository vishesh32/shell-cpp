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

    std::vector<std::string> inbuilt_matches;
    for (const auto &cmd : builtin) {  // fixed variable name
        if (cmd.compare(0, buffer.size(), buffer) == 0)
            inbuilt_matches.push_back(cmd);
    }

    auto path_executables = listExecutablesInPath();
    std::vector<std::string> path_matches;
    if (path_executables.has_value()) {
        const auto &executables = path_executables.value();
        for (const auto &exe : executables) {
            if (exe.compare(0, buffer.size(), buffer) == 0) {
                path_matches.push_back(exe);
            }
        }
    }
    
    std::vector<std::string> all_matches;
    all_matches.reserve(inbuilt_matches.size() + path_matches.size());
    all_matches.insert(all_matches.end(), inbuilt_matches.begin(), inbuilt_matches.end());
    all_matches.insert(all_matches.end(), path_matches.begin(), path_matches.end());
    std::sort(all_matches.begin(), all_matches.end());
    all_matches.erase(
        std::unique(all_matches.begin(), all_matches.end()),
        all_matches.end());

    if (all_matches.size() == 1) {
        std::string suffix = all_matches[0].substr(buffer.size());
        buffer += suffix;
        std::cout << suffix;
        buffer += ' '; //trailing space after autocomplete
        std::cout << ' ';
        std::cout.flush();
    } else if (!all_matches.empty()) {
        std::cout << "\n";
        for (auto& m : all_matches) std::cout << m << "  "; //list inbuilt_matches
        std::cout << "\n$ " << buffer;
        std::cout.flush();
    } else if (all_matches.empty()) {
        // No all_matches, ring bell
        buffer += '\a';
        std::cout << '\a';
        std::cout.flush();
    }
}
