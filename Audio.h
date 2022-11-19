#pragma once
#include <fstream>
#include <wrl.h>
#if _MSC_VER > 1922 && !defined(_SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING)
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#endif
#include <experimental/filesystem>
#include <xaudio2.h>

#pragma comment(lib,"xaudio2.lib")

class XAudio
{
public: // 定義
    using AUDIO_KEY = std::string;
    struct AUDIO_VALUE {
        WAVEFORMATEX wfex;
        uint8_t* pBuffer;
        uint32_t bufferSize;
    };

    struct SoundData
    {
        WAVEFORMATEX wfex;
        uint8_t* pBuffer;
        uint32_t bufferSize;
    };

private: // 定義
    // エイリアステンプレート
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    using fsPath = std::experimental::filesystem::path;

    struct ChunckHeader
    {
        char id[4];
        int32_t size;
    };

    struct RiffHeader
    {
        ChunckHeader chunk;
        char type[4];
    };

    struct FormatChunk
    {
        ChunckHeader chunk;
        WAVEFORMATEX fmt;
    };

public: // 関数
    static void Initialize(void);
    static SoundData Load(const fsPath pathAndFileName);
    static void UnLoad(SoundData* soundData);
    static void PlayWave(IXAudio2* xAudio2, const SoundData& soundData);

private: // 変数
    static ComPtr<IXAudio2> xAudio2_;
    static IXAudio2MasteringVoice* masterVoice_;
};