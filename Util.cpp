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
    endTime_ = endTime;
}

const bool Util::Timer::GetIsEnd(void)
{
    float_t elapsedTime = (GetNowCount<milliseconds>() - startTime_) / 1000.0f;
    return endTime_ < elapsedTime;
}

const float_t Util::Timer::GetElapsedTime(void)
{
    return float_t{ (GetNowCount<milliseconds>() - startTime_) / 1000.0f };
}

const float_t& Util::Timer::GetEndTime(void) const
{
    return endTime_;
}

float Util::EaseInOutSine(float t)
{
    return -(std::cosf(MY_PI * t) - 1.f) / 2.f;
}

float Util::EaseInOutSine(float start, float end, float t)
{
    float time = -(std::cosf(MY_PI * t) - 1.f) / 2.f;
    return start * (1.f - time) + end * time;
}

float Util::EaseInOutCubic(float t)
{
    return t < 0.5f ? 4 * t * t * t : 1.f - std::powf(-2.f * t + 2.f, 3.f) / 2.f;
}

float Util::EaseInOutCubic(float start, float end, float t)
{
    float time;
    if (t < 0.5f) {
        time = 4.f * t * t * t;
    }
    else {
        time = 1.f - std::powf(-2.f * t + 2.f, 3.f) / 2.f;
    }
    return start * (1.f - time) + end * time;
}
