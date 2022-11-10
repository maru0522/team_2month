#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include "Window.h"
#include <array>
#include <dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include <wrl.h>

namespace Input {
    class Keyboard
    {
    public: // 関数
        static void Initialize();
        static void Update();

        // 押した瞬間
        static bool IsTrigger(UINT8 key) { return !keysPre_[key] && keys_[key]; }

        // 押されているとき
        static bool IsDown(UINT8 key) { return keys_[key]; }

        // 離された瞬間
        static bool IsReleased(UINT8 key) { return keysPre_[key] && !keys_[key]; }

    private: // 変数
        // エイリアステンプレート
        template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

        static ComPtr<IDirectInput8> directInput_; // DirectInput生成
        static ComPtr<IDirectInputDevice8> keyboard_; // キーボードデバイス生成
        static std::array<BYTE, 256> keysPre_;
        static std::array<BYTE, 256> keys_;
    };
};

#ifndef NON_NAMESPACE_Input // 定義でusing解除

using KEYS = Input::Keyboard;

#endif // もしInputのnamespaceがあるなら