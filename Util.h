#pragma once
#include <string>
#include <chrono>
#include <random>

namespace Util {


    template<typename T>
    const T& Random(const double min, const double max) {
        static std::random_device randomSeed;
        static std::mt19937 generator(randomSeed());

        std::uniform_real_distribution<> dist1(min, max);

        return static_cast<T>(dist1(generator));
    }

    template<typename T>
    const T& Clamp(const T& value, const T& min, const T& max) {
        if (value < min) {
            return min;
        }
        else if (value > max) {
            return max;
        }
        else {
            return value;
        }
    }

    float EaseInOutSine(float t);
    float EaseInOutSine(float start, float end, float t);
    float EaseInOutCubic(float t);
    float EaseInOutCubic(float start, float end, float t);

    constexpr float MY_PI{ 3.14159265f };
    namespace Convert {

        inline constexpr float ToRadian(float fDegrees) { return fDegrees * MY_PI / 180.0f; }
        inline constexpr float ToDegree(float fRadians) { return fRadians * 180.0f / MY_PI; }
    }

    class Timer {
    public: // 定義
        using nanoseconds = std::chrono::nanoseconds;
        using microseconds = std::chrono::microseconds;
        using milliseconds = std::chrono::milliseconds;
        using seconds = std::chrono::seconds;
        using minutes = std::chrono::minutes;
        using hours = std::chrono::hours;

    public: // 静的関数
        /// <summary>
        /// <para>Windowsが起動してからの経過時間をミリ秒単位であらわした値を返す。</para>
        /// <para>日時等を取得するわけではないが、この関数の存在意義は時間計測にある。</para>
        /// <para>TimeUnitはstd::chrono::durationの別名にある[時間間隔を表す型]</para>
        /// <para>のみを使用できる。</para>
        /// </summary>
        /// <typeparam name="TimeUnit"></typeparam>
        /// <returns></returns>
        template<class TimeUnit>
        static int32_t GetNowCount(void);

    public: // 関数
        void Start(float_t endTime = 1.0f);

        // getter
        const float_t GetElapsedTime(void);
        const float_t& GetEndTime(void) const;
        const bool GetIsEnd(void);

    private: // 変数
        int32_t startTime_{ 0 }; // 計測開始時間
        float_t endTime_{ 0.0f }; // 到達目標時間

        bool isEnd_{ false }; // endTime_ < elapsedTime_ = true
    };

    namespace expt {

    }
}