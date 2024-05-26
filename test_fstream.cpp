#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

// Similar issue with fstream.write():
// https://stackoverflow.com/questions/64884491/why-stdbasic-fstreamunsigned-char-wont-work

using stype =  unsigned char; // This one fails
//using stype =  char; // This one works
using bfstream = std::basic_fstream<stype>;

TEST(test_fstream, test_read_unsigned_char)
{
    std::string test_file("test.bin");
    std::fstream ofs(test_file, std::ios::out | std::ios::binary);
    ASSERT_TRUE(ofs.is_open());
    ofs << '\x01' << '\x02' << '\x03' << '\x04' << '\x05' << '\x06';
    ofs.close();
    std::ios_base::openmode mode = std::ios_base::in | std::ios_base::binary;
    bfstream fstream(test_file, mode);
    ASSERT_TRUE(fstream.is_open());

    stype buffer[1];
    fstream.read(&buffer[0], sizeof(buffer));
    EXPECT_FALSE(fstream.fail());
    EXPECT_EQ(fstream.gcount(), sizeof(buffer));

    std::filesystem::remove(test_file);
}

