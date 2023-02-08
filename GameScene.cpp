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

    targetPoint_ = std::make_unique<Obj3d>("Resources/3dModels/cube/cube.obj", cameraT_.get());

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

    //cameraT_->eye_ = { -100.f, 60.f, -100.f };
    cameraT_->target_ = { Stage::maxBlockPosValue_.x / 2, -20.f, Stage::maxBlockPosValue_.z / 2 };

    //cameraPosY = cameraT_->eye_.y;

    targetPoint_->worldCoordinate_.position_ = cameraT_->target_;

    player_ = std::make_unique<Player>(cameraT_.get());
    player_->SetPos({ 0.0f,4.0f,0.0f });

    wireString_->SetCutStartPoint({ 297,0 });
    wireString_->SetCutLength({ 215,64 });
    wireString_->SetPosition({ 15,516 });
    wireString_->SetSize({ 215,64 });

    for (size_t i = 0; i < wireArray_.size(); i++) {
        wireArray_.at(i) = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);
    }
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
        std::unique_ptr<BaseScene> nextScene{ sceneManager_->CreateScene("STAGESELECT") };
        sceneManager_->RequestChangeScene(nextScene);
    }

#ifdef _DEBUG

    // ホットリロード
    if (KEYS::IsTrigger(DIK_5)) {
        BlockManager::ClearAll();
        Stage::LoadCsv(cameraT_.get(), "Resources/Csv/stage2.csv");
    }

    //if (KEYS::IsDown(DIK_LSHIFT)) {
    //    if (KEYS::IsDown(DIK_UPARROW)) {
    //        cameraT_->eye_.y += 2;
    //        cameraT_->target_.y += 2;
    //    }
    //    if (KEYS::IsDown(DIK_DOWNARROW)) {
    //        cameraT_->eye_.y -= 5;
    //        cameraT_->target_.y -= 5;
    //    }
    //}

    //if (KEYS::IsDown(DIK_NUMPAD8)) {
    //    cameraT_->eye_.z += 5;
    //    cameraT_->target_.z += 5;
    //}
    //if (KEYS::IsDown(DIK_NUMPAD2)) {
    //    cameraT_->eye_.z -= 5;
    //    cameraT_->target_.z -= 5;
    //}

    //if (KEYS::IsDown(DIK_RIGHTARROW)) {
    //    cameraT_->eye_.x += 5;
    //    cameraT_->target_.x += 5;
    //}
    //if (KEYS::IsDown(DIK_LEFTARROW)) {
    //    cameraT_->eye_.x -= 5;
    //    cameraT_->target_.x -= 5;
    //}
#endif // _DEBUG

    if (isAutoCameraMode_) {
        // 中央（注視点）からプレイヤーまでのベクトル算出
        DirectX::XMFLOAT3 tagToPlayer{
            player_->GetObject3d()->worldCoordinate_.position_.x - cameraT_->target_.x,
            player_->GetObject3d()->worldCoordinate_.position_.y - cameraT_->target_.y,
            player_->GetObject3d()->worldCoordinate_.position_.z - cameraT_->target_.z
        };
        // 該当ベクトルを正規化
        float ttpLength{ std::sqrtf(tagToPlayer.x * tagToPlayer.x + tagToPlayer.y * tagToPlayer.y + tagToPlayer.z * tagToPlayer.z) };
        DirectX::XMFLOAT3 ttpNormalized{};
        if (ttpLength != 0) {
            ttpNormalized.x = tagToPlayer.x / ttpLength;
            ttpNormalized.y = tagToPlayer.y / ttpLength;
            ttpNormalized.z = tagToPlayer.z / ttpLength;
        }

        cameraT_->eye_.x = cameraT_->target_.x + ttpNormalized.x * distancePlayerToCamera_;
        cameraT_->eye_.y = cameraT_->target_.y + ttpNormalized.y * distancePlayerToCamera_;
        cameraT_->eye_.z = cameraT_->target_.z + ttpNormalized.z * distancePlayerToCamera_;

       /* cameraT_->eye_.x = (player_->GetObject3d()->worldCoordinate_.position_.x - cameraT_->target_.z);
        cameraT_->eye_.z = (player_->GetObject3d()->worldCoordinate_.position_.z - cameraT_->target_.x) * cameraSpeed;

        cameraT_->eye_.y = (player_->GetObject3d()->worldCoordinate_.position_.y + cameraPosY);*/
    }
    else {

    }


    cameraT_->Update();
    player_->Update();


    BlockManager::Update();

    //targetPoint_->Update();

    reset_->Update();
    bokashi_->Update();
    wireString_->Update();
}

void GameScene::Draw3d(void)
{
    player_->Draw3d();


    BlockManager::Draw();

    //targetPoint_->Draw();
}

void GameScene::Draw2d(void)
{
    reset_->Draw();
    player_->Draw2d();
    bokashi_->Draw();
    wireString_->Draw();
}

void GameScene::Finalize(void)
{
}