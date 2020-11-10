#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include "verify.hpp"
#include "rule.hpp"

using std::string;
using std::cout;
using std::cerr;
using std::vector;
using std::ifstream;
using json = nlohmann::json;
namespace fs = std::filesystem; 

namespace {
    const string usage = string("Usage: ")
                            + program_invocation_short_name
                            + " verify RULES_PATH\n";
}

/*
 * Checks if a single json file is a valid rule.
 */
static bool isValidFile(const fs::path& path, vector<Rule>& rulesOut) {
    try {
        ifstream readfile(path);
        json arr = json::parse(readfile);
        readfile.close();
        if (!arr.is_array()) throw "All rules must be in arrays";
        if (arr.empty()) throw "Rules file is an empty array";
        for (json::iterator ptr = arr.begin(); ptr < arr.end(); ptr++) {
            Rule r(*ptr);
            rulesOut.push_back(r);
        }
    } catch (const json::parse_error e) {
        cerr << "Parse error on " << path << '\n'
             << "Message: " << e.what() << '\n'
             << "Byte position of error: " << e.byte << "\n\n";
        return false;
    } catch (const char* e) {
        cerr << "Error on " << path << '\n'
             << "Message: " << e << "\n\n";
        return false;
    } catch (std::out_of_range e) {
        cerr << "Error creating rule.\n"
             << "Message: " << e.what() << '\n';
        return false;
    }
    return true;
}

/*
 * Checks if a path only contains valid json rules
 */
bool isValidPath(const fs::path& path, vector<Rule>& rulesOut, bool posFeedback) {
    if (fs::is_directory(path)) {
        int badfiles = 0;
        for (auto& p: fs::recursive_directory_iterator(path)) {
            if (p.is_regular_file() 
                && p.path().extension() == ".json" 
                && !isValidFile(p, rulesOut)) badfiles++;
        }
        if (badfiles > 0) {
            cerr << badfiles << " bad rule file(s).\n";
            return false;
        }
        if (posFeedback) cout << "All rules are valid.\n";
    } else if (fs::is_regular_file(path)) {
        if (path.extension() != ".json") {
            cerr << path << " is not json.\n";
            return false;
        }
        if (isValidFile(path, rulesOut)) {
            if (posFeedback) cout << path << " is valid.\n";
        } else {
            cerr << path << " is invalid.\n";
            return false;
        }
    } else {
        cerr << path << " is not a regular file or directory.\n";
        return false;
    }
    return true;
}

/*
 * Verifies that a directory only contains valid (regex) rules
 */
int verify(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << usage;
        return 1;
    }
    if (!fs::exists(argv[2])) {
        cerr << "File or directory " << argv[2] << " does not exist.\n";
        return 1;
    }
    fs::path argPath(argv[2]);
    vector<Rule> rules;
    if (!isValidPath(argPath, rules)) return 1;
    cout << "Loaded " << rules.size() << " rule(s).\n";
    return 0;
}