#pragma once

#include <string>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include "pattern.hpp"

class Rule {
    public:
        std::string name, description;
        std::unordered_set<std::string> extensions;
        Pattern pattern, antipattern;
    
        Rule(const nlohmann::json& obj);
};