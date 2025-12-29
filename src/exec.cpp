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

    if (isBuiltIn(cmd->command_name)) {
        runBuiltin(cmd->command_name, cmd->args);
        return;
    }

    std::vector<std::string> argv;
    argv.reserve(cmd->args.size()+1);
    argv.push_back(cmd->command_name);
    argv.insert(argv.end(), cmd->args.begin(), cmd->args.end());

    std::vector<char*> cstr_argv;
    for (auto& arg : argv) {
        cstr_argv.push_back(arg.data());
    }
    cstr_argv.push_back(nullptr);


    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        execvp(cstr_argv[0], cstr_argv.data());
        perror("execvp failed");
        exit(1);
    } else if (pid > 0) {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    } else {
        perror("fork failed");
    }
}

void executeRedirect(RedirectNode* redirect) {
    // 1. Fork so redirection does NOT affect the parent shell
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    } else if (pid == 0) {
        // ===== CHILD PROCESS =====
        // 2. Open (or create) the output file
        // O_TRUNC ensures > overwrites the file
        int fd = open(
            redirect->outfile.c_str(),
            O_CREAT | O_WRONLY | O_TRUNC, 0644 );
        if (fd < 0) {
            perror("open");
            _exit(1); // Do NOT return to shell loop
        }

        // 3. Redirect stdout (fd 1) to the file
        if (dup2(fd, STDOUT_FILENO) < 0) {
            perror("dup2");
            close(fd);
            _exit(1);
        }

        // fd is no longer needed after dup2
        close(fd);

        execute(redirect->child.get());

        // Ensure child never falls back into shell loop
        _exit(0);
    }

    // ===== PARENT PROCESS =====

    // 6. Wait for the redirected command to finish
    int status;
    waitpid(pid, &status, 0);
}


void executePipeline(PipelineNode* pipe) {
    // Implementation for executing a pipeline
}
