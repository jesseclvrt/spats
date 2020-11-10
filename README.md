# SPATs
**Static Program Analysis Tools (SPATs)** is a framework that allows you to create rules to search through code. This was created for finding security vulnerabilities, but don't let that stop you from using it for code-querying tasks. SPATs aims to be portable, fast, easy to write rules for, and simple to build. Rules are simply written in json, so it's easy to write new rules using a text editor.

## Tools
* verify - ensures your regex rules are properly formatted.
* regex - finds code by a simple regex search. This is a quick way to find interesting parts of code, even if it has a high false positive/negative rate. This is best suited for finding interesting function calls, e.g. "gets("
* (Planned) clang-ast - finds C, C++, and Objective-C code that match specified abstract syntax trees (ASTs). ASTs work regardless of source code noise (whitespace, variable names) and allow you to write rules that are deadly accurate. ASTs come with a performance cost due to partial compilation. See Clang's ASTMatching documentation to learn how to write these rules.
* (Planned) inf - finds infinite recursion cases using ASTs

## Regex rule schema
For those that like to learn from example, samples are included in the rules directory. All rule objects must be in an json array, even if your file only has 1 rule.

```
{
    "name" : "required",
    "description" : "required",
    "languages" : [
        "required"
    ],
    "pattern" : {"required"},
    "antipattern" : {"optional"}
}
```

name: A title for your rule.
description: Why your rule constitutes a finding.
languages: Languages the rule applies to. Currently accepted types are "c" and "c++". You can make it apply to any file extension by explicitly typing that extension, for example ".pem".
pattern: A pattern object that defines a finding. See below for more detail.
antipattern: A pattern object that, if matched, negates a finding. For example, a function call to ... is okay if you call ... first.

### Pattern schema
```
"pattern" : {
    "regex" : "required",
    "modifiers" : [
        "optional"
    ]
}
```

regex: the string used to define the regex.
modifiers: modifies how your regex operates, and sets the regex standard. "i" makes your regex case-insensitive. Supported regex standards are "ECMAScript", "basic" for basic POSIX, "extended" for extended POSIX, "awk", "grep", and "egrep". If a standard is not specified, ECMAScript is the default. Only one standard is allowed to be defined.

## Building from source
Install the following dependencies:
* (Planned) Clang 11 or higher
* C++17 compiler
* CMake
* make

### Building commands for linux
```
git clone
cd spats
mkdir build
cd build
cmake ..
make
```

### Building for Windows
I haven't tried, but I wouldn't expect any issues, since Clang has a Windows release.

## Who is the author?
Jesse Calvert wrote SPATs while working at security consultancy company.

## Special thanks
This wouldn't be possible without Niels Lohmann's json library. Great stuff!
SPATs was inspired by Microsoft's DevSkim tool, Facebook's pfff/sgrep, and Semmle. I wouldn't have made this tool if it wasn't for my experience with these amazing tools.