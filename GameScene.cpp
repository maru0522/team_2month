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
<<<<<<< HEAD

    Stage::LoadCsv(cameraT_.get(), "Resources/Csv/stage2.csv");

    cameraT_->eye_ = { -50.f, 70.f, -50.f };
    cameraT_->target_ = { 30 ,-4, Stage::maxBlockPosZValue_ / 2.f };

    player_ = std::make_unique<Player>(cameraT_.get());
    player_->SetPos({ 0.0f,4.0f,0.0f });

=======
    spriteT_ = std::make_unique<Sprite>("Resources/reimu.png", CMode::PATH);
    objT_ = std::make_unique<Obj3d>("Resources/3dModels/cube/cube.obj", cameraT_.get());
>>>>>>> parent of 5499b30 (player縺ｧ縺阪◆)
}

void GameScene::Update(void)
{
<<<<<<< HEAD
#ifdef _DEBUG
    if (KEYS::IsTrigger(DIK_R) || XPAD::IsTrigger(XPAD_Y)) {
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

    cameraT_->eye_.z = { player_->GetObject3d()->worldCoordinate_.position_.z - cameraT_->target_.z };
    cameraT_->Update();
    player_->Update();


    BlockManager::Update();

    reset_->Update();

    cameraT_->eye_.z = { player_->GetObject3d()->worldCoordinate_.position_.z - cameraT_->target_.z };
=======
    cameraT_->Update();

    if (KEYS::IsDown(DIK_W)) {
        cameraT_->eye_.z += 5;
    }
    if (KEYS::IsDown(DIK_S)) {
        cameraT_->eye_.z -= 5;
    }
    if (KEYS::IsDown(DIK_A)) {
        cameraT_->eye_.x -= 5;
    }
    if (KEYS::IsDown(DIK_D)) {
        cameraT_->eye_.x += 5;
    }

    if (KEYS::IsDown(DIK_LEFTARROW)) {
        objT_->worldCoordinate_.position_.x -= 2;
    }
    if (KEYS::IsDown(DIK_RIGHTARROW)) {
        objT_->worldCoordinate_.position_.x += 2;
    }

    spriteT_->Update();
    objT_->Update();
>>>>>>> parent of 5499b30 (player縺ｧ縺阪◆)
}

void GameScene::Draw3d(void)
{
<<<<<<< HEAD
    player_->Draw3d();


    BlockManager::Draw();
=======
    objT_->Draw();
>>>>>>> parent of 5499b30 (player縺ｧ縺阪◆)
}

void GameScene::Draw2d(void)
{
<<<<<<< HEAD
    reset_->Draw();
    player_->Draw2d();
=======
    spriteT_->Draw();
>>>>>>> parent of 5499b30 (player縺ｧ縺阪◆)
}

void GameScene::Finalize(void)
{
}