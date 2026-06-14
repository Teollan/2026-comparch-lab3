#include "vram/PagesTable/PagesTable.hpp"

#include <gtest/gtest.h>

#include <vector>

namespace {

PagesTable makePagesTable() {
    std::vector<PageDescriptor> pages;
    pages.push_back(PageDescriptor{.isResident = true, .frame = 16});
    pages.push_back(PageDescriptor{.isResident = false, .frame = 0});
    pages.push_back(PageDescriptor{.isResident = true, .frame = 18});

    return PagesTable(std::move(pages));
}

}  // namespace

TEST(PagesTable, GetReturnsDescriptorForPageInRange) {
    PagesTable table = makePagesTable();

    const PageDescriptor* page = table.get(0);

    ASSERT_NE(page, nullptr);
    EXPECT_TRUE(page->isResident);
    EXPECT_EQ(page->frame, 16u);
}

TEST(PagesTable, GetPreservesNonResidentFlag) {
    PagesTable table = makePagesTable();

    const PageDescriptor* page = table.get(1);

    ASSERT_NE(page, nullptr);
    EXPECT_FALSE(page->isResident);
}

TEST(PagesTable, GetReturnsNullptrForPageBeyondSegment) {
    PagesTable table = makePagesTable();

    EXPECT_EQ(table.get(3), nullptr);
}
