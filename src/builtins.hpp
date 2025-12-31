#pragma once
#include "command.hpp"
#include "path.hpp"
#include <iostream>
#include <unordered_set>

extern const std::unordered_set<std::string> builtin;

bool isBuiltin(const std::string& command_name);
void runBuiltin(const std::string& command_name, const std::vector<std::string>& args);
void runType(const std::string& args);
