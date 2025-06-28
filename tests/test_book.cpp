#include <gtest/gtest.h>
#include <ostream>
#include <stdexcept>

#include "book.hpp"

using namespace bookdb;

TEST(TestBook, Ctor) {
    ASSERT_NO_THROW(Book book(Book::Genre::Fiction));
    ASSERT_NO_THROW(Book book("Fiction"));
    ASSERT_THROW(Book book("unexpected"), std::runtime_error);
};

TEST(TestBook, Formatter) {
    Book book(Book::Genre::Fiction);
    std::println(std::cout, "{}", book);
};
