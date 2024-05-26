#include <gtest/gtest.h>
#include <string>

TEST(test_hello, my_first_test)
{
    std::string str{"Hello World"};

    EXPECT_EQ(str.substr(0U, 5U), "Hello");
    EXPECT_EQ(str.substr(6U, 5U), "World");
}

