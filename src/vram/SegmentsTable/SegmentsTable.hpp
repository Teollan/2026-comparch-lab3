#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "vram/PagesTable/PagesTable.hpp"

struct SegmentDescriptor {
    std::unique_ptr<PagesTable> pagesTable;
};

class SegmentsTable {
private:
    std::vector<SegmentDescriptor> _entries;

public:
    SegmentsTable(std::vector<SegmentDescriptor> entries);

    const SegmentDescriptor* get(std::uint16_t id) const;
};
