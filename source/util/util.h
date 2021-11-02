#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef __APPLE__
#include <time.h>
#endif

#include <iostream>
#include <locale>
#include <string>
#include <codecvt>

typedef long long SysClockTick;

class NvpUtil
{
public:
    std::u16string utf8_to_utf16(const std::string& str);
    
    std::u32string utf8_to_utf32(const std::string& str);

    SysClockTick getSystemClockTick();
    SysClockTick getSysTickInterval(int channel);
    double getFpsByChannel(int channel);
};


/// GLOBAL nvpUtil;
extern NvpUtil* const nvpUtil;

