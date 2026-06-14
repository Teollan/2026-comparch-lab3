#pragma once

#include <cstdint>
#include <vector>

struct PageDescriptor {
    bool isResident;
    std::uint32_t frame;
};

class PagesTable {
private:
    std::vector<PageDescriptor> _entries;

public:
    PagesTable(std::vector<PageDescriptor> entries);

    const PageDescriptor* get(std::uint16_t id) const;
};
