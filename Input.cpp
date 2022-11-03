#include "Input.h"
#include <cassert>

// 実体の生成
Microsoft::WRL::ComPtr<IDirectInput8> Input::Keyboard::directInput_ = nullptr;
Microsoft::WRL::ComPtr<IDirectInputDevice8> Input::Keyboard::keyboard_ = nullptr;
std::array<BYTE, 256> Input::Keyboard::keysPre_ = { 0 };
std::array<BYTE, 256> Input::Keyboard::keys_ = { 0 };

void Input::Keyboard::Initialize()
{
    // ウィンドウのインスタンス取得
    Window* wnd_ = Window::GetInstance();

    // DirectInputの初期化
    HRESULT r = DirectInput8Create(
        wnd_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, nullptr);
#ifdef _DEBUG
	assert(SUCCEEDED(r));
#endif // _DEBUG

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

void Input::Keyboard::Update()
{
    // preKeysへ情報保存
    memcpy(keysPre_.data(), keys_.data(), sizeof(keys_));

    // キーボード情報の取得
    keyboard_->Acquire();

    // キーの入力状態の初期化
    keys_.fill(0);

    // キー全ての入力状態の取得
    keyboard_->GetDeviceState(static_cast<DWORD>(size(keys_)), keys_.data());
}