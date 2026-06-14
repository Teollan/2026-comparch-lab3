#include "util/Hex/Hex.hpp"

#include <cstddef>
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

    for (std::size_t i = 0; i < hex.size(); i++) {
        if (hex[i] == '/' && i + 1 < hex.size() && hex[i + 1] == '/') {
            while (i < hex.size() && hex[i] != '\n') {
                i++;
            }

            continue;
        }

        int value = hexDigitValue(hex[i]);

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

std::string bytesToHex(
    const std::vector<std::uint8_t>& bytes,
    std::size_t offset,
    std::size_t count
) {
    std::string result;

    for (std::size_t i = 0; i < count; i++) {
        result += std::format("{:02X} ", bytes[offset + i]);
    }

    return result;
}

std::string bytesToHex(const std::vector<std::uint8_t>& bytes) {
    return bytesToHex(bytes, 0, bytes.size());
}
