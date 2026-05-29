#include "Hex.hpp"

#include <format>
#include <stdexcept>

namespace {

int hexDigitValue(char nibble) {
    if (nibble >= '0' && nibble <= '9') {
        return nibble - '0';
    }

    if (nibble >= 'A' && nibble <= 'F') {
        return nibble - 'A' + 10;
    }

    if (nibble >= 'a' && nibble <= 'f') {
        return nibble - 'a' + 10;
    }

    return -1;
}

}  // namespace

std::vector<std::uint8_t> hexToBytes(std::string_view hex) {
    std::vector<std::uint8_t> result;
    int high = -1;

    for (char nibble : hex) {
        int value = hexDigitValue(nibble);

        if (value < 0) {
            continue;
        }

        if (high < 0) {
            high = value;
            continue;
        }

        result.push_back(static_cast<std::uint8_t>((high << 4) | value));
        high = -1;
    }

    if (high >= 0) {
        throw std::invalid_argument("hexToBytes: odd number of hex digits");
    }

    return result;
}

std::string bytesToHex(const std::vector<std::uint8_t> &bytes) {
    std::string result;

    for (std::size_t i = 0; i < bytes.size(); i++) {
        result += std::format("{:02X} ", bytes[i]);
    }

    return result;
}
