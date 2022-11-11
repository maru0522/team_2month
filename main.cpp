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


#pragma region テクスチャ読み込み

    Texture::Initialize();
    Texture::Load("Resources/reimu.png");
    Texture::Load("Resources/mario.jpg");

#pragma endregion

    Camera tmpCam;

    //Sprite test{ "Resources/mario.jpg" , CMode::PATH };
    ////test.SetPosition({ 200,200 });
    //Sprite test2{ "Resources/reimu.png" , CMode::PATH };
    //test2.SetPosition({ 50,50 });

    Cube tmp{ "Resources/mario.jpg" , &tmpCam };
    //Cube tmp2{ "Resources/mario.jpg" };

#pragma endregion


    // ゲームループ
    while (true) {

#pragma region ウィンドウメッセージ処理

        if (!wnd->GetKeepWindow()) {
            break;
        }

#pragma endregion

#pragma region DirextX毎フレーム処理
        // DirectX毎フレーム処理　ここから

#pragma region キーボード情報の取得

        KEYS::Update();

#pragma endregion

        if (KEYS::IsDown(DIK_A)) {
            tmpCam.eye_.x -= 2;
        }
        if (KEYS::IsDown(DIK_D)) {
            tmpCam.eye_.x += 2;
        }

        if (KEYS::IsDown(DIK_LEFTARROW)) {
            tmpCam.target_.x -= 2;
        }
        if (KEYS::IsDown(DIK_RIGHTARROW)) {
            tmpCam.target_.x += 2;
        }

        tmpCam.Update();
        //test.Update(); 
        //test2.Update();

        tmp.Update();
        //tmp2.Update();

        // DirectX毎フレーム処理　ここまで
#pragma endregion

#pragma region 描画開始

        iDX->PreDraw();

#pragma endregion

        //test.Draw();
        //test2.Draw();

        tmp.Draw();
        //tmp2.Draw();

        // ４．描画コマンドここまで

#pragma region 描画終了

        iDX->PostDraw();

#pragma endregion
    }

    // ウィンドウクラスを登録解除
    wnd->DelWindow();

    return 0;
}