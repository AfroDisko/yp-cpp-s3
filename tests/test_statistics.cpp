#include <gtest/gtest.h>
#include <list>
#include <stdexcept>

#include "book.hpp"
#include "statsistics.hpp"

using namespace bookdb;

TEST(TestStatistics, buildAuthorHistogramFlat) {
    Book book1(Book::Genre::Fiction);
    book1.author = "author 1";
    Book book2(Book::Genre::NonFiction);
    book2.author = "author 1";
    Book book3(Book::Genre::SciFi);
    book3.author = "author 2";

    BookDatabase db({book1, book2, book3});
    auto hist = buildAuthorHistogramFlat(db);

    ASSERT_EQ(hist.at("author 1"), 2);
    ASSERT_EQ(hist.at("author 2"), 1);
}

TEST(TestStatistics, calculateGenreRatings) {
    Book book1(Book::Genre::Fiction);
    book1.rating = 1.;
    Book book2(Book::Genre::Fiction);
    book2.rating = 5.;
    Book book3(Book::Genre::NonFiction);
    book3.rating = 10.;

    BookDatabase db({book1, book2, book3});
    auto hist = calculateGenreRatings(db);

    ASSERT_EQ(hist.at(Book::Genre::Fiction), 3.);
    ASSERT_EQ(hist.at(Book::Genre::NonFiction), 10.);
}

TEST(TestStatistics, sampleRandomBooks) {
    Book book1(Book::Genre::Fiction);
    Book book2(Book::Genre::Fiction);
    Book book3(Book::Genre::NonFiction);

    BookDatabase db({book1, book2, book3});
    auto books = sampleRandomBooks(db, 2);

    ASSERT_EQ(books.size(), 2);
}

TEST(TestStatistics, sampleRandomBooksThrow) {
    Book book1(Book::Genre::Fiction);
    Book book2(Book::Genre::Fiction);
    Book book3(Book::Genre::NonFiction);

    BookDatabase db({book1, book2, book3});
    ASSERT_THROW(sampleRandomBooks(db, 10), std::runtime_error);
}

TEST(TestStatistics, getTopNBy) {
    Book book1(Book::Genre::Fiction);
    book1.rating = -1.;
    Book book2(Book::Genre::Fiction);
    book2.rating = 10.;
    Book book3(Book::Genre::NonFiction);
    book3.rating = -10.;

    BookDatabase db({book1, book2, book3});
    auto books = getTopNBy(db, 2);

    ASSERT_EQ(books.size(), 2);
    ASSERT_EQ(books[0], book2);
    ASSERT_EQ(books[1], book1);
}

TEST(TestStatistics, getTopNByNonContiguousContainer) {
    Book book1(Book::Genre::Fiction);
    book1.rating = -1.;
    Book book2(Book::Genre::Fiction);
    book2.rating = 10.;
    Book book3(Book::Genre::NonFiction);
    book3.rating = -10.;

    BookDatabase<std::list<Book>> db({book1, book2, book3});
    auto books = getTopNBy(db, 2);

    ASSERT_EQ(books.size(), 2);
    ASSERT_EQ(books[0], book2);
    ASSERT_EQ(books[1], book1);
}
