#pragma once
#include <string>
#include <vector>
#include <optional>
#include <unistd.h>
#include <dirent.h>

std::vector<std::string> splitPath(std::string path);
std::optional<std::string> findExecutableInPath(const std::string& name);
std::optional<std::vector<std::string>> listExecutablesInPath();
