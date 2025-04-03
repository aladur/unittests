#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#ifdef _WIN32
#include <windows.h>
#define W_OK   (2)
#define R_OK   (4)
#else
#include <unistd.h>
#endif

namespace fs = std::filesystem;

TEST(test_access, fct_access)
{
    const auto *filename{u8"accessfile.txt"};
    const auto path = fs::temp_directory_path() / filename;

    std::ofstream ofs(path);
    ASSERT_TRUE(ofs.is_open());
    ofs << "abc";
    ofs.close();

    ASSERT_TRUE(access(path.u8string().c_str(), W_OK) == 0);
    fs::remove(path);
}

TEST(test_access, fct_access_utf8_path)
{
    const auto *filename{u8"accessfile\u2665.txt"};
    const auto path = fs::temp_directory_path() / filename;

    std::ofstream ofs(path);
    ASSERT_TRUE(ofs.is_open());
    ofs << "abc";
    ofs.close();

    ASSERT_TRUE(access(path.u8string().c_str(), W_OK) == 0);
    fs::remove(path);
}

