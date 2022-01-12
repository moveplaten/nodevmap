#pragma once

#include <iostream>
#include <locale>
#include <string>
#include <codecvt>

namespace NvpSysPort
{
    class File
    {
    public:
        virtual ~File() = default;

        struct Read_
        {
            Read_() : buf(nullptr), len(0) {}
            Read_(const char* _buf, uint64_t _len) : buf(_buf), len(_len) {}
            ~Read_() { if (buf) delete[](buf); }

            const char* buf;
            uint64_t len;
        };
        virtual Read_ Read() = 0;
        virtual void Write(const char* buf, uint64_t len) = 0;
        virtual void Close() = 0;
    };

    File* fileInExePath(const char* file_name);

    typedef long long SysClockTick;
    SysClockTick getSystemClockTick();
    SysClockTick getSystemClockFreq();
}

class NvpUtil
{
public:
    static NvpSysPort::File* fileInExePath(const char* file_name);

    static std::u16string utf8_to_utf16(const std::string& str);
    static std::u32string utf8_to_utf32(const std::string& str);

    static std::string utf16_to_utf8(const std::u16string& str);
    static std::string utf32_to_utf8(const std::u32string& str);

    static NvpSysPort::SysClockTick getSystemClockTick();
    static NvpSysPort::SysClockTick getSysTickInterval(int channel);
    static double getFpsByChannel(int channel);
};
