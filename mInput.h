#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include "mWindow.h"
#include <array>
#include <dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

namespace mInput {
    class Keyboard
    {
    public: // 関数
        static void Initialize();
        static void Update();

        // 押した瞬間
        static bool IsTrigger(UINT8 key) { return !preKeys[key] && keys[key]; }

        // 押されているとき
        static bool IsDown(UINT8 key) { return keys[key]; }

        // 離された瞬間
        static bool IsReleased(UINT8 key) { return preKeys[key] && !keys[key]; }

    private: // 変数
        static IDirectInput8* directInput; // DirectInput生成
        static IDirectInputDevice8* keyboard; // キーボードデバイス生成
        static std::array<BYTE, 256> preKeys;
        static std::array<BYTE, 256> keys;
    };
};

#ifndef NON_NAMESPACE_mInput

using namespace mInput;

#endif // もしmInputのnamespaceがあるなら