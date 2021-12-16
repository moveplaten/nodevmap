#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

#include <sys/stat.h>
#include <time.h>
#include <iostream>
#include <locale>
#include <string>
#include <codecvt>

class NvpUtil
{
public:
    static std::string getExeFilePath();
    static void writeExePath(const char* buf, uint64_t len, const char* name, const char* mode);

    struct FileRead
    {
        FileRead() : buf(nullptr), len(0) {}
        FileRead(const char* _buf, uint64_t _len) : buf(_buf), len(_len) {}
        ~FileRead() { if (buf) delete[](buf); }

        const char* buf;
        uint64_t len;
    };
    static FileRead readExePath(const char* name, const char* mode);

    static std::u16string utf8_to_utf16(const std::string& str);
    static std::u32string utf8_to_utf32(const std::string& str);

    static std::string utf16_to_utf8(const std::u16string& str);
    static std::string utf32_to_utf8(const std::u32string& str);

    typedef long long SysClockTick;
    static SysClockTick getSystemClockTick();
    static SysClockTick getSysTickInterval(int channel);
    static double getFpsByChannel(int channel);
};
