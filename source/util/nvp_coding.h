/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#pragma once

#include <string>
#include <cstdlib>

namespace NvpDrawData
{
    class Null_Data_;
}

namespace NvpCoding
{
    static void encode32Bit(char dst[4], const uint32_t& bit_32)
    {
        uint8_t* const rep = reinterpret_cast<uint8_t*>(dst);

        rep[0] = static_cast<uint8_t>(bit_32);
        rep[1] = static_cast<uint8_t>(bit_32 >> 8);
        rep[2] = static_cast<uint8_t>(bit_32 >> 16);
        rep[3] = static_cast<uint8_t>(bit_32 >> 24);
    }

    static void decode32Bit(const char src[4], uint32_t& bit_32)
    {
        const uint8_t* const rep = reinterpret_cast<const uint8_t*>(src);

        bit_32 = (static_cast<uint32_t>(rep[0])) |
            (static_cast<uint32_t>(rep[1]) << 8) |
            (static_cast<uint32_t>(rep[2]) << 16) |
            (static_cast<uint32_t>(rep[3]) << 24);
    }

    static void encode64Bit(char dst[8], const uint64_t& bit_64)
    {
        uint8_t* const rep = reinterpret_cast<uint8_t*>(dst);

        rep[0] = static_cast<uint8_t>(bit_64);
        rep[1] = static_cast<uint8_t>(bit_64 >> 8);
        rep[2] = static_cast<uint8_t>(bit_64 >> 16);
        rep[3] = static_cast<uint8_t>(bit_64 >> 24);
        rep[4] = static_cast<uint8_t>(bit_64 >> 32);
        rep[5] = static_cast<uint8_t>(bit_64 >> 40);
        rep[6] = static_cast<uint8_t>(bit_64 >> 48);
        rep[7] = static_cast<uint8_t>(bit_64 >> 56);
    }

    static void decode64Bit(const char src[8], uint64_t& bit_64)
    {
        const uint8_t* const rep = reinterpret_cast<const uint8_t*>(src);

        bit_64 = (static_cast<uint64_t>(rep[0])) |
            (static_cast<uint64_t>(rep[1]) << 8) |
            (static_cast<uint64_t>(rep[2]) << 16) |
            (static_cast<uint64_t>(rep[3]) << 24) |
            (static_cast<uint64_t>(rep[4]) << 32) |
            (static_cast<uint64_t>(rep[5]) << 40) |
            (static_cast<uint64_t>(rep[6]) << 48) |
            (static_cast<uint64_t>(rep[7]) << 56);
    }

    struct CodingParam
    {
        std::string* dst;
        const char* src;
        uint32_t& offset;
    };

    template<typename T>
    void codingUTF8Char(CodingParam& param, T t)
    {
        if (param.dst)
        {
            auto utf8 = reinterpret_cast<std::string*>(t);
            auto size = utf8->size();
            char str[4];
            encode32Bit(str, reinterpret_cast<const uint32_t&>(size)); //encode size first;
            param.dst->append(str, 4);
            param.dst->append(*utf8);
        }
        else if (param.src)
        {
            auto utf8 = reinterpret_cast<std::string*>(t);
            uint32_t size = 0;
            decode32Bit(param.src + param.offset, size);
            *utf8 = std::string(param.src + param.offset + 4, size);
            param.offset += (4 + size);
        }
    }

    template<typename T>
    void coding8Bit(CodingParam& param, T t)
    {
        if (param.dst)
        {
            const char* str = reinterpret_cast<const char*>(t);
            param.dst->append(str, 1);
        }
        else if (param.src)
        {
            char* str = reinterpret_cast<char*>(t);
            *str = *(param.src + param.offset);
            param.offset += 1;
        }
    }

    template<typename T>
    void coding32Bit(CodingParam& param, T t)
    {
        if (param.dst)
        {
            char str[4];
            encode32Bit(str, reinterpret_cast<const uint32_t&>(*t));
            param.dst->append(str, 4);
        }
        else if (param.src)
        {
            decode32Bit(param.src + param.offset, reinterpret_cast<uint32_t&>(*t));
            param.offset += 4;
        }
    }

    template<typename T>
    void coding64Bit(CodingParam& param, T t)
    {
        if (param.dst)
        {
            char str[8];
            encode64Bit(str, reinterpret_cast<const uint64_t&>(*t));
            param.dst->append(str, 8);
        }
        else if (param.src)
        {
            decode64Bit(param.src + param.offset, reinterpret_cast<uint64_t&>(*t));
            param.offset += 8;
        }
    }

    template<typename T>
    void detectType(CodingParam& param, T t)
    {
        if (typeid(t) == typeid(char*))
        {
            coding8Bit(param, t);
        }
        else if (typeid(t) == typeid(uint8_t*))
        {
            coding8Bit(param, t);
        }
        else if (typeid(t) == typeid(bool*))
        {
            coding8Bit(param, t);
        }
        else if (typeid(t) == typeid(float*))
        {
            coding32Bit(param, t);
        }
        else if (typeid(t) == typeid(uint32_t*))
        {
            coding32Bit(param, t);
        }
        else if (typeid(t) == typeid(int32_t*))
        {
            coding32Bit(param, t);
        }
        else if (typeid(t) == typeid(double*))
        {
            coding64Bit(param, t);
        }
        else if (typeid(t) == typeid(uint64_t*))
        {
            coding64Bit(param, t);
        }
        else if (typeid(t) == typeid(int64_t*))
        {
            coding64Bit(param, t);
        }
        else if (typeid(t) == typeid(std::string*))
        {
            codingUTF8Char(param, t);
        }
        else if (typeid(t) == typeid(NvpDrawData::Null_Data_*))
        {
            return;
        }
        else
        {
            abort();
        }
    }

    template<typename ... T>
    void codingSeq(std::string* dst, const char* src, T ... args)
    {
        auto size = sizeof...(args);
        uint32_t seq_offset = 0;
        CodingParam param{ dst, src, seq_offset };
        auto a = { (detectType(param, args), 0) ... };
    }
}
