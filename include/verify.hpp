#pragma once

#include <vector>
#include "rule.hpp"

namespace fs = std::filesystem;

bool isValidPath(const fs::path& path, std::vector<Rule>& rulesOut, bool posFeedback = true);

int verify(int argc, char* argv[]);