#include "parse.hpp"

Command parseInput(const std::string& input) {
    Command cmd;

    size_t idx = input.find(' ');
    if (idx == std::string::npos) {
        cmd.name = input;
    } else {
        cmd.name = input.substr(0, idx);
        cmd.args = buildArgList(input.substr(idx + 1));
    }

    if (cmd.name == "exit") cmd.type = CommandType::Exit;
    else if (cmd.name == "echo") cmd.type = CommandType::Echo;
    else if (cmd.name == "type") cmd.type = CommandType::Type;
    else cmd.type = CommandType::External;

    if (cmd.type == CommandType::External) {
        cmd.args.insert(cmd.args.begin(), cmd.name);
    }

    return cmd;
}


std::vector<std::string> buildArgList(const std::string& args_str) {
    std::vector<std::string> list;

    size_t it = 0;
    while (it < args_str.size()) {
        while (it < args_str.size() && args_str[it] == ' ') it++;

        size_t space_idx = args_str.find(' ', it);
        if (space_idx == std::string::npos) {
            list.push_back(args_str.substr(it));
            break;
        }
        list.push_back(args_str.substr(it, space_idx - it));
        it = space_idx + 1;
    }
    return list;
}

std::vector<char*> buildArgV(std::vector<std::string>& args) {
    std::vector<char*> argv;
  argv.reserve(args.size() + 1);
    for (auto& arg : args) {
        argv.push_back(const_cast<char*>(arg.c_str()));
    }
    argv.push_back(nullptr);
    return argv;
}