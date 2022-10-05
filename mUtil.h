#pragma once
#include <string>

namespace Convert {
    //----------//
    /* •¶Žš—ñ•ÏŠ· */
    //----------//

    // char* -> string
    std::string CharToString(const char* cstr);
    // char* -> wstring(unicode)
    std::wstring CharToWString(const char* cstr);

    // wchar_t*(unicode) -> string 
    std::string WCharToString(const wchar_t* wcstr);
    // wchar_t*(unicode) -> wstring(unicode)
    std::wstring WCharToWString(const wchar_t* wcstr);

    // string -> wstring(unicode)
    std::wstring StringToWString(std::string str);
    // wstring(unicode) -> string
    std::string WstringToString(std::wstring wstr);
}