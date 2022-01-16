/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#include <Windows.h>
#include "util/nvp_util.h"

namespace NvpSysPort
{
    class FileWIN : public File
    {
    public:
        FileWIN(HANDLE file_) : file(file_) {}

        virtual Read_ Read() override
        {
            LARGE_INTEGER file_size;
            if (!GetFileSizeEx(file, &file_size))
            {
                return Read_();
            }
            if (file_size.QuadPart == 0)
            {
                return Read_();
            }

            auto buf = new char[file_size.QuadPart];
            DWORD bytes_read;
            if (!ReadFile(file, buf, file_size.QuadPart, &bytes_read, nullptr))
            {
                delete[](buf);
                return Read_();
            }
            return Read_(buf, file_size.QuadPart);
        }

        virtual void Write(const char* buf, uint64_t len) override
        {
            SetFilePointer(file, NULL, NULL, FILE_BEGIN);
            DWORD bytes_written;
            WriteFile(file, buf, len, &bytes_written, nullptr);
            SetEndOfFile(file);
        }

        virtual void Close() override
        {
            CloseHandle(file);
        }

    private:
        HANDLE file;
    };

    File* fileInExePath(const char* file_name)
    {
        CHAR path[MAX_PATH] = { 0 };
        auto ret = GetModuleFileNameA(NULL, path, MAX_PATH);
        if (ret == 0)
        {
            return nullptr;
        }

        std::string path_str(path);
        auto end = path_str.find_last_of("\\");
        path_str = std::string(path_str.c_str(), end + 1);
        path_str.append(file_name);

        HANDLE file = CreateFileA(path_str.c_str(), GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

        return new FileWIN(file);
    }

    SysClockTick getSystemClockTick()
    {
        LARGE_INTEGER QPC;
        QueryPerformanceCounter(&QPC);
        return QPC.QuadPart;
    }

    SysClockTick getSystemClockFreq()
    {
        LARGE_INTEGER Frequency;
        QueryPerformanceFrequency(&Frequency);
        return Frequency.QuadPart;
    }
}
