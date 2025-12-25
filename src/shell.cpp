#include "shell.hpp"


void Shell::run() {
    while (true) {
        std::cout << "$ ";
        std::string input;
        std::getline(std::cin, input);

        Command cmd = parseInput(input);

        if (cmd.type == CommandType::Exit) return;

        if (isBuiltin(cmd)) {
            runBuiltin(cmd);
        } else {
            executeExternal(cmd);
        }
    }
}
