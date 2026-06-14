#pragma once

#include <filesystem>

#include "vram/Vram/Vram.hpp"

class VramImage {
public:
    static Vram load(const std::filesystem::path& path);
};
