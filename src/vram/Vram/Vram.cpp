#include "vram/Vram/Vram.hpp"

#include <format>
#include <string>
#include <utility>

namespace {

const std::uint32_t SEGMENT_ID_MASK = 0xFFF00000;
const std::uint8_t SEGMENT_ID_SIZE = 12;

const std::uint32_t PAGE_ID_MASK = 0x000FFC00;
const std::uint8_t PAGE_ID_SIZE = 10;

const std::uint32_t OFFSET_MASK = 0x000003FF;
const std::uint8_t OFFSET_SIZE = 10;

}  // namespace

Vram::Vram(std::unique_ptr<SegmentsTable> segmentsTable) :
    _segmentsTable(std::move(segmentsTable)) {}

std::expected<std::uint32_t, TranslationError> Vram::virtualToPhysical(
    std::uint32_t vaddr
) const {
    std::uint16_t segmentId =
        (vaddr & SEGMENT_ID_MASK) >> (PAGE_ID_SIZE + OFFSET_SIZE);
    std::uint16_t pageId = (vaddr & PAGE_ID_MASK) >> OFFSET_SIZE;
    std::uint16_t offset = (vaddr & OFFSET_MASK);

    auto segment = _segmentsTable->get(segmentId);
    if (!segment) {
        return std::unexpected(TranslationError::SegmentNotFound);
    }

    auto page = segment->pagesTable->get(pageId);
    if (!page) {
        return std::unexpected(TranslationError::PageNotFound);
    }

    if (!page->isResident) {
        return std::unexpected(TranslationError::PageNotResident);
    }

    return (page->frame << OFFSET_SIZE) | offset;
}

std::string describeTranslationError(
    std::uint32_t virtualAddress,
    TranslationError error
) {
    std::uint16_t segmentId =
        (virtualAddress & SEGMENT_ID_MASK) >> (PAGE_ID_SIZE + OFFSET_SIZE);
    std::uint16_t pageId = (virtualAddress & PAGE_ID_MASK) >> OFFSET_SIZE;

    std::string reason;
    switch (error) {
        case TranslationError::SegmentNotFound:
            reason = std::format("segment [{}] doesn't exist", segmentId);
            break;
        case TranslationError::PageNotFound:
            reason = std::format("page [{}] doesn't exist", pageId);
            break;
        case TranslationError::PageNotResident:
            reason = std::format("page [{}] is not resident", pageId);
            break;
    }

    return std::format(
        "failed to convert virtual address [0x{:08X}] because {}",
        virtualAddress,
        reason
    );
}
