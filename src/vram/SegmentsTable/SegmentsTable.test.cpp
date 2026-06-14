#include "vram/SegmentsTable/SegmentsTable.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <vector>

namespace {

SegmentsTable makeSegmentsTable() {
    std::vector<PageDescriptor> pages;
    pages.push_back(PageDescriptor{.isResident = true, .frame = 1});

    std::vector<SegmentDescriptor> segments;
    segments.push_back(
        SegmentDescriptor{
            .pagesTable = std::make_unique<PagesTable>(std::move(pages))
        }
    );

    return SegmentsTable(std::move(segments));
}

}  // namespace

TEST(SegmentsTable, GetReturnsDescriptorForSegmentInRange) {
    SegmentsTable table = makeSegmentsTable();

    const SegmentDescriptor* segment = table.get(0);

    ASSERT_NE(segment, nullptr);
    ASSERT_NE(segment->pagesTable, nullptr);
    EXPECT_NE(segment->pagesTable->get(0), nullptr);
}

TEST(SegmentsTable, GetReturnsNullptrForSegmentBeyondTable) {
    SegmentsTable table = makeSegmentsTable();

    EXPECT_EQ(table.get(1), nullptr);
}
