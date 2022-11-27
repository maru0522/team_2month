#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include "Window.h"
#include <array>
#include <DirectXMath.h>
#include <dinput.h>
#include <Xinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "xinput.lib")
#include <wrl.h>

namespace Input {
    // エイリアステンプレート
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    class Keyboard
    {
    public: // 関数
        static void Initialize(void);
        static void Update(void);

        // 押した瞬間
        static bool IsTrigger(UINT8 key) { return !keysPre_[key] && keys_[key]; }

        // 押されているとき
        static bool IsDown(UINT8 key) { return keys_[key]; }

        // 離された瞬間
        static bool IsReleased(UINT8 key) { return keysPre_[key] && !keys_[key]; }

    private: // 変数
        static ComPtr<IDirectInputDevice8> keyboard_; // キーボードデバイス生成
        static std::array<BYTE, 256> keysPre_;
        static std::array<BYTE, 256> keys_;
    };

    class DIPad
    {
    public: // 関数
        static void Initialize(void);
        static void Update(void);

        // 押した瞬間
        static bool IsTrigger(uint8_t button) { return !diStatePre_.rgbButtons[button] && diState_.rgbButtons[button]; }

        // 押されているとき
        static bool IsDown(uint8_t button) { return diState_.rgbButtons[button]; }

        // 離された瞬間
        static bool IsReleased(uint8_t button) { return diStatePre_.rgbButtons[button] && !diState_.rgbButtons[button]; }

    private: // 関数
        static BOOL __stdcall DeviceFindCallBack(const DIDEVICEINSTANCE* pdidInstance, void* pContext);

    private: // 変数
        static ComPtr<IDirectInputDevice8> diPad_; 
        static DIJOYSTATE diStatePre_;
        static DIJOYSTATE diState_;
        static bool isConnect_;
    };

    class XPad
    {
    public: // 定義
        struct CustomDeadZone
        {
            uint16_t xLeftValueX;
            uint16_t xLeftValueY;
            uint16_t xRightValueX;
            uint16_t xRightValueY;
        };

    public: // 関数
        static void Initialize(void);
        static void Update(void);

        // 押した瞬間
        static bool IsTrigger(uint16_t button) { return xStatePre_.Gamepad.wButtons != button && xState_.Gamepad.wButtons == button; }

        // 押されているとき
        static bool IsDown(uint16_t button) { return xState_.Gamepad.wButtons == button; }

        // 離された瞬間
        static bool IsReleased(uint16_t button) { return xStatePre_.Gamepad.wButtons == button && xState_.Gamepad.wButtons != button; }

        static const DirectX::XMFLOAT2 GetLStick(void);
        static const DirectX::XMFLOAT2 GetLStickRaw(void);
        static const DirectX::XMFLOAT2 GetLStickCustom(void);

        static const DirectX::XMFLOAT2 GetRStick(void);
        static const DirectX::XMFLOAT2 GetRStickRaw(void);
        static const DirectX::XMFLOAT2 GetRStickCustom(void);

        static bool IsTriggerLT(void) { return xState_.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD; }
        static uint8_t GetLT(void) { return xState_.Gamepad.bLeftTrigger; }

        static bool IsTriggerRT(void) { return xState_.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD; }
        static uint8_t GetRT(void) { return xState_.Gamepad.bRightTrigger; }

        static void Vibrate(int32_t lPower = 0, int32_t rPower = 0); // 引数なしで振動停止
        
#pragma region setter
        static void SetDeadZone(const CustomDeadZone& deadzone) { deadZone_ = deadzone; }
#pragma endregion

    private: // 変数
        static CustomDeadZone deadZone_;
        static XINPUT_STATE xStatePre_;
        static XINPUT_STATE xState_;
        static bool isConnect_;
    };
};

#ifndef NON_NAMESPACE_Input // 定義でusing解除

using KEYS = Input::Keyboard;
using DPAD = Input::DIPad;
using XPAD = Input::XPad;

#endif // もしInputのnamespaceがあるなら