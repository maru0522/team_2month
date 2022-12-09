#include "GameSystem.h"
#include "Obj3d.h"
#include "Sprite.h"

GameSystem::GameSystem(void)
{
}

void GameSystem::Initialize(void)
{
    BasicFrame::Initialize();
}

void GameSystem::Update(void)
{
    BasicFrame::Update();
}

void GameSystem::Draw(void)
{
    iDX_->PreDraw();

    // -----3Dオブジェクトの描画----- //
    Obj3d::PreDraw();

    // -----3Dオブジェクトの描画----- //

    // ---------2DUIの描画---------
    Sprite::PreDraw();

    // ---------2DUIの描画---------

    iDX_->PostDraw();
}

void GameSystem::Finalize(void)
{
    BasicFrame::Finalize();
}
