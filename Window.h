#pragma once
#include <Windows.h>

class Window
{
public: // 静的関数
    static Window* GetInstance(); // インスタンスを取得
    static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public: // 静的変数
    // ウィンドウサイズ
    static constexpr size_t width_ = 1280;
    static constexpr size_t height_ = 720;

    // ウィンドウクラス名
    static const wchar_t windowClassName_[];

public: // 関数
    Window();
    Window(const char* title);
    void DelWindow();

    // ウィンドウの表示状態を変更する関数
    // デフォルト:nCmdShow = SW_SHOW
    void SetDisplay(int nCmdShow);

    // ウィンドウ固定用のメッセージループ処理
    // WM_QUITならFALSEを返す
    bool GetKeepWindow();

    // ウィンドウハンドルの取得
    // const: 引き渡しするメンバ変数のメンバ関数内部における変更の禁止
    HWND GetHwnd() const { return hwnd_; }

    HINSTANCE GetHInstance() const { return w_.hInstance; }

private: // 変数
    // ウィンドウクラスの生成
    WNDCLASSEX w_{};

    // ウィンドウハンドルの生成
    HWND hwnd_;
};