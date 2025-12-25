#pragma once
#include "command.hpp"
#include <string>

Command parseInput(const std::string& input);

std::vector<std::string> buildArgList(const std::string& args_str);
std::vector<char*> buildArgV(std::vector<std::string>& args);