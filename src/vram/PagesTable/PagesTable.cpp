#include "vram/PagesTable/PagesTable.hpp"

#include <utility>

PagesTable::PagesTable(std::vector<PageDescriptor> entries) :
    _entries(std::move(entries)) {}

const PageDescriptor* PagesTable::get(std::uint16_t id) const {
    if (id >= _entries.size()) {
        return nullptr;
    }

    return &_entries[id];
}
