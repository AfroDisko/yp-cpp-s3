#include <gtest/gtest.h>

#include <functional>

#include "heterogeneous_lookup.hpp"

using namespace std::string_literals;
using namespace std::string_view_literals;

using namespace bookdb;

TEST(HetLookup, TransparentStringLess) {
    static_assert(TransparentStringLess{}("a"s, "b"s));
    static_assert(TransparentStringLess{}("a"sv, "b"sv));
    static_assert(TransparentStringLess{}("a"s, "b"sv));
    static_assert(TransparentStringLess{}("a"sv, "b"s));
}

TEST(HetLookup, TransparentStringEqual) {
    static_assert(TransparentStringEqual{}("a"s, "a"s));
    static_assert(TransparentStringEqual{}("a"sv, "a"sv));
    static_assert(TransparentStringEqual{}("a"s, "a"sv));
    static_assert(TransparentStringEqual{}("a"sv, "a"s));
}

TEST(HetLookup, TransparentStringHash) {
    ASSERT_EQ(TransparentStringHash{}("a"s), std::hash<std::string>{}("a"));
    ASSERT_EQ(TransparentStringHash{}("a"sv), std::hash<std::string_view>{}("a"));
}
