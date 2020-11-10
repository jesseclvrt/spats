#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <vector>
#include <regex>
#include <map>
#include <nlohmann/json.hpp>
#include "regex.hpp"
#include "verify.hpp"
#include "rule.hpp"

#define OUTPUT_INDENT 4

using std::string;
using std::cerr;
using std::vector;
using std::ofstream;
using json = nlohmann::json;
namespace fs = std::filesystem; 

namespace {
    const string usage = string("Usage: ")
                            + program_invocation_short_name
                            + " regex RULES_PATH SCAN_PATH OUTPUT_PATH\n";
}

static int getLineFromPosition(const string& file, const std::ptrdiff_t& pos, const std::ptrdiff_t& offset) {
    int line = 1;
    ptrdiff_t truePos = pos + offset;
    while (truePos >= 0) {
        if (file.at(truePos) == '\n') line++;
        truePos--;
    }
    return line;
}

static void addFinding(const std::smatch res,
                       const Rule& rule, 
                       const fs::path& p, 
                       json::array_t& findings,
                       const string& file,
                       const ptrdiff_t& offset) {
    json j = { //todo: find a way to force order without digits or hackiness
        {"1. name", rule.name},
        {"2. description", rule.description},
        {"3. match", res[0]},
        {"4. path", p},
        {"5. line", getLineFromPosition(file, res.position(0), offset)},
        {"6. byte position", res.position(0) + offset}
    };
    findings.push_back(j);
}

static void regexOneFile(const fs::path& file, const vector<Rule>& rules, json::array_t& findings) {
    vector<int> chosenRules;
    for (int i = 0; i < rules.size(); i++) {
        if (rules[i].extensions.count(file.extension())) chosenRules.push_back(i);
    }
    std::ifstream in(file);
    std::ostringstream buf;
    buf << in.rdbuf();
    const string fullFile = buf.str();
    string searchString = fullFile;
    std::smatch res;
    for (int i: chosenRules) {
        string searchString = fullFile;
        ptrdiff_t offset = 0;
        while (std::regex_search(searchString, res, rules[i].pattern.regex)) {
            addFinding(res, rules[i], file, findings, fullFile, offset);
            offset += res.position(0) + res[0].length();
            searchString = res.suffix().str();
        }
    }
}

static bool regexScan(const fs::path& rulesPath, const fs::path& scanPath, ofstream& output) {
    vector<Rule> rules;
    json::array_t findings;
    if (!isValidPath(rulesPath, rules, false)) { return false; }
    for (auto& p: fs::recursive_directory_iterator(scanPath)) {
        if (p.is_regular_file()) regexOneFile(p, rules, findings);
    }
    output << std::setw(OUTPUT_INDENT) << findings << '\n';
    return true;
}

int regex(int argc, char* argv[]) {
    if (argc != 5) {
        cerr << usage;
        return 1;
    }
    if (!fs::exists(argv[2])) {
        cerr << "File or directory " << argv[2] << " does not exist.\n";
        return 1;
    }
    if (!fs::exists(argv[3])) {
        cerr << "File or directory " << argv[3] << " does not exist.\n";
        return 1;
    }
    ofstream out(argv[4]); //if this isnt present, print to stdout?
    if (out.fail()) {
        cerr << "File " << argv[4] << " cannot be opened.\n";
        return 1;
    }
    if (!regexScan(fs::path(argv[2]), fs::path(argv[3]), out)) {
        cerr << "Scan failed.\n";
        return 1;
    }
    out.close();
    return 0;
}