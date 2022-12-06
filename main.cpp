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

#include "ImGuiController.h"

using namespace Microsoft::WRL;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#pragma region 基礎機能の初期化
    // WinAPI初期化
    Window* wnd = Window::GetInstance();

    // DirectX初期化
    InitDirectX* iDX = InitDirectX::GetInstance();
    iDX->Initialize();

    // Input初期化
    KEYS::Initialize();
    DPAD::Initialize();

    // graphicsPipeline初期化
    GraphicsPipeline::Initialize();

    std::unique_ptr<ImGuiController> imGuiCtrller = std::make_unique<ImGuiController>();
#pragma endregion

#pragma region リソース読み込み
    // テクスチャ読み込み
    Texture::Initialize();
    Texture::Load("Resources/reimu.png");
    Texture::Load("Resources/mario.jpg");

    // 3d.obj読み込み
    Model::Load("Resources/3dModels/cube/cube.obj");

#pragma endregion

    Camera cameraT{};

    // 使用する変数宣言
    Sprite spriteT{ "Resources/reimu.png" ,CMode::PATH };
    //spriteT.SetSize({100.0f, 100.0f});
    spriteT.SetPosition({ 100,100 });

    Obj3d objT{ "Resources/3dModels/cube/cube.obj", &cameraT };
    Obj3d objT2{ "Resources/3dModels/cube/cube.obj", &cameraT };

    objT2.worldCoordinate_.position_ = { 15,0,0 };
    objT2.SetTexture("Resources/reimu.png");

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
            cameraT.eye_.z += 5;
        }
        if (KEYS::IsDown(DIK_S)) {
            cameraT.eye_.z -= 5;
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

        spriteT.Update();
        objT.Update();
        objT2.Update();

        //imGuiCtrller->Update();
        static float xy[2]{spriteT.GetPosition().x,spriteT.GetPosition().y };

        imGuiCtrller->Begin();
        imGuiCtrller->SpriteSlider(xy);
        imGuiCtrller->End();

        spriteT.SetPosition({ xy[0], xy[1] });
        // 更新処理　ここまで
#pragma endregion

#pragma region 描画開始
        iDX->PreDraw();
#pragma endregion
        // 描画処理　ここから
        Obj3d::PreDraw();

        objT.Draw();
        objT2.Draw();

        Sprite::PreDraw();
        spriteT.Draw();

        imGuiCtrller->Draw();
        // 描画処理　ここまで

#pragma region 描画終了
        iDX->PostDraw();
#pragma endregion
    }
    // ウィンドウクラスを登録解除
    wnd->DelWindow();

    return 0;
}