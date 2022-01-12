#import <Cocoa/Cocoa.h>
#include <mach-o/dyld.h>
#include "util/nvp_util.h"

namespace NvpSysPort
{
    class FileOSX : public File
    {
    public:
        FileOSX(NSFileHandle* file_) : file(file_) {}

        virtual Read_ Read() override
        {
            NSData* ret = [file readDataToEndOfFile];
            auto buf = [ret bytes];
            auto len = [ret length];
            return Read_(static_cast<const char*>(buf), len);
        }

        virtual void Write(const char* buf, uint64_t len) override
        {
            NSData* data = [[NSData alloc] initWithBytes:buf length:len];
            [file truncateFileAtOffset:0];
            [file writeData:data];
            [data release];
        }

        virtual void Close() override
        {
            [file closeFile];
        }

    private:
        NSFileHandle* file;
    };

    File* fileInExePath(const char* file_name)
    {
        char path[PATH_MAX] = { 0 };
        uint32_t size = sizeof(path);
        if (_NSGetExecutablePath(path, &size) != 0)
        {
            return nullptr;
        }

        char exe_path[PATH_MAX] = { 0 };
        if (!realpath(path, exe_path))
        {
            return nullptr;
        }

        std::string path_str(exe_path);
        auto end = path_str.find_last_of('/');
        path_str = std::string(path_str.c_str(), end + 1);
        path_str.append(file_name);
        
        NSString* ns_str = [[NSString alloc] initWithUTF8String:path_str.c_str()];
        NSFileHandle* file = [NSFileHandle fileHandleForUpdatingAtPath:ns_str];
        if (file == nil)
        {
            NSFileManager* file_manager = [NSFileManager defaultManager];
            [file_manager createFileAtPath:ns_str contents:nil attributes:nil];
            file = [NSFileHandle fileHandleForUpdatingAtPath:ns_str];
        }
        [ns_str release];
        return new FileOSX(file);
    }

    SysClockTick getSystemClockTick()
    {
        return clock_gettime_nsec_np(CLOCK_MONOTONIC_RAW);
    }

    SysClockTick getSystemClockFreq()
    {
        return 1000000000;
    }
}
