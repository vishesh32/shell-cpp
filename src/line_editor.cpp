#include "line_editor.hpp"


struct termios LineEditor::orig_termios;
std::string LineEditor::last_autocomplete_buffer = "";
std::vector<std::string> LineEditor::last_autocomplete_matches;
bool LineEditor::tabWasLastPress = false;
int LineEditor::autocomplete_index = 0;

void LineEditor::enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) perror("tcgetattr");

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON); // Disable echo + canonical mode
    raw.c_cc[VMIN] = 1;   // read() returns after 1 byte
    raw.c_cc[VTIME] = 0;  // no timeout

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) perror("tcsetattr");
}

void LineEditor::disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) perror("tcsetattr");
}

std::string LineEditor::readLine() {
    std::string buffer;
    enableRawMode();

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1) {
        if (c == '\n') {
            tabWasLastPress = false;
            last_autocomplete_buffer.clear();
            autocomplete_index = 0;
            std::cout << "\n";
            break;
        } else if (c == '\t') {
            handleAutocomplete(buffer);
        } else if (c == 127) { // backspace
            tabWasLastPress = false;
            last_autocomplete_buffer.clear();
            autocomplete_index = 0;
            if (!buffer.empty()) {
                buffer.pop_back();
                std::cout << "\b \b";
                std::cout.flush();
            }
        } else {
            tabWasLastPress = false;
            last_autocomplete_buffer.clear();
            autocomplete_index = 0;
            buffer.push_back(c);
            std::cout << c;
            std::cout.flush();
        }
    }

    disableRawMode();
    return buffer;
}

void LineEditor::handleAutocomplete(std::string &buffer) {
    // Only complete first word
    if (buffer.find(' ') != std::string::npos) return;

    // Check cache
    if (buffer == last_autocomplete_buffer) {
        // use cached matches
    } else {
        last_autocomplete_buffer = buffer;
        last_autocomplete_matches.clear();
        autocomplete_index = 0;

        for (const auto &cmd : builtin) {
            if (cmd.compare(0, buffer.size(), buffer) == 0)
                last_autocomplete_matches.push_back(cmd);
        }

        auto path_executables = listExecutablesInPath();
        if (path_executables.has_value()) {
            const auto &executables = path_executables.value();
            for (const auto &exe : executables) {
                if (exe.compare(0, buffer.size(), buffer) == 0)
                    last_autocomplete_matches.push_back(exe);
            }
        }

        std::sort(last_autocomplete_matches.begin(), last_autocomplete_matches.end());
        last_autocomplete_matches.erase(
            std::unique(last_autocomplete_matches.begin(), last_autocomplete_matches.end()),
            last_autocomplete_matches.end()
        );
    }

    // Handle tab 
    if (last_autocomplete_matches.size() > 1) {
        // Calculate longest common prefix
        std::string lcp = last_autocomplete_matches[0];
        for (size_t i = 1; i < last_autocomplete_matches.size(); i++) {
            size_t j = 0;
            while (j < lcp.size() && j < last_autocomplete_matches[i].size() 
                   && lcp[j] == last_autocomplete_matches[i][j]) {
                j++;
            }
            lcp = lcp.substr(0, j);
        }
        
        // If LCP is longer than buffer, complete to LCP
        if (lcp.size() > buffer.size()) {
            std::string suffix = lcp.substr(buffer.size());
            buffer += suffix;
            std::cout << suffix;
            std::cout.flush();
            tabWasLastPress = false;
            return;
        }
        
        // Otherwise ambiguous - ring bell on first tab
        if (!tabWasLastPress) {
            std::cout << '\a';
            std::cout.flush();
            tabWasLastPress = true;
            return;
        }
        
        // Second tab lists matches
        std::cout << "\n";
        for (const auto &m : last_autocomplete_matches) std::cout << m << "  ";
        std::cout << "\n$ " << buffer;
        std::cout.flush();
        tabWasLastPress = false;
        return;
    } else if (last_autocomplete_matches.size() == 1) {
        std::string suffix = last_autocomplete_matches[0].substr(buffer.size());
        buffer += suffix;
        std::cout << suffix;
        buffer += ' ';
        std::cout << ' ';
        std::cout.flush();
        tabWasLastPress = false;
    } else {
        // No matches rings bell
        std::cout << '\a';
        std::cout.flush();
        tabWasLastPress = false;
    }
}