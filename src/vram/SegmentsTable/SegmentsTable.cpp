#include "vram/SegmentsTable/SegmentsTable.hpp"

#include <utility>

SegmentsTable::SegmentsTable(std::vector<SegmentDescriptor> entries) :
    _entries(std::move(entries)) {}

const SegmentDescriptor* SegmentsTable::get(std::uint16_t id) const {
    if (id >= _entries.size()) {
        return nullptr;
    }

    return &_entries[id];
}
