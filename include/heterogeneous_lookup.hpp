#pragma once

#include <concepts>
#include <string>
#include <string_view>

namespace bookdb {

namespace concepts {

template <typename T>
concept StringLike = std::convertible_to<T, std::string> || std::convertible_to<T, std::string_view>;

}

struct TransparentStringLess {
    using is_transparent = void;

    template <concepts::StringLike LhsT, concepts::StringLike RhsT>
    constexpr bool operator()(const LhsT &lhs, const RhsT &rhs) const {
        return lhs < rhs;
    }
};

struct TransparentStringEqual {
    using is_transparent = void;

    template <concepts::StringLike LhsT, concepts::StringLike RhsT>
    constexpr bool operator()(const LhsT &lhs, const RhsT &rhs) const {
        return lhs == rhs;
    }
};

struct TransparentStringHash {
    using is_transparent = void;

    template <concepts::StringLike String>
    std::size_t operator()(const String &str) const {
        return std::hash<String>{}(str);
    }
};

}  // namespace bookdb
