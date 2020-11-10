#include <map>
#include <unordered_set>
#include <string>

using std::string;
using std::unordered_set;

const std::map<string, unordered_set<string>> languageMap = { // C++11
    { "c", unordered_set<string>    ({".c", ".h"}) },
    { "c++", unordered_set<string>  ({".C", ".H", ".cc", //maybe this is too much
                                    ".hh", ".cpp", ".hpp",
                                    ".CPP", ".HPP", ".c++",
                                    ".h++", ".cp", ".hp",
                                    ".cxx", ".hxx", ".h"}) }
};