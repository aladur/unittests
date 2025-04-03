#include <gtest/gtest.h>
#include <string>

struct ci_char_traits : public std::char_traits<char>
{
    static bool eq(char c1, char c2)
    {
        return tolower(c1) == tolower(c2);
    }

    static bool ne(char c1, char c2)
    {
        return tolower(c1) != tolower(c2);
    }

    static bool lt(char c1, char c2)
    {
        return tolower(c1) < tolower(c2);
    }

    static int compare(const char* c1, const char* c2, size_t n)
    {
        while (n-- != 0)
        {
            if (tolower(*c1) < tolower(*c2))
            {
                return -1;
            }
            if (tolower(*c1) > tolower(*c2))
            {
                return 1;
            }
            ++c1;
            ++c2;
        }
        return 0;
    }

    static const char* find(const char* s, int n, char a)
    {
        const char lc_a = static_cast<char>(tolower(a));

        while (n-- > 0 && tolower(*s) != lc_a)
        {
            ++s;
        }
        return s;
    }
};

typedef std::basic_string<char, ci_char_traits> ci_string;

TEST(test_fstream, test_read_unsigned_char)
{
    ci_string ci_str("AbcDEf");

    EXPECT_EQ(ci_str.compare("abcdef"), 0);
    EXPECT_EQ(ci_str.compare("ABCDEF"), 0);
    EXPECT_EQ(ci_str.compare("aBCdeF"), 0);
}



