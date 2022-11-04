#pragma once
#include <string>

namespace Util {
    namespace Convert {
        //----------//
        /* 文字列変換 */
        //----------//

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

    public: // 関数
        void Start(float_t endTime = 1.0f);

        // getter
        const bool GetIsEnd(void);

    private: // 変数
        int32_t startTime_{ 0 }; // 計測開始時間
        float_t endTime_{ 0.0f }; // 到達目標時間

        bool isEnd_{ false }; // endTime_ < elapsedTime_ = true
    };
}