#include "vram/Vram/Vram.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <vector>

#include "vram/PagesTable/PagesTable.hpp"
#include "vram/SegmentsTable/SegmentsTable.hpp"

namespace {

std::uint32_t makeVaddr(
    std::uint16_t segment,
    std::uint16_t page,
    std::uint16_t offset
) {
    return (std::uint32_t(segment) << 20) | (std::uint32_t(page) << 10) |
           offset;
}

Vram makeVram() {
    std::vector<PageDescriptor> seg0Pages;
    seg0Pages.push_back(PageDescriptor{.isResident = true, .frame = 0x5});
    seg0Pages.push_back(PageDescriptor{.isResident = false, .frame = 0});
    seg0Pages.push_back(PageDescriptor{.isResident = true, .frame = 0x7});

    std::vector<PageDescriptor> seg1Pages;
    seg1Pages.push_back(PageDescriptor{.isResident = true, .frame = 0x9});

    std::vector<SegmentDescriptor> segments;
    segments.push_back(
        SegmentDescriptor{
            .pagesTable = std::make_unique<PagesTable>(std::move(seg0Pages))
        }
    );
    segments.push_back(
        SegmentDescriptor{
            .pagesTable = std::make_unique<PagesTable>(std::move(seg1Pages))
        }
    );

    return Vram(std::make_unique<SegmentsTable>(std::move(segments)));
}

}  // namespace

TEST(Vram, TranslatesResidentPage) {
    Vram vram = makeVram();

    auto result = vram.virtualToPhysical(makeVaddr(0, 0, 0xAB));

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, (std::uint32_t(0x5) << 10) | 0xAB);
}

TEST(Vram, PreservesOffsetWithinPage) {
    Vram vram = makeVram();

    auto result = vram.virtualToPhysical(makeVaddr(0, 2, 0x3FF));

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result & 0x3FF, 0x3FFu);
}

TEST(Vram, UnknownSegmentReturnsError) {
    Vram vram = makeVram();

    auto result = vram.virtualToPhysical(makeVaddr(2, 0, 0));

    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), TranslationError::SegmentNotFound);
}

TEST(Vram, PageBeyondSegmentReturnsError) {
    Vram vram = makeVram();

    auto result = vram.virtualToPhysical(makeVaddr(0, 3, 0));

    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), TranslationError::PageNotFound);
}

TEST(Vram, NonResidentPageReturnsError) {
    Vram vram = makeVram();

    auto result = vram.virtualToPhysical(makeVaddr(0, 1, 0));

    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), TranslationError::PageNotResident);
}
