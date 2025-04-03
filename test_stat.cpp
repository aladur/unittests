#include <gtest/gtest.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

TEST(test_stat, fct_stat)
{
    const auto *filename{u8"statfile.txt"};
    const auto path = fs::temp_directory_path() / filename;
    struct stat sbuf{};

    std::ofstream ofs(path);
    ASSERT_TRUE(ofs.is_open());
    ofs << "abc";
    ofs.close();

    ASSERT_TRUE(stat(path.u8string().c_str(), &sbuf) == 0);
    fs::remove(path);
}

TEST(test_stat, fct_stat_utf8_path)
{
    const auto *filename{u8"statfile\u2665.txt"};
    const auto path = fs::temp_directory_path() / filename;
    struct stat sbuf{};

    std::ofstream ofs(path);
    ASSERT_TRUE(ofs.is_open());
    ofs << "abc";
    ofs.close();

    ASSERT_TRUE(stat(path.u8string().c_str(), &sbuf) == 0);
    fs::remove(path);
}

