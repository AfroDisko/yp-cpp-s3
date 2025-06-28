#include <gtest/gtest.h>
#include <ostream>

#include "book_database.hpp"

using namespace bookdb;

TEST(TestBookDatabase, Ctor) {
    Book book1(Book::Genre::Fiction);
    Book book2(Book::Genre::NonFiction);
    Book book3(Book::Genre::SciFi);

    BookDatabase db({book1, book2, book3});
    const auto &books = db.GetBooks();

    ASSERT_EQ(books[0], book1);
    ASSERT_EQ(books[1], book2);
    ASSERT_EQ(books[2], book3);
}

TEST(TestBookDatabase, EmptyDb) {
    BookDatabase db;

    ASSERT_EQ(db.size(), 0);
    ASSERT_TRUE(db.empty());
    ASSERT_TRUE(db.GetBooks().empty());
    ASSERT_TRUE(db.GetAuthors().empty());
    ASSERT_TRUE(db.GetAuthorsContainer().empty());

    for (const auto &book : db.GetBooks()) {
        ASSERT_TRUE(false);
        std::println("{}", book);
    }
    for (const auto &author : db.GetAuthors()) {
        ASSERT_TRUE(false);
        std::println("{}", author);
    }
}

TEST(TestBookDatabase, PushBack) {
    Book book1(Book::Genre::Fiction);
    Book book2(Book::Genre::NonFiction);
    Book book3(Book::Genre::SciFi);

    BookDatabase db;
    db.PushBack(book1);

    ASSERT_EQ(db.GetBooks()[0], book1);
}

TEST(TestBookDatabase, EmplaceBack) {
    Book book1(Book::Genre::Fiction);
    Book book2(Book::Genre::NonFiction);
    Book book3(Book::Genre::SciFi);

    BookDatabase db;
    db.EmplaceBack(book1.genre);

    ASSERT_EQ(db.GetBooks()[0], book1);
}

TEST(TestBookDatabase, AuthorCache) {
    Book book1(Book::Genre::Fiction);
    book1.author = "author 1";
    Book book2(Book::Genre::NonFiction);
    book2.author = "author 1";
    Book book3(Book::Genre::SciFi);
    book3.author = "author 2";

    BookDatabase db({book1, book2, book3});

    ASSERT_EQ(db.size(), 3);
    ASSERT_EQ(db.GetAuthors().size(), 2);
}

TEST(TestBookDatabase, Formatter) {
    Book book1(Book::Genre::Fiction);
    Book book2(Book::Genre::NonFiction);
    Book book3(Book::Genre::SciFi);

    BookDatabase db({book1, book2, book3});

    std::println(std::cout, "{}", db);
}
