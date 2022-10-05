#include "mWindow.h"
#include <string>
#include "mUtil.h"

static mWindow wnd;
mWindow* mWindow::GetInstance()
{
    return &wnd;
}

LRESULT mWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    // メッセージに応じてゲーム固有の処理を行う
    switch (msg) {
        // ウィンドウが破棄された
        case WM_DESTROY:
            // OSに対して、アプリの終了を伝える
            PostQuitMessage(0);
            return 0;
    }

    // 標準のメッセージ処理を行う
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

const wchar_t mWindow::windowClassName[] = L"maruyamaEngine";

mWindow::mWindow()
{
    // ウィンドウクラスの設定
    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = static_cast<WNDPROC>(WindowProc);   // ウィンドウプロシージャを設定
    w.lpszClassName = windowClassName;                  // ウィンドウクラス名
    w.hInstance = GetModuleHandle(nullptr);             // ウィンドウハンドル
    w.hCursor = LoadCursor(NULL, IDC_ARROW);            // カーソル指定

    // ウィンドウクラスをOSに登録する
    RegisterClassEx(&w);

    // ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
    RECT wrc = { 0, 0, width, height };
    // 自動でサイズを補正する
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

    // ウィンドウオブジェクトの生成
    hwnd = CreateWindow(
        windowClassName,      // クラス名
        w.lpszClassName,      // タイトルバーの文字
        WS_OVERLAPPEDWINDOW,  // 標準的なウィンドウスタイル
        CW_USEDEFAULT,        // 表示X座標（OSに任せる）
        CW_USEDEFAULT,        // 表示Y座標（OSに任せる）
        wrc.right - wrc.left, // ウィンドウ横幅
        wrc.bottom - wrc.top, // ウィンドウ縦幅
        nullptr,              // 親ウィンドウハンドル
        nullptr,              // メニューハンドル
        w.hInstance,          // 呼び出しアプリケーションハンドル
        nullptr               // オプション
    );

    // ウィンドウを表示状態にする
    ShowWindow(hwnd, SW_SHOW);
}

mWindow::mWindow(const char* title)
{
    // ウィンドウクラスの設定
    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = static_cast<WNDPROC>(WindowProc);   // ウィンドウプロシージャを設定
    w.lpszClassName = windowClassName;                  // ウィンドウクラス名
    w.hInstance = GetModuleHandle(nullptr);             // ウィンドウハンドル
    w.hCursor = LoadCursor(NULL, IDC_ARROW);            // カーソル指定

    // ウィンドウクラスをOSに登録する
    RegisterClassEx(&w);

    // ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
    RECT wrc = { 0, 0, width, height };
    // 自動でサイズを補正する
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

    // ウィンドウタイトルをwstring(wchar_t)に変換
    std::wstring wstrTitle = Convert::CharToWString(title);

    // ウィンドウオブジェクトの生成
    hwnd = CreateWindow(
        w.lpszClassName,      // クラス名
        wstrTitle.c_str(),    // タイトルバーの文字
        WS_OVERLAPPEDWINDOW,  // 標準的なウィンドウスタイル
        CW_USEDEFAULT,        // 表示X座標 (OSに任せる)
        CW_USEDEFAULT,        // 表示Y座標 (OSに任せる)
        wrc.right - wrc.left, // ウィンドウ横幅
        wrc.bottom - wrc.top, // ウィンドウ縦幅
        nullptr,              // 親ウィンドウハンドル
        nullptr,              // メニューハンドル
        w.hInstance,          // 呼び出しアプリケーションハンドル
        nullptr               // オプション
    );

    // ウィンドウを表示状態にする
    ShowWindow(hwnd, SW_SHOW);
}

void mWindow::DelWindow()
{
    UnregisterClass(w.lpszClassName, w.hInstance);
}

void mWindow::SetDisplay(int nCmdShow)
{
    ShowWindow(hwnd, nCmdShow);
}

bool mWindow::IsKeep()
{
    // メッセージ
    MSG msg{};

    // メッセージがある？
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);     // キー入力メッセージの処理
        DispatchMessage(&msg);      // プロシージャにメッセージを送る
    }

    // ✖ボタンで終了メッセージが来たらゲームループを抜ける
    return msg.message != WM_QUIT;
}
