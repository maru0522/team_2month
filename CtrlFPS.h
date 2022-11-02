#pragma once
#include <chrono>

class CtrlFPS
{
public: // Ã“I•Ï”
    static constexpr std::chrono::microseconds kMinTime_{uint64_t(1000000.0f / 60.0f)}; // 1/60•b‚Ò‚Á‚½‚è‚ÌŠÔ
    static constexpr std::chrono::microseconds kMiCheckTime_{uint64_t(1000000.0f / 65.0f)}; // 1/60•b‚æ‚è‹Í‚©‚É’Z‚¢ŠÔ

public: // ŠÖ”
    void Initialize(void);
    void Update(void);

private: // •Ï”
    std::chrono::steady_clock::time_point timeRef_; // ŠÔ‹L˜^iFPSŒÅ’è—pj
};