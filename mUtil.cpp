#include "mUtil.h"

std::string Convert::CharToString(const char* cstr)
{
    return std::string(cstr);
}

std::wstring Convert::CharToWString(const char* cstr)
{
    std::string str(cstr);

    return std::wstring(str.begin(), str.end());
}

std::string Convert::WCharToString(const wchar_t* wcstr)
{
    std::wstring wstr(wcstr);

    return std::string(wstr.begin(), wstr.end());
}

std::wstring Convert::WCharToWString(const wchar_t* wcstr)
{
    return std::wstring(wcstr);
}

std::wstring Convert::StringToWString(std::string str)
{
    return std::wstring(str.begin(), str.end());
}

std::string Convert::WstringToString(std::wstring wstr)
{
    return std::string(wstr.begin(), wstr.end());
}