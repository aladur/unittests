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
    const auto path = fs::temp_directory_path() / fs::u8path("accessfile.txt");

    std::ofstream ofs(path);
    ASSERT_TRUE(ofs.is_open());
    ofs << "abc";
    ofs.close();

#ifdef _WIN32
    const auto result = _waccess(path.wstring().c_str(), W_OK);
#else
    const auto result = access(path.u8string().c_str(), W_OK);
#endif
    EXPECT_EQ(errno, 0) << "path=" << path.u8string();
    EXPECT_EQ(result, 0) << "path=" << path.u8string();
    fs::remove(path);
}

TEST(test_access, fct_access_utf8_path)
{
    const auto path = fs::temp_directory_path() /
        fs::u8path(u8"accessfile\u2665.txt");

    std::ofstream ofs(path);
    ASSERT_TRUE(ofs.is_open());
    ofs << "abc";
    ofs.close();

#ifdef _WIN32
    const auto result = _waccess(path.wstring().c_str(), W_OK);
#else
    const auto result = access(path.u8string().c_str(), W_OK);
#endif
    EXPECT_EQ(errno, 0) << "path=" << path.u8string();
    EXPECT_EQ(result, 0) << "path=" << path.u8string();
    fs::remove(path);
}

