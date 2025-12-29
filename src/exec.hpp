#pragma once
#include "builtins.hpp"
#include "command.hpp"
#include "path.hpp"
#include <iostream>
#include "parse.hpp"
#include <sys/wait.h> // waitpid
#include <cassert>
#include "parser.hpp"
#include <fcntl.h>   // open(), O_CREAT, O_WRONLY, O_TRUNC
#include <unistd.h>  // dup2(), close()


void execute(ASTNode* ast);

void executeCommand(CommandNode* cmd);

void executeRedirect(RedirectNode* redirect);

void executePipeline(PipelineNode* pipe);
