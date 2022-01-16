/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#include "nvp_util.h"

#if 0
std::string NvpUtil::getExeFilePath()
{
    WCHAR path_u16[MAX_PATH] = { 0 };
    auto ret = GetModuleFileNameW(NULL, path_u16, MAX_PATH);
    if (ret == 0)
    {
        return std::string();
    }

    auto u16_str = std::u16string((const char16_t*)path_u16);
    auto path_u8 = NvpUtil::utf16_to_utf8(u16_str);

    for (;;)
    {
        auto offset = path_u8.find("\\");

        if (offset == std::string::npos)
        {
            break;
        }
        path_u8.replace(offset, 1, "/");
    }

    auto end = path_u8.find_last_of('/');

    if (end != std::string::npos)
    {
        return std::string(path_u8.c_str(), end);
    }
    else
    {
        return path_u8;
    }
}

std::string NvpUtil::getExeFilePath()
{
    char path[PATH_MAX] = { 0 };
    uint32_t size = sizeof(path);

    if (_NSGetExecutablePath(path, &size) != 0)
    {
        return std::string();
    }

    char exe_path[PATH_MAX] = { 0 };

    if (!realpath(path, exe_path))
    {
        return std::string();
    }

    std::string path_u8(exe_path);
    auto end = path_u8.find_last_of('/');

    if (end != std::string::npos)
    {
        return std::string(path_u8.c_str(), end);
    }
    else
    {
        return path_u8;
    }
}
#endif

NvpSysPort::File* NvpUtil::fileInExePath(const char* file_name)
{
    return NvpSysPort::fileInExePath(file_name);
}

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

std::string NvpUtil::utf16_to_utf8(const std::u16string& str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conversion;
    return conversion.to_bytes(str);
}

std::string NvpUtil::utf32_to_utf8(const std::u32string& str)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conversion;
    return conversion.to_bytes(str);
}

NvpSysPort::SysClockTick NvpUtil::getSystemClockTick()
{
    return NvpSysPort::getSystemClockTick();
}

static const int MAX_QPC_CHANNEL = 100;

NvpSysPort::SysClockTick NvpUtil::getSysTickInterval(int channel)
{
    if (channel >= MAX_QPC_CHANNEL || channel < 0)
    {
        return -1;
    }

    auto now = getSystemClockTick();
    static NvpSysPort::SysClockTick prev[MAX_QPC_CHANNEL] = { 0 };
    auto QPCInterval = now - prev[channel];
    prev[channel] = now;
    return QPCInterval;
}

double NvpUtil::getFpsByChannel(int channel)
{
    auto frequency = NvpSysPort::getSystemClockFreq();
    double interval_sec = (double)getSysTickInterval(channel) / (double)frequency;
    double fps = (double)1.0 / interval_sec;
    return fps;
}
