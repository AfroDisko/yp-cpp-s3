#pragma once

#include <algorithm>
#include <functional>
#include <iterator>
#include <utility>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {

inline auto YearBetween(int first, int last) {
    return [first, last](const Book &book) { return book.year >= first && book.year <= last; };
}

inline auto RatingAbove(double threshold) {
    return [threshold](const Book &book) { return book.rating > threshold; };
}

inline auto GenreIs(Book::Genre genre) {
    return [genre](const Book &book) { return book.genre == genre; };
}

template <BookPredicate... PredicatesT>
auto all_of(PredicatesT &&...predicates) {
    return [&predicates...](const Book &book) { return (... && std::forward<PredicatesT>(predicates)(book)); };
}

template <BookPredicate... PredicatesT>
auto any_of(PredicatesT &&...predicates) {
    return [&predicates...](const Book &book) { return (... || std::forward<PredicatesT>(predicates)(book)); };
}

template <BookIterator IteratorT, BookPredicate PredicateT>
auto filterBooks(IteratorT begin, IteratorT end, PredicateT &&predicate) {
    std::vector<std::reference_wrapper<const Book>> sample;
    sample.reserve(std::count_if(begin, end, predicate));

    std::copy_if(begin, end, std::back_inserter(sample), predicate);

    return sample;
}

}  // namespace bookdb
