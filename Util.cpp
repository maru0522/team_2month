#include "Util.h"
#include <chrono>
#include <cmath>

std::string Util::Convert::CharToString(const char* cstr)
{
    return std::string(cstr);
}

std::wstring Util::Convert::CharToWString(const char* cstr)
{
    std::string str(cstr);

    return std::wstring(str.begin(), str.end());
}

std::string Util::Convert::WCharToString(const wchar_t* wcstr)
{
    std::wstring wstr(wcstr);

    return std::string(wstr.begin(), wstr.end());
}

std::wstring Util::Convert::WCharToWString(const wchar_t* wcstr)
{
    return std::wstring(wcstr);
}

std::wstring Util::Convert::StringToWString(const std::string str)
{
    return std::wstring(str.begin(), str.end());
}

std::string Util::Convert::WstringToString(const std::wstring wstr)
{
    return std::string(wstr.begin(), wstr.end());
}

using namespace std::chrono;

template<class TimeUnit>
inline int32_t Util::Timer::GetNowCount(void)
{
    // template‚ÌŒ^‚Í[ŠÔŠÔŠu‚ğ‚ğ•\‚·Œ^]‚Å‚È‚¯‚ê‚Î‚È‚ç‚È‚¢B
    // ref: https://cpprefjp.github.io/reference/chrono/duration_aliases.html

    return static_cast<int32_t>(duration_cast<TimeUnit>(steady_clock::now().time_since_epoch()).count());
}

void Util::Timer::Start(float_t endTime)
{
    startTime_ = GetNowCount<milliseconds>();
}

const bool Util::Timer::GetIsEnd(void)
{
    float_t elapsedTime = (GetNowCount<milliseconds>() - startTime_) / 1000.0f;
    return endTime_ < elapsedTime;
}
