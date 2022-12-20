#include "GameSystem.h"
#include "Obj3d.h"
#include "Sprite.h"
#include "GameScene.h"
#include "TitleScene.h"

GameSystem::GameSystem(void)
{
}

void GameSystem::Initialize(void)
{
    BasicFrame::Initialize();

    scene_ = std::make_unique<TitleScene>();
    scene_->Initialize();
}

void GameSystem::Update(void)
{
    BasicFrame::Update();
    scene_->Update();
}

void GameSystem::Draw(void)
{
    iDX_->PreDraw();

    // -----3Dオブジェクトの描画----- //
    Obj3d::PreDraw();
    scene_->Draw3d();
    // -----3Dオブジェクトの描画----- //

    // ---------2DUIの描画---------
    Sprite::PreDraw();
    scene_->Draw2d();

    // ---------2DUIの描画---------

    iDX_->PostDraw();
}

void GameSystem::Finalize(void)
{
    scene_->Finalize();
    scene_.reset();

    BasicFrame::Finalize();
}
