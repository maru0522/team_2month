#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<d3dcompiler.h>
#include<DirectXMath.h>
#include<math.h>
using namespace DirectX;
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include<vector>
#include<string>
#include<DirectXTex.h>
#include<wrl.h>

#include "Window.h"
#include "Input.h"
#include "InitDirectX.h"
#include "Texture.h"
#include "Sprite.h"
#include "Cube.h"
#include "Camera.h"
#include "Model.h"
#include "Obj3d.h"

using namespace Microsoft::WRL;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

#pragma region WindowsAPI初期化

    Window* wnd = Window::GetInstance();

#pragma endregion

#pragma region DirectX初期化

    InitDirectX* iDX = InitDirectX::GetInstance();
    iDX->Initialize();

#pragma region DirectInput初期化

    KEYS::Initialize();

#pragma endregion
#pragma endregion

#pragma region 描画初期化処理
    // テクスチャ読み込み
    Texture::Initialize();
    Texture::Load("Resources/reimu.png");
    Texture::Load("Resources/mario.jpg");

    // 3d.obj読み込み
    Model::Load("Resources/3dModels/cube/untitled.obj");
#pragma endregion

    // 使用する変数宣言
    Obj3d objT{ "Resources/3dModels/cube/untitled.obj" };


    // ゲームループ
    while (true) {
#pragma region ウィンドウメッセージ処理
        if (!wnd->GetKeepWindow()) {
            break;
        }
#pragma endregion

#pragma region DirextX毎フレーム処理
        // 更新処理　ここから

        // キーボード情報の取得
        KEYS::Update();


        objT.Update();

        // 更新処理　ここまで
#pragma endregion

#pragma region 描画開始

        iDX->PreDraw();

#pragma endregion
        // 描画処理　ここから
        Obj3d::PreDraw();

        objT.Draw();

        // 描画処理　ここまで
#pragma region 描画終了

        iDX->PostDraw();

#pragma endregion
    }

    // ウィンドウクラスを登録解除
    wnd->DelWindow();

    return 0;
}