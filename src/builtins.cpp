#include "builtins.hpp"


static const std::unordered_set<std::string> builtin = {
    "echo", "exit", "type"
};

bool isBuiltin(const std::string& command_name) {
    return builtin.count(command_name);
}

void runBuiltin(const std::string& command_name, const std::vector<std::string>& args){
    if(command_name == "echo"){
        for(size_t i= 0; i < args.size(); i++){
            if(i) std::cout << " ";
            std::cout << args[i];
        }
        std::cout << std::endl;
    } else if (command_name == "type"){
        runType(args.empty() ? "" : args[0]);
    } else if (command_name == "exit"){
        exit(0);
    } else {
        std::cout << "Unknown builtin command: " << command_name << "\n";
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

    if (isBuiltin(cmd)) {
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