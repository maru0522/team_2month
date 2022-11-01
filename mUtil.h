#pragma once
#include <string>

namespace Convert {
    //----------//
    /* 文字列変換 */
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

class Timer {
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

private: // 静的変数
    static constexpr float_t epsilon_{ std::numeric_limits<float>::epsilon() };

public: // 関数
    void Start(float_t endTime = 1.0f);
    void Update(void);

    // getter
    const bool& GetIsEnd(void) const { return isEnd_; }

private: // 変数
    int32_t startTime_{ 0 }; // 計測開始時間
    float_t endTime_{ 0.0f }; // 到達目標時間
    float_t elapsedTime_{ 0.0f }; // 経過時間

    bool isEnd_{ false }; // endTime_ < elapsedTime_ = true
};