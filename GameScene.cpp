#include "GameScene.h"
#include "Texture.h"
#include "Model.h"
#include "Input.h"

void GameScene::Initialize(SceneManager* pSceneManager)
{
    BaseScene::Initialize(pSceneManager);

    // •Ï”‰Šú‰»
    cameraT_ = std::make_unique<Camera>();
    cameraT_->eye_ = { 0,0, -100 };
    cameraT_->target_ = { 0 ,0, 0 };
    player_ = std::make_unique<Player>(cameraT_.get());
}

void GameScene::Update(void)
{

    //if (KEYS::IsDown(DIK_W)) {
    //    cameraT_->eye_.z += 5;
    //}
    //if (KEYS::IsDown(DIK_S)) {
    //    cameraT_->eye_.z -= 5;
    //}
    //if (KEYS::IsDown(DIK_A)) {
    //    cameraT_->eye_.x -= 5;
    //}
    //if (KEYS::IsDown(DIK_D)) {
    //    cameraT_->eye_.x += 5;
    //}
    if (KEYS::IsDown(DIK_UPARROW)) {
        cameraT_->eye_.y += 5;
        cameraT_->target_.y += 5;
    }
    if (KEYS::IsDown(DIK_DOWNARROW)) {
        cameraT_->eye_.y -= 5;
        cameraT_->target_.y -= 5;
    }

    if (KEYS::IsDown(DIK_RIGHTARROW)) {
        cameraT_->eye_.x += 5;
        cameraT_->target_.x += 5;
    }

    cameraT_->Update();
    player_->Update();
}

void GameScene::Draw3d(void)
{
    player_->Draw();
}

void GameScene::Draw2d(void)
{
    player_->Draw2D();
}

void GameScene::Finalize(void)
{
}
