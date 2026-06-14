#include "util/Csv/Csv.hpp"

#include "util/String/String.hpp"

Csv::Csv(std::ifstream stream) {
    std::string buffer = "";

    while (std::getline(stream, buffer)) {
        _rows.push_back(String::split(buffer, ','));
    }
}

const std::vector<std::vector<std::string>>& Csv::rows() {
    return _rows;
}
