#include <gtest/gtest.h>
#include <algorithm>
#include <string>
#include <ctype.h>


const char *white_space = " \t\n\r";

char tolower_value(char ch)
{
    if (ch >= 'A' && ch <= 'Z')
    {
        return static_cast<char>(ch - ('z' - 'Z'));
    }
    return ch;
}

char &tolower_ref(char &ch)
{
    if (ch >= 'A' && ch <= 'Z')
    {
        ch = static_cast<char>(ch + ('z' - 'Z'));
    }
    return ch;
}

std::string tolower(const std::string& src)
{
    std::string result;
    std::transform(src.cbegin(), src.cend(), std::back_inserter(result),
            tolower_value);

    return result;
}

std::string tolower(std::string&& src)
{
    std::string result = std::move(src);

    std::for_each(result.begin(), result.end(), tolower_ref);

    return result;
}

inline std::string rtrim(const std::string &str, const char* t = white_space)
{
    auto result(str);
    result.erase(str.find_last_not_of(t) + 1);
    return result;
}

inline std::string rtrim(std::string &&str, const char* t = white_space)
{
    str.erase(str.find_last_not_of(t) + 1);
    return str;
}

TEST(test_refref_param, test_refref_param1) 
{
    const char *token ="    AbCdEF   ";
    const auto result = rtrim(tolower(token));
    EXPECT_EQ(result, "    abcdef");
    std::string str1{"bcdef \t\n  "};
    str1 = rtrim(std::move(str1));
    EXPECT_EQ(str1, "bcdef");
}
