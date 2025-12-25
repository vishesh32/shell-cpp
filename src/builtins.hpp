#pragma once
#include "command.hpp"
#include "path.hpp"
#include <iostream>
#include <unordered_set>

bool isBuiltin(const Command& cmd);
void runBuiltin(const Command& cmd);
void runType(const std::string& args);
