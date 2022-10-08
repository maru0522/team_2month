#pragma once
#include <Windows.h>

class mWindow
{
public: // 静的関数
    static mWindow* GetInstance(); // インスタンスを取得
    static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public: // 静的変数
    // ウィンドウサイズ
    static const size_t width = 1280;
    static const size_t height = 720;

    // ウィンドウクラス名
    static const wchar_t windowClassName[];

public: // 関数
    mWindow();
    mWindow(const char* title);
    void DelWindow();

    // ウィンドウの表示状態を変更する関数
    // デフォルト:nCmdShow = SW_SHOW
    void SetDisplay(int nCmdShow);

    // ウィンドウ固定用のメッセージループ処理
    // WM_QUITならFALSEを返す
    bool IsKeep();

    // ウィンドウハンドルの取得
    // const: 引き渡しするメンバ変数のメンバ関数内部における変更の禁止
    HWND GetHwnd() const { return hwnd; }

    HINSTANCE GetHInstance() const { return w.hInstance; }

private: // 変数
    // ウィンドウクラスの生成
    WNDCLASSEX w{};

    // ウィンドウハンドルの生成
    HWND hwnd;
};