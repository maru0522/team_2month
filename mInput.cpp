#include "mInput.h"
#include <cassert>

// 実体の生成
Microsoft::WRL::ComPtr<IDirectInput8> mInput::Keyboard::directInput = nullptr;
Microsoft::WRL::ComPtr<IDirectInputDevice8> mInput::Keyboard::keyboard = nullptr;
std::array<BYTE, 256> mInput::Keyboard::preKeys = { 0 };
std::array<BYTE, 256> mInput::Keyboard::keys = { 0 };

void mInput::Keyboard::Initialize()
{
    HRESULT result = S_FALSE;

    // ウィンドウのインスタンス取得
    mWindow* wnd_ = mWindow::GetInstance();

    // DirectInputの初期化
    result = DirectInput8Create(
        wnd_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
    assert(SUCCEEDED(result));

    // キーボードデバイスの生成
    result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
    assert(SUCCEEDED(result));

    // 入力データ形式のセット
    result = keyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式
    assert(SUCCEEDED(result));

    // 排他制御レベルのセット
    result = keyboard->SetCooperativeLevel(
        wnd_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
    assert(SUCCEEDED(result));
}

void mInput::Keyboard::Update()
{
    // preKeysへ情報保存
    memcpy(preKeys.data(), keys.data(), sizeof(keys));

    // キーボード情報の取得
    keyboard->Acquire();

    // キーの入力状態の初期化
    keys.fill(0);

    // キー全ての入力状態の取得
    keyboard->GetDeviceState(static_cast<DWORD>(size(keys)), keys.data());
}