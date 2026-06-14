#pragma once

#include <string>
#include <vector>

class String {
public:
    String() = delete;

    static std::vector<std::string> split(std::string str, char delimiter);
};
