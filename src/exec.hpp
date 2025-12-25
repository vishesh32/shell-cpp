#pragma once
#include "command.hpp"
#include "path.hpp"
#include <iostream>
#include "parse.hpp"
#include <sys/wait.h> // waitpid

void executeExternal(const Command& cmd);
