#include "BasicFrame.h"
#include "Input.h"
#include "Texture.h"
#include "GraphicsPipeline.h"

void BasicFrame::Run(void)
{
    Initialize();

    while (true)
    {
        Update();

        if (EndRequest()) {
            break;
        }

        Draw();
    }

    Finalize();
}

void BasicFrame::Initialize(void)
{
    // WinAPI初期化
    wnd_ = Window::GetInstance();

    // DirectX初期化
    iDX_ = InitDirectX::GetInstance();
    iDX_->Initialize();

    // Input初期化
    KEYS::Initialize();
    DPAD::Initialize();
    XPAD::Initialize();

    // graphicsPipeline初期化
    GraphicsPipeline::Initialize();

    imGuiController_ = std::make_unique<ImGuiController>();

    // テクスチャ初期化
    Texture::Initialize();
}

void BasicFrame::Update(void)
{
    // キーボード情報の取得
    KEYS::Update();
    DPAD::Update();
    XPAD::Update();
}

void BasicFrame::Draw(void)
{
}

void BasicFrame::Finalize(void)
{
    // ウィンドウクラスを登録解除
    wnd_->DelWindow();
}

bool BasicFrame::EndRequest(void)
{
    return !wnd_->GetKeepWindow();
}
