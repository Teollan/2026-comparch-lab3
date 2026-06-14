#pragma once

#include <fstream>
#include <string>
#include <vector>

class Csv {
private:
    std::vector<std::vector<std::string>> _rows;

public:
    Csv(std::ifstream stream);

    const std::vector<std::vector<std::string>>& rows();
};
