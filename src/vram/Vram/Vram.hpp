#pragma once

#include <cstdint>
#include <expected>
#include <memory>
#include <string>

#include "vram/SegmentsTable/SegmentsTable.hpp"

enum class TranslationError {
    SegmentNotFound,
    PageNotFound,
    PageNotResident,
};

struct TranslationFailure {
    std::uint32_t virtualAddress;
    TranslationError error;
};

std::string describeTranslationError(
    std::uint32_t virtualAddress,
    TranslationError error
);

class Vram {
private:
    std::unique_ptr<SegmentsTable> _segmentsTable;

public:
    Vram(std::unique_ptr<SegmentsTable> segmentsTable);

    std::expected<std::uint32_t, TranslationError> virtualToPhysical(
        std::uint32_t vaddr
    ) const;
};