#pragma once

#include "book.hpp"
#include <tuple>
#include <type_traits>

namespace bookdb::comp {

struct LessByGenre {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const {
        return static_cast<int>(lhs.genre) < static_cast<int>(rhs.genre);
    }
};

struct LessByTitle {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.title < rhs.title; }
};

struct LessByAuthor {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.author < rhs.author; }
};

struct LessByYear {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.year < rhs.year; }
};

struct LessByReadCount {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.read_count < rhs.read_count; }
};

struct LessByRating {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.rating < rhs.rating; }
};

struct LessByGenreRating {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const {
        return std::forward_as_tuple(lhs.genre, lhs.rating) < std::forward_as_tuple(rhs.genre, rhs.rating);
    }
};

}  // namespace bookdb::comp
