#include "GameScene.h"
#include "Texture.h"
#include "Model.h"
#include "Input.h"
#include "SceneManager.h"
#include "Stage.h"
#include "BlockManager.h"

void GameScene::Initialize(SceneManager* pSceneManager)
{
    BaseScene::Initialize(pSceneManager);


    // 変数初期化
    cameraT_ = std::make_unique<Camera>();

    Stage::LoadCsv(cameraT_.get(), "Resources/Csv/stage2.csv");

    cameraT_->eye_ = { -50.f, 70.f, -20.f };
    cameraT_->target_ = { 30 ,-4, Stage::maxBlockPosZValue_ / 2.f };

    player_ = std::make_unique<Player>(cameraT_.get());
    player_->SetPos({ 0.0f,4.0f,0.0f });

}

void GameScene::Update(void)
{
    if (KEYS::IsTrigger(DIK_R)) {
        std::unique_ptr<BaseScene> nextScene{ sceneManager_->CreateScene("GAMEPLAY") };
        sceneManager_->RequestChangeScene(nextScene);
    }

    if (KEYS::IsTrigger(DIK_0)) {
        std::unique_ptr<BaseScene> nextScene{ sceneManager_->CreateScene("TITLE") };
        sceneManager_->RequestChangeScene(nextScene);
    }

    // ホットリロード
    if (KEYS::IsTrigger(DIK_5)) {
        BlockManager::ClearAll();
        Stage::LoadCsv(cameraT_.get(), "Resources/Csv/stage2.csv");
    }

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

    if (KEYS::IsDown(DIK_LSHIFT)) {
        if (KEYS::IsDown(DIK_UPARROW)) {
            cameraT_->eye_.y += 2;
            cameraT_->target_.y += 2;
        }
        if (KEYS::IsDown(DIK_DOWNARROW)) {
            cameraT_->eye_.y -= 5;
            cameraT_->target_.y -= 5;
        }
    }

    if (KEYS::IsDown(DIK_NUMPAD8)) {
        cameraT_->eye_.z += 5;
        cameraT_->target_.z += 5;
    }
    if (KEYS::IsDown(DIK_NUMPAD2)) {
        cameraT_->eye_.z -= 5;
        cameraT_->target_.z -= 5;
    }

    if (KEYS::IsDown(DIK_RIGHTARROW)) {
        cameraT_->eye_.x += 5;
        cameraT_->target_.x += 5;
    }
    if (KEYS::IsDown(DIK_LEFTARROW)) {
        cameraT_->eye_.x -= 5;
        cameraT_->target_.x -= 5;
    }
    cameraT_->eye_.z = { player_->GetObject3d()->worldCoordinate_.position_.z - cameraT_->target_.z };
    cameraT_->Update();
    player_->Update();


    BlockManager::Update();

    reset_->Update();

    cameraT_->eye_.z = { player_->GetObject3d()->worldCoordinate_.position_.z - cameraT_->target_.z };
}

void GameScene::Draw3d(void)
{
    player_->Draw3d();


    BlockManager::Draw();
}

void GameScene::Draw2d(void)
{
    reset_->Draw();
    player_->Draw2d();
}

void GameScene::Finalize(void)
{
}