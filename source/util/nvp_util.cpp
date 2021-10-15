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
