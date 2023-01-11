#include "Util.h"
#include <chrono>
#include <cmath>

using namespace std::chrono;

template<class TimeUnit>
inline int32_t Util::Timer::GetNowCount(void)
{
    // template‚ÌŒ^‚Í[ŠÔŠÔŠu‚ğ‚ğ•\‚·Œ^]‚Å‚È‚¯‚ê‚Î‚È‚ç‚È‚¢B
    // ref: https://cpprefjp.github.io/reference/chrono/duration_aliases.html

    return (int32_t)duration_cast<TimeUnit>(steady_clock::now().time_since_epoch()).count();
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