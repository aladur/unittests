#ifdef _WIN32
#include <windows.h>
#endif
#include "gtest/gtest.h"


int main(int argc, char *argv[])
{
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

