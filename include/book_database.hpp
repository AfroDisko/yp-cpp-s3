#pragma once

#include <cstddef>
#include <cstdint>
#include <format>
#include <initializer_list>
#include <iterator>
#include <print>
#include <ranges>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    using value_type = typename BookContainer::value_type;
    using allocator_type = typename BookContainer::allocator_type;
    using size_type = typename BookContainer::size_type;
    using reference = value_type &;
    using const_reference = const value_type &;
    using iterator = typename BookContainer::iterator;
    using const_iterator = typename BookContainer::const_iterator;

    using AuthorContainer =
        std::unordered_map<std::string, std::uint64_t, TransparentStringHash, TransparentStringEqual>;

    BookDatabase() = default;
    BookDatabase(std::initializer_list<Book>);

    const BookContainer &GetBooks() const { return books_; }
    auto GetAuthors() const { return authors_ | std::views::keys; }
    const AuthorContainer &GetAuthorsContainer() const { return authors_; }

    bool empty() const { return books_.empty(); }
    std::size_t size() const { return books_.size(); }

    void reserve(std::size_t size) {
        if constexpr (has_reserve<BookContainer>) {
            books_.reserve(size);
        }
    }

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    void PushBack(const Book &);
    void PushBack(Book &&);

    template <typename... ArgsT>
    void EmplaceBack(ArgsT &&...);

    auto begin() { return books_.begin(); }
    auto end() { return books_.end(); }

    auto begin() const { return books_.begin(); }
    auto end() const { return books_.end(); }

    auto cbegin() const { return books_.cbegin(); }
    auto cend() const { return books_.cend(); }

    auto rbegin() { return books_.rbegin(); }
    auto rend() { return books_.rend(); }

    auto rbegin() const { return books_.rbegin(); }
    auto rend() const { return books_.rend(); }

    auto crbegin() const { return books_.crbegin(); }
    auto crend() const { return books_.crend(); }

private:
    void addAuthor(Book &);

    BookContainer books_;
    AuthorContainer authors_;
};

template <BookContainerLike BookContainer>
BookDatabase<BookContainer>::BookDatabase(std::initializer_list<Book> list) {
    if constexpr (has_reserve<BookContainer>) {
        books_.reserve(list.size());
    }
    for (const Book &book : list) {
        books_.push_back(book);
        addAuthor(books_.back());
    }
}

template <BookContainerLike BookContainer>
void BookDatabase<BookContainer>::PushBack(const Book &book) {
    books_.push_back(book);
    addAuthor(books_.back());
}

template <BookContainerLike BookContainer>
void BookDatabase<BookContainer>::PushBack(Book &&book) {
    books_.push_back(std::move(book));
    addAuthor(books_.back());
}

template <BookContainerLike BookContainer>
template <typename... ArgsT>
void BookDatabase<BookContainer>::EmplaceBack(ArgsT &&...args) {
    books_.emplace_back(std::forward<ArgsT>(args)...);
    addAuthor(books_.back());
}

template <BookContainerLike BookContainer>
void BookDatabase<BookContainer>::addAuthor(Book &book) {
    if (auto it = authors_.find(book.author); it == authors_.end()) {
        auto [it_new, _] = authors_.emplace(book.author, 1);
        book.author = it_new->first;
    } else {
        it->second += 1;
        book.author = it->first;
    }
}

}  // namespace bookdb

template <bookdb::BookContainerLike BookContainer>
struct std::formatter<bookdb::BookDatabase<BookContainer>> : std::formatter<std::string> {
    auto format(const bookdb::BookDatabase<BookContainer> &db, std::format_context &ctx) const {
        static constexpr std::size_t reserve_per_entry = 512;

        std::string string;
        string.reserve(db.size() * reserve_per_entry);

        for (const bookdb::Book &book : db) {
            string += std::format("{}\n", book);
        }
        return std::formatter<std::string>::format(string, ctx);
    }
};
