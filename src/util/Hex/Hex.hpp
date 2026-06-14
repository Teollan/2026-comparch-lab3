#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

std::vector<std::uint8_t> hexToBytes(std::string_view hex);

std::string bytesToHex(const std::vector<std::uint8_t>& bytes);

std::string bytesToHex(
    const std::vector<std::uint8_t>& bytes,
    std::size_t offset,
    std::size_t count
);
