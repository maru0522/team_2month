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
    public: // ��`
        using nanoseconds = std::chrono::nanoseconds;
        using microseconds = std::chrono::microseconds;
        using milliseconds = std::chrono::milliseconds;
        using seconds = std::chrono::seconds;
        using minutes = std::chrono::minutes;
        using hours = std::chrono::hours;

    public: // �ÓI�֐�
        /// <summary>
        /// <para>Windows���N�����Ă���̌o�ߎ��Ԃ��~���b�P�ʂł���킵���l��Ԃ��B</para>
        /// <para>���������擾����킯�ł͂Ȃ����A���̊֐��̑��݈Ӌ`�͎��Ԍv���ɂ���B</para>
        /// <para>TimeUnit��std::chrono::duration�̕ʖ��ɂ���[���ԊԊu��\���^]</para>
        /// <para>�݂̂��g�p�ł���B</para>
        /// </summary>
        /// <typeparam name="TimeUnit"></typeparam>
        /// <returns></returns>
        template<class TimeUnit>
        static int32_t GetNowCount(void);

    public: // �֐�
        void Start(float_t endTime = 1.0f);

        // getter
        const float_t GetElapsedTime(void);
        const float_t& GetEndTime(void) const;
        const bool GetIsEnd(void);

    private: // �ϐ�
        int32_t startTime_{ 0 }; // �v���J�n����
        float_t endTime_{ 0.0f }; // ���B�ڕW����

        bool isEnd_{ false }; // endTime_ < elapsedTime_ = true
    };

    namespace expt {

    }
}