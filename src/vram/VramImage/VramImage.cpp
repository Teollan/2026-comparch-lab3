#include "vram/VramImage/VramImage.hpp"

#include <cstdint>
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "util/Csv/Csv.hpp"
#include "vram/PagesTable/PagesTable.hpp"
#include "vram/SegmentsTable/SegmentsTable.hpp"

namespace {

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

    return Vram(std::make_unique<SegmentsTable>(std::move(segments)));
}
