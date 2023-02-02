#include "GameScene.h"
#include "Texture.h"
#include "Model.h"
#include "Input.h"
#include "SceneManager.h"
#include "Stage.h"
#include "BlockManager.h"
#include "StageSelectScene.h"

void GameScene::Initialize(SceneManager* pSceneManager)
{
    BaseScene::Initialize(pSceneManager);


    // 変数初期化
    cameraT_ = std::make_unique<Camera>();

    switch (StageSelectScene::GetStageIdx())
    {
    case 1:
        BlockManager::ClearAll();
        Stage::LoadCsv(cameraT_.get(), "Resources/Csv/stage1.csv");
        break;
    case 2:
        BlockManager::ClearAll();
        Stage::LoadCsv(cameraT_.get(), "Resources/Csv/stage2.csv");
        break;
    case 3:
        BlockManager::ClearAll();
        Stage::LoadCsv(cameraT_.get(), "Resources/Csv/stage3.csv");
        break;
    case 4:
        BlockManager::ClearAll();
        Stage::LoadCsv(cameraT_.get(), "Resources/Csv/stage4.csv");
        break;
    case 5:
        BlockManager::ClearAll();
        Stage::LoadCsv(cameraT_.get(), "Resources/Csv/stage5.csv");
        break;
    case 6:
        BlockManager::ClearAll();
        Stage::LoadCsv(cameraT_.get(), "Resources/Csv/stage6.csv");
        break;
    default:
        BlockManager::ClearAll();
        Stage::LoadCsv(cameraT_.get(), "Resources/Csv/tutorial.csv");
        break;
    }

    cameraT_->eye_ = { -100.f, 70.f, -100.f };
    cameraT_->target_ = { Stage::maxBlockPosValue_.x / 2.f ,-4, Stage::maxBlockPosValue_.z / 2.f };


    player_ = std::make_unique<Player>(cameraT_.get());
    player_->SetPos({ 0.0f,4.0f,0.0f });
}

void GameScene::Update(void)
{
    if (KEYS::IsTrigger(DIK_R) || XPAD::IsTrigger(XPAD_Y)) {
        BlockManager::ClearAll();
        std::unique_ptr<BaseScene> nextScene{ sceneManager_->CreateScene("GAMEPLAY") };
        sceneManager_->RequestChangeScene(nextScene);
    }

    if (KEYS::IsTrigger(DIK_0)) {
        BlockManager::ClearAll();
        std::unique_ptr<BaseScene> nextScene{ sceneManager_->CreateScene("TITLE") };
        sceneManager_->RequestChangeScene(nextScene);
    }

    // プレイヤーが落下したら強制リセット（初期足場から５ブロック下の座標以下になったとき）
    if (player_->GetObject3d()->worldCoordinate_.position_.y <= -10.f) {
        BlockManager::ClearAll();
        std::unique_ptr<BaseScene> nextScene{ sceneManager_->CreateScene("GAMEPLAY") };
        sceneManager_->RequestChangeScene(nextScene);
    }

    // プレイヤーがクリアしたら強制リセット（ゴールブロックを踏んだら強制リセット）
    if (player_->GetIsGoal()) {
        BlockManager::ClearAll();
        std::unique_ptr<BaseScene> nextScene{ sceneManager_->CreateScene("GAMEPLAY") };
        sceneManager_->RequestChangeScene(nextScene);
    }

#ifdef _DEBUG

    // ホットリロード
    if (KEYS::IsTrigger(DIK_5)) {
        BlockManager::ClearAll();
        Stage::LoadCsv(cameraT_.get(), "Resources/Csv/stage2.csv");
    }

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
#endif // _DEBUG

    if (isAutoCameraMode_) {
        cameraT_->eye_.x = (player_->GetObject3d()->worldCoordinate_.position_.x - cameraT_->target_.z);
        cameraT_->eye_.z = (player_->GetObject3d()->worldCoordinate_.position_.z - cameraT_->target_.x) * 1.2f;
    }
    else {

    }

    //cameraT_->eye_.z = { player_->GetObject3d()->worldCoordinate_.position_.z - cameraT_->target_.z };
    cameraT_->Update();
    player_->Update();


    BlockManager::Update();

    reset_->Update();


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