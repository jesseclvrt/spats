#pragma once

#include <string>
#include <vector>
#include <regex>

class Pattern {
    public:
        std::regex regex;

        Pattern(const std::string& regex = "", const std::vector<std::string>& modifiers = {});
};