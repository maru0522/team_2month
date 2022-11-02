#include "Util.h"
#include <chrono>
#include <cmath>

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

using namespace std::chrono;

template<class TimeUnit>
inline int32_t Timer::GetNowCount(void)
{
#ifdef _DEBUG
    // template‚ÌŒ^‚Í[ŠÔŠÔŠu‚ğ‚ğ•\‚·Œ^]‚Å‚È‚¯‚ê‚Î‚È‚ç‚È‚¢B
    // ref: https://cpprefjp.github.io/reference/chrono/duration_aliases.html

    static_assert(std::is_class<TimeUnit>::value, "template must be a class");
#endif // _DEBUG
    return static_cast<int32_t>(duration_cast<TimeUnit>(steady_clock::now().time_since_epoch()).count());
}

void Timer::Start(float_t endTime)
{
    startTime_ = GetNowCount<milliseconds>();
}

void Timer::Update(void)
{
    elapsedTime_ = (GetNowCount<milliseconds>() - startTime_) / 1000.0f;
    if (std::fabsf(endTime_ - elapsedTime_ <= epsilon_)) isEnd_ = true;
}
