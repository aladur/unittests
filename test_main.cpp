#ifdef _WIN32
#include <windows.h>
#endif
#include "gtest/gtest.h"
#include <clocale>
#include <iostream>


int main(int argc, char *argv[])
{
#ifdef _WIN32
    std::cout << "GetACP()=" << GetACP() << "\n";
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "GetACP()=" << GetACP() << "\n";
#endif
    //const std::string prev_locale = std::setlocale(LC_CTYPE, "C.UTF-8");
    //std::cout << "prev_locale=" << prev_locale << "\n";
#ifdef _WIN32
    std::cout << "GetACP()=" << GetACP() << "\n";
#endif

    ::testing::InitGoogleTest(&argc, argv);

    const auto result = RUN_ALL_TESTS();

    //std::setlocale(LC_CTYPE, prev_locale.c_str());

    return result;
}
