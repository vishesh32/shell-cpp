#include "exec.hpp"


void executeExternal(const Command& cmd) {
    auto full_path = findExecutableInPath(cmd.name);
    if (!full_path) {
        std::cout << cmd.name << ": command not found\n";
        return;
    }

    std::vector<char*> argv = buildArgV(
        const_cast<std::vector<std::string>&>(cmd.args)
    );

    pid_t pid = fork();
    if (pid == 0) {
        execv(full_path->c_str(), argv.data());
        perror("execv failed");
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}
