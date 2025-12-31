#include "exec.hpp"


void execute(ASTNode* ast) {
    if (!ast) return;

    switch (ast->type) {
        case ASTNodeType::Command: {
            auto cmd = static_cast<CommandNode*>(ast);
            executeCommand(cmd);
            break;
        }
        case ASTNodeType::Redirect: {
            auto redirect = static_cast<RedirectNode*>(ast);
            executeRedirect(redirect);
            break;
        }
        case ASTNodeType::Pipeline: {
            auto* pipe = static_cast<PipelineNode*>(ast);
            executePipeline(pipe);
            break;
        }

        // Handle other AST node types...
    }
}

void executeCommand(CommandNode* cmd) {

    if (isBuiltin(cmd->command_name)) {
        runBuiltin(cmd->command_name, cmd->args);
        return;
    }

    std::vector<std::string> argv;
    argv.reserve(cmd->args.size()+1);
    argv.push_back(cmd->command_name);
    argv.insert(argv.end(), cmd->args.begin(), cmd->args.end());

    std::vector<char*> cstr_argv;
    for (auto& arg : argv) {
        cstr_argv.push_back(const_cast<char*>(arg.c_str()));
    }
    cstr_argv.push_back(nullptr);


    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        execvp(cstr_argv[0], cstr_argv.data());
        std::cout << cstr_argv[0] << ": command not found" << std::endl;
        exit(1);
    } else if (pid > 0) {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    } else {
        perror("fork failed");
    }
}

void execCommandNoFork(CommandNode* cmd) {
    if (isBuiltin(cmd->command_name)) {
        runBuiltin(cmd->command_name, cmd->args);
        exit(0); // Exit after running builtin in child process returns
    }
    
    std::vector<std::string> argv;
    argv.reserve(cmd->args.size()+1);
    argv.push_back(cmd->command_name);
    argv.insert(argv.end(), cmd->args.begin(), cmd->args.end());

    std::vector<char*> cstr_argv;
    for (auto& arg : argv) {
        cstr_argv.push_back(const_cast<char*>(arg.c_str()));
    }
    cstr_argv.push_back(nullptr);

    execvp(cstr_argv[0], cstr_argv.data());
    //exec only returns on failure
    std::cout << cstr_argv[0] << ": command not found" << std::endl;
    exit(1);
}

void executeRedirect(RedirectNode* redirect) {
    int target_fd;
    switch (redirect->redirect_type) {
    case RedirectType::StdOut:
        target_fd = STDOUT_FILENO;
        break;
    case RedirectType::StdErr:
        target_fd = STDERR_FILENO;
        break;
    }

    if (!redirect || !redirect->child) return;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return;
    } else if (pid == 0) {
        // ===== CHILD PROCESS =====
        // 1. Open (or create) the output file O_TRUNC ensures '>' overwrites existing files
        int fd = open(redirect->outfile.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (fd < 0) {
            _exit(1);  // exit child process immediately, file open error
        }

        // 2. Redirect stdout/stderr to the file
        if (dup2(fd, target_fd) < 0) {
            perror("dup2");
            close(fd);
            _exit(1);
        }

        // 3. Close the original file descriptor; it's no longer needed
        close(fd);
        // 4. Execute the child command (could be another CommandNode or nested AST)
        if (redirect->child->type == ASTNodeType::Command) {
            execCommandNoFork(static_cast<CommandNode*>(redirect->child.get()));
        }

        // 5. Exit child; prevent it from returning to shell loop
        _exit(0);
    }

    // ===== PARENT PROCESS =====
    // Wait for the child process to finish
    int status; // unused variable, used to debug errors but shells need to fail silently unless asked otherwise
    waitpid(pid, &status, 0);

}


void executePipeline(PipelineNode* pipe) {
    // Implementation for executing a pipeline
}
