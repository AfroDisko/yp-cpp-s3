#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <flat_map>
#include <format>
#include <functional>
#include <iterator>
#include <map>
#include <numeric>
#include <print>
#include <random>
#include <stdexcept>
#include <string_view>
#include <vector>

#include "book.hpp"
#include "book_database.hpp"
#include "comparators.hpp"
#include "concepts.hpp"

namespace bookdb {

template <BookContainerLike T, typename ComparatorT = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &db, ComparatorT &&comp = {}) {
    const auto &authors_container = db.GetAuthorsContainer();
    return std::flat_map<std::string_view, std::uint64_t, ComparatorT>(authors_container.begin(),
                                                                       authors_container.end(), std::move(comp));
}

template <BookContainerLike T, typename ComparatorT = TransparentStringLess>
auto buildAuthorHistogram(const BookDatabase<T> &db, ComparatorT &&comp = {}) {
    const auto &authors_container = db.GetAuthorsContainer();
    return std::map<std::string_view, std::uint64_t, ComparatorT>(authors_container.begin(), authors_container.end());
}

template <BookIterator IteratorT>
auto calculateGenreRatings(IteratorT begin, IteratorT end) {
    std::unordered_map<Book::Genre, std::pair<double, std::uint64_t>> ratings;

    auto update = [&ratings](const Book &book) {
        auto &pair = ratings[book.genre];
        pair.first += book.rating;
        pair.second += 1;
    };
    std::for_each(begin, end, update);

    std::flat_map<Book::Genre, double> hist;
    for (const auto &[genre, pair] : ratings) {
        hist.emplace(genre, pair.first / pair.second);
    }
    return hist;
}

template <BookContainerLike T>
auto calculateGenreRatings(const BookDatabase<T> &db) {
    return calculateGenreRatings(db.begin(), db.end());
}

template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T> &db) {
    static constexpr auto rating_sum = []<typename LhsT, typename RhsT>(const LhsT &lhs, const RhsT &rhs) {
        if constexpr (std::same_as<LhsT, Book> && std::same_as<RhsT, Book>) {
            return lhs.rating + rhs.rating;
        }
        if constexpr (std::same_as<LhsT, Book> && std::same_as<RhsT, double>) {
            return lhs.rating + rhs;
        }
        if constexpr (std::same_as<LhsT, double> && std::same_as<RhsT, Book>) {
            return lhs + rhs.rating;
        }
        if constexpr (std::same_as<LhsT, double> && std::same_as<RhsT, double>) {
            return lhs + rhs;
        }
    };

    if (db.empty()) {
        throw std::runtime_error("DB is empty");
    }

    return std::reduce(db.begin(), db.end(), 0., rating_sum) / db.size();
}

template <BookContainerLike T>
auto sampleRandomBooks(const BookDatabase<T> &db, std::size_t size) {
    static std::mt19937 rnd(std::random_device{}());

    if (db.size() < size) {
        throw std::runtime_error(std::format("DB size {} is less than sample size {}", db.size(), size));
    }

    std::vector<std::reference_wrapper<const Book>> sample;
    sample.reserve(size);
    std::sample(db.begin(), db.end(), std::back_inserter(sample), size, rnd);

    return sample;
}

template <BookContainerLike T, BookComparator ComparatorT = comp::LessByRating>
auto getTopNBy(BookDatabase<T> &db, std::size_t size, ComparatorT &&comparator = {}) {
    if (db.size() < size) {
        throw std::runtime_error(std::format("DB size {} is less than sample size {}", db.size(), size));
    }

    std::vector<std::reference_wrapper<const Book>> sample;
    sample.reserve(size);

    if constexpr (std::random_access_iterator<typename BookDatabase<T>::iterator>) {
        std::nth_element(db.begin(), std::prev(db.end(), size), db.end(), comparator);
        std::sort(std::prev(db.end(), size), db.end(), comparator);
        std::reverse_copy(std::prev(db.end(), size), db.end(), std::back_inserter(sample));
    } else {
        std::vector<std::reference_wrapper<const Book>> copy(db.begin(), db.end());
        std::nth_element(copy.begin(), std::prev(copy.end(), size), copy.end(), comparator);
        std::sort(std::prev(copy.end(), size), copy.end(), comparator);
        std::reverse_copy(std::prev(copy.end(), size), copy.end(), std::back_inserter(sample));
    }

    return sample;
}

}  // namespace bookdb
