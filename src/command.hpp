#pragma once
#include <string>
#include <vector>

enum class CommandType {
    Echo,
    Exit,
    Type,
    External
};

struct Command {
    CommandType type;
    std::string name;
    std::vector<std::string> args;

    // Future expansion:
    // std::string stdout_file;
    // bool append = false;
    // Command* pipe_next = nullptr;
};
