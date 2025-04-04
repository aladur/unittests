#include <gtest/gtest.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <filesystem>

#ifdef _WIN32
#define stat _stat64i32
#endif

namespace fs = std::filesystem;

TEST(test_stat, fct_stat)
{
    const auto path = fs::temp_directory_path() / fs::u8path("statfile.txt");
    struct stat sbuf{};

    std::ofstream ofs(path);
    ASSERT_TRUE(ofs.is_open());
    ofs << "abc";
    ofs.close();
#ifdef _WIN32
    const auto result = _wstat(path.wstring().c_str(), &sbuf);
#else
    const auto result = stat(path.u8string().c_str(), &sbuf);
#endif
    EXPECT_EQ(errno, 0) << "path=" << path.u8string();
    EXPECT_EQ(result, 0) << "path=" << path.u8string();
    fs::remove(path);
}

TEST(test_stat, fct_stat_utf8_path)
{
    const auto path = fs::temp_directory_path() /
        fs::u8path(u8"statfile\u2665.txt");
    struct stat sbuf{};

    std::ofstream ofs(path);
    ASSERT_TRUE(ofs.is_open());
    ofs << "abc";
    ofs.close();

#ifdef _WIN32
    const auto result = _wstat(path.wstring().c_str(), &sbuf);
#else
    const auto result = stat(path.u8string().c_str(), &sbuf);
#endif
    EXPECT_EQ(errno, 0) << "path=" << path.u8string();
    EXPECT_EQ(result, 0) << "path=" << path.u8string();
    fs::remove(path);
}

