#include "mInput.h"
#include <cassert>

// 実体の生成
Microsoft::WRL::ComPtr<IDirectInput8> mInput::Keyboard::directInput_ = nullptr;
Microsoft::WRL::ComPtr<IDirectInputDevice8> mInput::Keyboard::keyboard_ = nullptr;
std::array<BYTE, 256> mInput::Keyboard::keysPre_ = { 0 };
std::array<BYTE, 256> mInput::Keyboard::keys_ = { 0 };

void mInput::Keyboard::Initialize()
{
    HRESULT result = S_FALSE;

    // ウィンドウのインスタンス取得
    mWindow* wnd_ = mWindow::GetInstance();

    // DirectInputの初期化
    result = DirectInput8Create(
        wnd_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, nullptr);
    assert(SUCCEEDED(result));

    // キーボードデバイスの生成
    result = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
    assert(SUCCEEDED(result));

    // 入力データ形式のセット
    result = keyboard_->SetDataFormat(&c_dfDIKeyboard); // 標準形式
    assert(SUCCEEDED(result));

    // 排他制御レベルのセット
    result = keyboard_->SetCooperativeLevel(
        wnd_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
    assert(SUCCEEDED(result));
}

void mInput::Keyboard::Update()
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