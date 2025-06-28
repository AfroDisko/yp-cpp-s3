#pragma once

#include <concepts>
#include <cstddef>
#include <iterator>
#include <ranges>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept has_reserve = requires(T t, std::size_t size) { t.reserve(size); };

template <typename T>
concept has_size = requires(T t) {
    { t.size() } -> std::convertible_to<std::size_t>;
};

template <typename T>
concept has_clear = requires(T t) { t.clear(); };

template <typename T>
concept BookContainerLike =
    std::same_as<typename T::value_type, Book> && std::ranges::range<T> && has_size<T> && has_clear<T>;

template <typename T>
concept BookIterator = std::bidirectional_iterator<T>;

template <typename S, typename I>
concept BookSentinel = std::sentinel_for<S, I> && BookIterator<I>;

template <typename P>
concept BookPredicate = std::predicate<P, const Book &> || std::predicate<P, Book &>;

template <typename C>
concept BookComparator = std::predicate<C, const Book &, const Book &>;

}  // namespace bookdb
