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
    // WinAPI������
    wnd_ = Window::GetInstance();

    // DirectX������
    iDX_ = InitDirectX::GetInstance();
    iDX_->Initialize();

    // Input������
    Input::Initialize();

    // graphicsPipeline������
    GraphicsPipeline::Initialize();

    imGuiController_ = std::make_unique<ImGuiController>();
    sceneManager_ = std::make_unique<SceneManager>();

    // �e�N�X�`��������
    Texture::Initialize();
}

void BasicFrame::Update(void)
{
    // �L�[�{�[�h���̎擾

    imGuiController_->Begin();
    sceneManager_->Update();
    imGuiController_->End();
}

void BasicFrame::Draw(void)
{
}

void BasicFrame::Finalize(void)
{
    if (imGuiController_) imGuiController_.reset();
    if (sceneManager_) sceneManager_.reset();

    GraphicsPipeline::Finalize();
    Texture::Finalize();
    Model::Finalize();
    Input::Finalize();

    // �E�B���h�E�N���X��o�^����
    wnd_->DelWindow();
}

bool BasicFrame::EndRequest(void)
{
    return !wnd_->GetKeepWindow();
}
