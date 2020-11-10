#include <string>
#include <vector>
#include <regex>
#include <map>
#include <iostream> //debug
#include "pattern.hpp"

using std::string;
using std::regex;

namespace {
    const std::map<string, regex::flag_type> flagMap = { // C++11
        { "i", regex::icase },
        { "ECMAScript", regex::ECMAScript },
        { "basic", regex::basic },
        { "extended", regex::extended },
        { "awk", regex::awk },
        { "grep", regex::grep },
        { "egrep", regex::egrep }
    };
}

Pattern::Pattern(const string& regex, const std::vector<string>& modifiers) {
    bool grammarSpecified = false;
    regex::flag_type flags;
    for (int i = 0; i < modifiers.size(); i++) {
        flags |= flagMap.at(modifiers[i]);
        if (modifiers[i] != "i") {
            grammarSpecified ? throw "Only one grammar allowed." : grammarSpecified = true;
        }
    }
    if (!grammarSpecified) flags |= regex::ECMAScript;
    flags |= regex::optimize; // investigate if this should be user opt-out
    flags |= regex::nosubs;
    this->regex = std::regex(regex, flags);
}