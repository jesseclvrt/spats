#include <iostream>
#include <filesystem> // C++17
#include <string>
#include <map>
#include <algorithm> // transform
#include <cctype> // tolower
#include "regex.hpp"
#include "verify.hpp"

using std::string;
using std::cerr;

namespace {
    const string intro = "SPATS - Static Program Analysis Tools\n";
    const string usage = string("Usage: ")
                            + program_invocation_short_name
                            + " TOOL [TOOL_OPTIONS]...\n"
                            + "Try " + program_invocation_short_name + " help "
                            + "for more information.\n";
}

static void printUsage() {
    cerr << intro << usage;
}

static int help(int, char**) {
    printUsage();
    cerr << "Tools: verify, regex, help\n";
    return 0;
}

static const std::map<std::string, int(*)(int, char**)> toolMap = { // C++11
    { "verify", verify },
    { "regex", regex },
    { "help", help }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "No tool specified.\n";
        printUsage();
        exit(1);
    }
    string tool = argv[1];
    try {
        std::transform(tool.begin(), tool.end(),
            tool.begin(), [](unsigned char c){ return std::tolower(c); });
        return toolMap.at(tool)(argc, argv);
    } catch (const std::out_of_range) {
        cerr << tool << " is not a tool.\n";
        printUsage();
        exit(1);
    }
}