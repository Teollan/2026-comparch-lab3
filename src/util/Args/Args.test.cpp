#include "util/Args/Args.hpp"

#include <gtest/gtest.h>

#include <string>
#include <vector>

namespace {

class MockArgv {
public:
    explicit MockArgv(std::initializer_list<std::string> tokens) :
        _tokens(tokens) {
        for (auto& s : _tokens) {
            _ptrs.push_back(s.data());
        }
    }

    int argc() const {
        return static_cast<int>(_ptrs.size());
    }

    char** argv() {
        return _ptrs.data();
    }

private:
    std::vector<std::string> _tokens;
    std::vector<char*> _ptrs;
};

}  // namespace

TEST(Args, ParsesRequiredStringArg) {
    MockArgv input{"./app", "--input", "foo.bin"};
    Args args(input.argc(), input.argv());

    args.expectString("input", "i", true).parse();

    EXPECT_EQ(args.getString("input"), "foo.bin");
}

TEST(Args, ParsesShortFormStringArg) {
    MockArgv input{"./app", "-i", "foo.bin"};
    Args args(input.argc(), input.argv());

    args.expectString("input", "i", true).parse();

    EXPECT_EQ(args.getString("input"), "foo.bin");
}

TEST(Args, ParsesFlagLongForm) {
    MockArgv input{"./app", "--hex"};
    Args args(input.argc(), input.argv());

    args.expectFlag("hex", "x").parse();

    EXPECT_TRUE(args.getFlag("hex"));
}

TEST(Args, ParsesFlagShortForm) {
    MockArgv input{"./app", "-x"};
    Args args(input.argc(), input.argv());

    args.expectFlag("hex", "x").parse();

    EXPECT_TRUE(args.getFlag("hex"));
}

TEST(Args, UnsetFlagDefaultsToFalse) {
    MockArgv input{"./app"};
    Args args(input.argc(), input.argv());

    args.expectFlag("hex", "x").parse();

    EXPECT_FALSE(args.getFlag("hex"));
}

TEST(Args, OptionalStringDefaultsToEmpty) {
    MockArgv input{"./app"};
    Args args(input.argc(), input.argv());

    args.expectString("output", "o").parse();

    EXPECT_EQ(args.getString("output"), "");
}

TEST(Args, MixesStringsAndFlags) {
    MockArgv input{"./app", "-i", "in.bin", "-x", "-o", "out.txt"};
    Args args(input.argc(), input.argv());

    args.expectString("input", "i", true)
        .expectFlag("hex", "x")
        .expectString("output", "o")
        .parse();

    EXPECT_EQ(args.getString("input"), "in.bin");
    EXPECT_TRUE(args.getFlag("hex"));
    EXPECT_EQ(args.getString("output"), "out.txt");
}

TEST(Args, OrderOfArgsDoesNotMatter) {
    MockArgv input{"./app", "-o", "out.txt", "--hex", "--input", "in.bin"};
    Args args(input.argc(), input.argv());

    args.expectString("input", "i", true)
        .expectFlag("hex", "x")
        .expectString("output", "o")
        .parse();

    EXPECT_EQ(args.getString("input"), "in.bin");
    EXPECT_TRUE(args.getFlag("hex"));
    EXPECT_EQ(args.getString("output"), "out.txt");
}

TEST(Args, ThrowsOnMissingRequired) {
    MockArgv input{"./app"};
    Args args(input.argc(), input.argv());

    args.expectString("input", "i", true);

    EXPECT_THROW(args.parse(), ArgsException);
}

TEST(Args, ThrowsOnUnknownArg) {
    MockArgv input{"./app", "--bogus"};
    Args args(input.argc(), input.argv());

    EXPECT_THROW(args.parse(), ArgsException);
}

TEST(Args, ThrowsOnMissingValueForStringArg) {
    MockArgv input{"./app", "--input"};
    Args args(input.argc(), input.argv());

    args.expectString("input", "i", true);

    EXPECT_THROW(args.parse(), ArgsException);
}

TEST(Args, GetStringThrowsOnUnknownName) {
    MockArgv input{"./app"};
    Args args(input.argc(), input.argv());

    args.parse();

    EXPECT_THROW(args.getString("never-registered"), std::runtime_error);
}

TEST(Args, GetFlagThrowsOnUnknownName) {
    MockArgv input{"./app"};
    Args args(input.argc(), input.argv());

    args.parse();

    EXPECT_THROW(args.getFlag("never-registered"), std::runtime_error);
}

TEST(Args, LaterValueOverridesEarlier) {
    MockArgv input{"./app", "-i", "first.bin", "-i", "second.bin"};
    Args args(input.argc(), input.argv());

    args.expectString("input", "i", true).parse();

    EXPECT_EQ(args.getString("input"), "second.bin");
}
