#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#ifdef _WIN32
#include <sys/utime.h>
#else
#include <utime.h>
#endif

namespace fs = std::filesystem;

class test_utime : public ::testing::Test
{
protected:
    void createAndSetFileTime(const fs::path &path)
    {
        std::ofstream ofs(path);
        ASSERT_TRUE(ofs.is_open());
        ofs << "abc";
        ofs.close();

        struct tm file_time {};
        struct utimbuf timebuf {};

        file_time.tm_sec = 7;
        file_time.tm_min = 7;
        file_time.tm_hour = 7;
        file_time.tm_mon = 7;
        file_time.tm_mday = 7;
        file_time.tm_year = 77;
        file_time.tm_isdst = -1;
        timebuf.actime = mktime(&file_time);
        timebuf.modtime = mktime(&file_time);

        const auto result = utime(path.u8string().c_str(), &timebuf);
        EXPECT_EQ(errno, 0);
        ASSERT_EQ(result, 0);
        fs::remove(path);

    }
};

TEST_F(test_utime, fct_utime)
{
    const auto *filename{u8"utimefile.txt"};
    const auto path = fs::temp_directory_path() / filename;

    createAndSetFileTime(path);
}

TEST_F(test_utime, fct_utime_utf8_path)
{
    const auto *filename{u8"utimefile\u2665.txt"};
    const auto path = fs::temp_directory_path() / filename;

    createAndSetFileTime(path);
}

