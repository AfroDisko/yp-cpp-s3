#include <gtest/gtest.h>

#include "book_database.hpp"
#include "filters.hpp"

using namespace bookdb;

TEST(TestFilters, YearBetween) {
    Book book1;
    book1.year = 100;
    Book book2;
    book2.year = 1000;

    BookDatabase db({book1, book2});
    auto books = filterBooks(db.begin(), db.end(), YearBetween(101, 1001));

    ASSERT_EQ(books.size(), 1);
    ASSERT_EQ(books[0], book2) << std::format("{}\n", books[0].get());
}

TEST(TestFilters, RatingAbove) {
    Book book1;
    book1.rating = 10.;
    Book book2;
    book2.rating = -10.;

    BookDatabase db({book1, book2});
    auto books = filterBooks(db.begin(), db.end(), RatingAbove(0.));

    ASSERT_EQ(books.size(), 1);
    ASSERT_EQ(books[0], book1) << std::format("{}\n", books[0].get());
}

TEST(TestFilters, GenreIs) {
    Book book1(Book::Genre::Biography);
    Book book2(Book::Genre::Mystery);

    BookDatabase db({book1, book2});
    auto books = filterBooks(db.begin(), db.end(), GenreIs(Book::Genre::Mystery));

    ASSERT_EQ(books.size(), 1);
    ASSERT_EQ(books[0], book2) << std::format("{}\n", books[0].get());
}

TEST(TestFilters, all_of) {
    Book book1(Book::Genre::Biography);
    book1.rating = 1.;
    Book book2(Book::Genre::Biography);
    book2.rating = 2.;
    Book book3(Book::Genre::Fiction);
    book3.rating = 3.;

    BookDatabase db({book1, book2, book3});
    auto books = filterBooks(db.begin(), db.end(), all_of(GenreIs(Book::Genre::Biography), RatingAbove(1.5)));

    ASSERT_EQ(books.size(), 1);
    ASSERT_EQ(books[0], book2) << std::format("{}\n", books[0].get());
}

TEST(TestFilters, any_of) {
    Book book1(Book::Genre::Biography);
    book1.rating = 1.;
    Book book2(Book::Genre::Biography);
    book2.rating = 2.;
    Book book3(Book::Genre::Fiction);
    book3.rating = 3.;

    BookDatabase db({book1, book2, book3});
    auto books = filterBooks(db.begin(), db.end(), any_of(GenreIs(Book::Genre::Biography), RatingAbove(1.5)));

    ASSERT_EQ(books.size(), 3);
    ASSERT_EQ(books[0], book1) << std::format("{}\n", books[0].get());
    ASSERT_EQ(books[1], book2) << std::format("{}\n", books[1].get());
    ASSERT_EQ(books[2], book3) << std::format("{}\n", books[2].get());
}
