#include <vector>
#include <unordered_set>
#include <iostream> //debug
#include <nlohmann/json.hpp>
#include "rule.hpp"
#include "pattern.hpp"
#include "languages.hpp"

using std::string;
using json = nlohmann::json;

/* JSON SCHEMA
{
    "name" : "required",
    "description" : "required",
    "languages" : [
        "required"
    ],
    "pattern" : { //required pattern object
        "regex" : "required",
        "modifiers" : [
            "optional"
        ]
    },
    "antipattern" : { //optional pattern object
}
*/

Rule::Rule(const json& obj) 
: name(obj["name"]), description(obj["description"]) {
    for (auto& p: obj["languages"]) {
        if (languageMap.count(p)) {
            extensions.insert(languageMap.at(p).begin(), languageMap.at(p).end());
        } else {
            extensions.insert((string) p);
        }
    }
    try {
        obj["pattern"].contains("modifiers") ?
            pattern = Pattern(obj["pattern"]["regex"], obj["pattern"]["modifiers"])
            : pattern = Pattern(obj["pattern"]["regex"]);
        if (obj.contains("antipattern")) {
            obj["antipattern"].contains("modifiers") ?
                antipattern = Pattern(obj["antipattern"]["regex"], obj["antipattern"]["modifiers"])
                : antipattern = Pattern(obj["antipattern"]["regex"]);
        }
    } catch (const char* e) { throw; }
}