#include "vram/VramImage/VramImage.hpp"

#include <cstddef>
#include <cstdint>
#include <format>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "util/Csv/Csv.hpp"
#include "vram/PagesTable/PagesTable.hpp"
#include "vram/SegmentsTable/SegmentsTable.hpp"

namespace {

const std::size_t MAX_SEGMENTS = 4096;
const std::size_t MAX_PAGES = 1024;

PagesTable loadPagesTable(const std::filesystem::path& path) {
    std::ifstream file(path);
    Csv csv(std::move(file));

    std::vector<PageDescriptor> pages;

    for (const auto& row : csv.rows()) {
        bool isResident = std::stoul(row.at(1)) > 0;
        std::uint32_t frame = std::stoul(row.at(2));

        pages.push_back(
            PageDescriptor{.isResident = isResident, .frame = frame}
        );
    }

    if (pages.size() > MAX_PAGES) {
        throw std::runtime_error(
            std::format(
                "pages table '{}' has {} entries, exceeding the {}-page limit",
                path.string(),
                pages.size(),
                MAX_PAGES
            )
        );
    }

    return PagesTable(std::move(pages));
}

}  // namespace

Vram VramImage::load(const std::filesystem::path& path) {
    std::ifstream file(path);
    Csv csv(std::move(file));

    std::vector<SegmentDescriptor> segments;

    for (const auto& row : csv.rows()) {
        segments.push_back(
            SegmentDescriptor{
                .pagesTable =
                    std::make_unique<PagesTable>(loadPagesTable(row.at(0)))
            }
        );
    }

    if (segments.size() > MAX_SEGMENTS) {
        throw std::runtime_error(
            std::format(
                "segments table '{}' has {} entries, exceeding the {}-segment "
                "limit",
                path.string(),
                segments.size(),
                MAX_SEGMENTS
            )
        );
    }

    return Vram(std::make_unique<SegmentsTable>(std::move(segments)));
}
