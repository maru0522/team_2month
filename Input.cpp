#include "Input.h"
#include <cassert>

// 実体の生成
Microsoft::WRL::ComPtr<IDirectInput8> directInput_{ nullptr }; // DirectInput生成

#pragma region keyboard
Microsoft::WRL::ComPtr<IDirectInputDevice8> Input::Keyboard::keyboard_{ nullptr };
std::array<BYTE, 256> Input::Keyboard::keysPre_{ 0 };
std::array<BYTE, 256> Input::Keyboard::keys_{ 0 };
#pragma endregion

#pragma region diPad
Microsoft::WRL::ComPtr<IDirectInputDevice8> Input::DIPad::diPad_{ nullptr };
DIJOYSTATE Input::DIPad::diStatePre_{ 0 };
DIJOYSTATE Input::DIPad::diState_{ 0 };
bool Input::DIPad::isConnect_{ false };
#pragma endregion

#pragma region xPad
Input::XPad::CustomDeadZone Input::XPad::deadZone_{ 0 };
XINPUT_STATE Input::XPad::xStatePre_{ 0 };
XINPUT_STATE Input::XPad::xState_{ 0 };
bool Input::XPad::isConnect_{ false };
#pragma endregion

void Input::Keyboard::Initialize(void)
{
    // ウィンドウのインスタンス取得
    Window* wnd_ = Window::GetInstance();
    HRESULT r = S_FALSE;

    if (directInput_ == nullptr) {
        // DirectInputの初期化
        r = DirectInput8Create(
            wnd_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, nullptr);
#ifdef _DEBUG
        assert(SUCCEEDED(r));
#endif // _DEBUG
    }

    // キーボードデバイスの生成
    r = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG

    // 入力データ形式のセット
    r = keyboard_->SetDataFormat(&c_dfDIKeyboard); // 標準形式
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG

    // 排他制御レベルのセット
    r = keyboard_->SetCooperativeLevel(
        wnd_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
}

void Input::Keyboard::Update(void)
{
    // preKeysへ情報保存
    memcpy(keysPre_.data(), keys_.data(), sizeof(keys_));

    // キーボード情報の取得
    keyboard_->Acquire();

    // キーの入力状態の初期化
    keys_.fill(0);

    // キー全ての入力状態の取得
    keyboard_->GetDeviceState((DWORD)size(keys_), keys_.data());
}

void Input::DIPad::Initialize(void)
{
    // ウィンドウのインスタンス取得
    Window* wnd_ = Window::GetInstance();
    HRESULT r = S_FALSE;

    if (directInput_ == nullptr) {
        // DirectInputの初期化
        r = DirectInput8Create(
            wnd_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, nullptr);
#ifdef _DEBUG
        assert(SUCCEEDED(r));
#endif // _DEBUG
    }

    // デバイスの列挙
    r = directInput_->EnumDevices(DI8DEVTYPE_GAMEPAD, DeviceFindCallBack, nullptr, DIEDFL_ATTACHEDONLY);
}

void Input::DIPad::Update(void)
{
    if (!isConnect_) {
        return;
    }

    // preKeysへ情報保存
    memcpy(&diStatePre_, &diState_, sizeof(diState_));

    diPad_->Acquire();

    std::fill(std::begin(diState_.rgbButtons), std::end(diState_.rgbButtons), 0);

    //DIJOYSTATE pad;

    //joyPad_->GetDeviceState(sizeof(pad), &pad);
    diPad_->GetDeviceState(sizeof(diState_), &diState_);

    //std::copy(std::begin(pad.rgbButtons), std::end(pad.rgbButtons), pad_.rgbButtons);
}

BOOL __stdcall Input::DIPad::DeviceFindCallBack(const DIDEVICEINSTANCE* pdidInstance, void* pContext)
{
    directInput_->CreateDevice(pdidInstance->guidInstance, &diPad_, NULL);

    diPad_->SetDataFormat(&c_dfDIJoystick);

    return DIENUM_STOP;
}

void Input::XPad::Initialize(void)
{
    XInputGetState(0, &xState_);
}

void Input::XPad::Update(void)
{
    xStatePre_ = xState_;

    ZeroMemory(&xState_, sizeof(XINPUT_STATE));

    if (XInputGetState(0, &xState_) == ERROR_SUCCESS) {
        isConnect_ = true;
    }
    else {
        isConnect_ = false;
    }
}

const DirectX::XMFLOAT2 Input::XPad::GetLStick(void)
{
    DirectX::XMFLOAT2 tmp;

    tmp.x = xState_.Gamepad.sThumbLX;
    tmp.y = xState_.Gamepad.sThumbLY;

    if ((xState_.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && xState_.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
        (xState_.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && xState_.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) {
        tmp.x = 0;
        tmp.y = 0;
    }

    return { (float)tmp.x, (float)tmp.y };
}

const DirectX::XMFLOAT2 Input::XPad::GetLStickRaw(void)
{
    return { (float)xState_.Gamepad.sThumbLX, (float)xState_.Gamepad.sThumbLY };
}

const DirectX::XMFLOAT2 Input::XPad::GetLStickCustom(void)
{
    DirectX::XMFLOAT2 tmp;

    tmp.x = xState_.Gamepad.sThumbLX;
    tmp.y = xState_.Gamepad.sThumbLY;

    if ((xState_.Gamepad.sThumbLX < deadZone_.xLeftValueX && xState_.Gamepad.sThumbLX > -deadZone_.xLeftValueX) &&
        (xState_.Gamepad.sThumbLY < deadZone_.xLeftValueY && xState_.Gamepad.sThumbLY > -deadZone_.xLeftValueY)) {
        tmp.x = 0;
        tmp.y = 0;
    }

    return { (float)tmp.x, (float)tmp.y };
}

const DirectX::XMFLOAT2 Input::XPad::GetRStick(void)
{
    DirectX::XMFLOAT2 tmp;

    tmp.x = xState_.Gamepad.sThumbRX;
    tmp.y = xState_.Gamepad.sThumbRY;

    if ((xState_.Gamepad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && xState_.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
        (xState_.Gamepad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && xState_.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)) {
        tmp.x = 0;
        tmp.y = 0;
    }

    return { (float)tmp.x, (float)tmp.y };
}

const DirectX::XMFLOAT2 Input::XPad::GetRStickRaw(void)
{
    return { (float)xState_.Gamepad.sThumbRX, (float)xState_.Gamepad.sThumbRY };
}

const DirectX::XMFLOAT2 Input::XPad::GetRStickCustom(void)
{
    DirectX::XMFLOAT2 tmp;

    tmp.x = xState_.Gamepad.sThumbRX;
    tmp.y = xState_.Gamepad.sThumbRY;

    if ((xState_.Gamepad.sThumbRX < deadZone_.xRightValueX && xState_.Gamepad.sThumbRX > -deadZone_.xRightValueX) &&
        (xState_.Gamepad.sThumbRY <  deadZone_.xRightValueY && xState_.Gamepad.sThumbRY > -deadZone_.xRightValueY)) {
        tmp.x = 0;
        tmp.y = 0;
    }

    return { (float)tmp.x, (float)tmp.y };
}

void Input::XPad::Vibrate(int32_t lPower, int32_t rPower)
{
    XINPUT_VIBRATION v;

    ZeroMemory(&v, sizeof(XINPUT_VIBRATION));

    v.wLeftMotorSpeed = lPower;
    v.wRightMotorSpeed = rPower;

    XInputSetState(0, &v);
}