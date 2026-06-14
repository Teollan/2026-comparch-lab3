#include "util/String/String.hpp"

#include <gtest/gtest.h>

using Parts = std::vector<std::string>;

TEST(split, Empty) {
    EXPECT_EQ(String::split("", ','), (Parts{""}));
}

TEST(split, NoDelimiter) {
    EXPECT_EQ(String::split("abc", ','), (Parts{"abc"}));
}

TEST(split, TwoParts) {
    EXPECT_EQ(String::split("a,b", ','), (Parts{"a", "b"}));
}

TEST(split, MultipleParts) {
    EXPECT_EQ(String::split("a,b,c,d", ','), (Parts{"a", "b", "c", "d"}));
}

TEST(split, LeadingDelimiter) {
    EXPECT_EQ(String::split(",a", ','), (Parts{"", "a"}));
}

TEST(split, TrailingDelimiter) {
    EXPECT_EQ(String::split("a,", ','), (Parts{"a", ""}));
}

TEST(split, ConsecutiveDelimiters) {
    EXPECT_EQ(String::split("a,,b", ','), (Parts{"a", "", "b"}));
}

TEST(split, OnlyDelimiters) {
    EXPECT_EQ(String::split(",,", ','), (Parts{"", "", ""}));
}

TEST(split, SingleDelimiter) {
    EXPECT_EQ(String::split(",", ','), (Parts{"", ""}));
}

TEST(split, NewlineDelimiter) {
    EXPECT_EQ(
        String::split("line1\nline2\n", '\n'), (Parts{"line1", "line2", ""})
    );
}

TEST(split, PreservesWhitespace) {
    EXPECT_EQ(String::split(" a , b ", ','), (Parts{" a ", " b "}));
}

TEST(split, PartCountIsDelimiterCountPlusOne) {
    EXPECT_EQ(String::split("a,b,c", ',').size(), 3u);
    EXPECT_EQ(String::split("a;b;c;d;e", ';').size(), 5u);
}
