#include "util.h"

NvpUtil* const nvpUtil = nullptr;

std::u16string NvpUtil::utf8_to_utf16(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conversion;
    return conversion.from_bytes(str);
}

std::u32string NvpUtil::utf8_to_utf32(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conversion;
    return conversion.from_bytes(str);
}

SysClockTick NvpUtil::getSystemClockTick()
{
#ifdef _WIN32
    LARGE_INTEGER QPC;
    QueryPerformanceCounter(&QPC);
    return QPC.QuadPart;
#endif

#ifdef __APPLE__
    return clock_gettime_nsec_np(CLOCK_MONOTONIC_RAW);
#endif
}

#define MAX_QPC_CHANNEL 100

SysClockTick NvpUtil::getSysTickInterval(int channel)
{
    if (channel >= MAX_QPC_CHANNEL || channel < 0)
    {
        return -1;
    }
    
    auto now = getSystemClockTick();
    static SysClockTick prev[MAX_QPC_CHANNEL] = { 0 };
    auto QPCInterval = now - prev[channel];
    prev[channel] = now;
    return QPCInterval;
}

double NvpUtil::getFpsByChannel(int channel)
{
    SysClockTick frequency;
    
#ifdef _WIN32
    LARGE_INTEGER Frequency;
    QueryPerformanceFrequency(&Frequency);
    frequency = Frequency.QuadPart;
#endif

#ifdef __APPLE__
    frequency = 1000000000;
#endif
    
    double interval_sec = (double)getSysTickInterval(channel) / (double)frequency;
    double fps = (double)1.0 / interval_sec;
    return fps;
}
