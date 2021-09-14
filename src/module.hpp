#pragma once
#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <dlfcn.h>

namespace fs = std::filesystem;

class Module {
    std::map<std::string, uint32_t> funcs;
    std::vector<void *> handlers;
    std::vector<uint32_t> stack;
public:
    Module();
    ~Module();

    void record(const std::string &func, uint32_t index);
    void scan(const std::string &folder);
    void execute(const std::string &name);

    void push(uint32_t data);
    void push(const std::vector<uint32_t> &data);
    uint32_t pop();

    std::istream & cin();
    std::ostream & cout();
};
