#include "builtins.hpp"


static const std::unordered_set<std::string> builtin = {
    "echo", "exit", "type"
};

bool isBuiltin(const Command& cmd) {
    return builtin.count(cmd.name);
}

void runBuiltin(const Command& cmd) {
    if (cmd.type == CommandType::Echo) {
        for (size_t i = 0; i < cmd.args.size(); i++) {
            if (i) std::cout << " ";
            std::cout << cmd.args[i];
        }
        std::cout << "\n";
    }
    else if (cmd.type == CommandType::Type) {
        runType(cmd.args.empty() ? "" : cmd.args[0]);
    }
}

void runType(const std::string& args) {
    if (args.empty()) {
        std::cout << "type: missing argument\n";
        return;
    }

    size_t idx = args.find(' ');
    std::string cmd = (idx == std::string::npos)
        ? args
        : args.substr(0, idx);

    static const std::unordered_set<std::string> builtin = {
        "echo", "exit", "type"
    };

    if (builtin.count(cmd)) {
        std::cout << cmd << " is a shell builtin\n";
        return;
    }

    auto path = findExecutableInPath(cmd);
    if (path) {
        std::cout << cmd << " is " << *path << "\n";
    } else {
        std::cout << cmd << ": not found\n";
    }
}