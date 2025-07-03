#pragma once

#include <format>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

namespace bookdb {

struct Book {
    enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

    static constexpr std::string_view strFiction = "Fiction";
    static constexpr std::string_view strNonFiction = "NonFiction";
    static constexpr std::string_view strSciFi = "SciFi";
    static constexpr std::string_view strBiography = "Biography";
    static constexpr std::string_view strMystery = "Mystery";
    static constexpr std::string_view strUnknonw = "Unknown";

    static constexpr std::string genreToString(Genre genre) {
        static const std::unordered_map<Genre, std::string_view> mapping = {
            {Genre::Fiction, strFiction},     {Genre::NonFiction, strNonFiction}, {Genre::SciFi, strSciFi},
            {Genre::Biography, strBiography}, {Genre::Mystery, strMystery},       {Genre::Unknown, strUnknonw}};

        auto it = mapping.find(genre);
        if (it == mapping.end()) {
            throw std::runtime_error(std::format("unexpected genre enum {}", static_cast<int>(genre)));
        }
        return std::string(it->second);
    }

    static constexpr Genre stringToGenre(std::string_view string) {
        static const std::unordered_map<std::string_view, Genre> mapping = {
            {strFiction, Genre::Fiction},     {strNonFiction, Genre::NonFiction}, {strSciFi, Genre::SciFi},
            {strBiography, Genre::Biography}, {strMystery, Genre::Mystery},       {strUnknonw, Genre::Unknown}};

        auto it = mapping.find(string);
        if (it == mapping.end()) {
            throw std::runtime_error(std::format("unexpected genre string {}", string));
        }
        return it->second;
    }

    constexpr Book() = default;
    constexpr Book(Genre genre) : genre(genre) {}
    constexpr Book(std::string_view genre) : genre(stringToGenre(genre)) {}
    constexpr Book(std::string_view title, std::string_view author, int year, Genre genre, double rating,
                   int read_count)
        : genre(genre), title(title), author(author), year(year), read_count(read_count), rating(rating) {}

    bool operator==(const Book &) const = default;

    Genre genre = Genre::Unknown;
    std::string title;
    std::string_view author;

    int year = 0;
    int read_count = 0;
    double rating = 0.;
};

}  // namespace bookdb

template <>
struct std::formatter<bookdb::Book::Genre> : std::formatter<std::string> {
    auto format(bookdb::Book::Genre genre, std::format_context &ctx) const {
        return std::formatter<std::string>::format(bookdb::Book::genreToString(genre), ctx);
    }
};

template <>
struct std::formatter<bookdb::Book> : std::formatter<std::string> {
    auto format(const bookdb::Book &book, std::format_context &ctx) const {
        static constexpr std::string_view fmt =
            "[Genre: {}, Title: {}, Author: {}, Year: {}, Read count: {}, Rating: {}]";
        return std::formatter<std::string>::format(
            std::format(fmt, book.genre, book.title, book.author, book.year, book.read_count, book.rating), ctx);
    }
};
