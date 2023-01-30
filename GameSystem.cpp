#include "GameSystem.h"
#include "Obj3d.h"
#include "Sprite.h"
#include "ResourceLoad.h"
#include "TitleScene.h"
#include "ImGuiController.h"

GameSystem::GameSystem(void)
{
}

void GameSystem::Initialize(void)
{
    BasicFrame::Initialize();

    scene_ = std::make_unique<ResourceLoad>();
    sceneManager_->RequestChangeScene(scene_);
}

void GameSystem::Update(void)
{
    BasicFrame::Update();
}

void GameSystem::Draw(void)
{
    iDX_->PreDraw();

    // -----3D�I�u�W�F�N�g�̕`��----- //
    Obj3d::PreDraw();
    sceneManager_->Draw3d();

    // -----3D�I�u�W�F�N�g�̕`��----- //

    // ---------2DUI�̕`��---------
    Sprite::PreDraw();
    sceneManager_->Draw2d();

    // ---------2DUI�̕`��---------

    //imGuiController_->Draw();

    iDX_->PostDraw();
}

void GameSystem::Finalize(void)
{
    BasicFrame::Finalize();
}