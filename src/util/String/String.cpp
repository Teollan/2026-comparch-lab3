#include "util/String/String.hpp"

#include <utility>

std::vector<std::string> String::split(std::string str, char delimiter) {
    std::vector<std::string> parts;
    std::size_t start = 0;

    while (true) {
        std::size_t pos = str.find(delimiter, start);

        if (pos == std::string::npos) {
            parts.push_back(str.substr(start));

            break;
        }

        parts.push_back(str.substr(start, pos - start));
        start = pos + 1;
    }

    return parts;
}
