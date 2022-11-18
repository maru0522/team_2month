#pragma once
#include <fstream>
#include <wrl.h>
#include <xaudio2.h>

#pragma comment(lib,"xaudio2.lib")

class XAudio
{
private: // 定義
    // エイリアステンプレート
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // 関数
    static void Initialize(void);

private: // 変数
    ComPtr<IXAudio2> xAudio2_;
    IXAudio2MasteringVoice* masterVoice_;
};