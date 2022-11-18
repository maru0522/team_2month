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
    // WinAPI初期化
    Window* wnd = Window::GetInstance();

    // DirectX初期化
    InitDirectX* iDX = InitDirectX::GetInstance();
    iDX->Initialize();

    // Input初期化
    KEYS::Initialize();
    DPAD::Initialize();

#pragma region 描画初期化処理
    // graphicsPipeline初期化
    GraphicsPipeline::Initialize();

    Obj3d::Initialize();
#pragma endregion

#pragma region リソース読み込み
    // テクスチャ読み込み
    Texture::Initialize();
    Texture::Load("Resources/reimu.png");
    Texture::Load("Resources/mario.jpg");

    // 3d.obj読み込み
    Model::Load("Resources/3dModels/cube/untitled.obj");

#pragma endregion

    Camera cameraT{};

    // 使用する変数宣言
    Obj3d objT{ "Resources/3dModels/cube/untitled.obj", &cameraT };
    Obj3d objT2{ "Resources/3dModels/cube/untitled.obj", &cameraT };

    objT2.worldCoordinate_.position_ = { 15,0,0 };

    // ゲームループ
    while (true) {
        // ウィンドウメッセージ処理
        if (!wnd->GetKeepWindow()) {
            break;
        }

#pragma region DirextX毎フレーム処理
        // 更新処理　ここから

        // キーボード情報の取得
        KEYS::Update();
        DPAD::Update();

        cameraT.Update();

        if (KEYS::IsDown(DIK_W)) {
            cameraT.eye_.z -= 5;
        }
        if (KEYS::IsDown(DIK_S)) {
            cameraT.eye_.z += 5;
        }
        if (KEYS::IsDown(DIK_A)) {
            cameraT.eye_.x -= 5;
        }
        if (KEYS::IsDown(DIK_D)) {
            cameraT.eye_.x += 5;
        }

        if (KEYS::IsDown(DIK_LEFTARROW)) {
            objT.worldCoordinate_.position_.x -= 2;
        }
        if (KEYS::IsDown(DIK_RIGHTARROW)) {
            objT.worldCoordinate_.position_.x += 2;
        }

        objT.Update();
        objT2.Update();

        // 更新処理　ここまで
#pragma endregion

#pragma region 描画開始
        iDX->PreDraw();
#pragma endregion

        // 描画処理　ここから
        Obj3d::PreDraw();

        objT.Draw();
        objT2.Draw();

        // 描画処理　ここまで

#pragma region 描画終了
        iDX->PostDraw();
#pragma endregion
    }
    // ウィンドウクラスを登録解除
    wnd->DelWindow();

    return 0;
}