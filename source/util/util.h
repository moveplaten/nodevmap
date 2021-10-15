#pragma once

#include <iostream>
#include <locale>
#include <string>
#include <codecvt>

class NvpUtil
{
public:
    std::u16string utf8_to_utf16(const std::string& str);
    
    std::u32string utf8_to_utf32(const std::string& str);

};


/// GLOBAL nvpUtil;
extern NvpUtil* const nvpUtil;

