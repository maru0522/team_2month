#include "CtrlFPS.h"
#include <thread>

void CtrlFPS::Initialize(void)
{
    // 現在時間を記録する
    timeRef_ = std::chrono::steady_clock::now();
}

void CtrlFPS::Update(void)
{
    //　現在時間を取得する
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    // 前回記録からの経過時間を取得する
    std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - timeRef_);

    // 1/60秒（より僅かに短い時間）経っていない場合
    if (elapsed < kMinTime_) {
        // 1/60秒経過するまで微小なスリープを繰り返す
        while (std::chrono::steady_clock::now() - timeRef_ < kMinTime_)
        {
            // 1マイクロ秒スリープ
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    }

    // 現在の時間を記録
    timeRef_ = std::chrono::steady_clock::now();
}
