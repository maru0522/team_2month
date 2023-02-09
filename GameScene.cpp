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

    for (size_t i = 0; i < arrow_.size(); i++)
    {
        arrow_.at(i) = std::make_unique<Obj3d>("Resources/3dModels/hookShadow/hookShadow.obj", cameraT_.get());
        /*arrow_[i]->worldCoordinate_.scale_ = { 4,4,4 };*/
    }

    switch (StageSelectScene::GetStageIdx())
    {
    case 1:
        BlockManager::ClearAll();
        Stage::LoadCsv(cameraT_.get(), "Resources/Csv/stage1.csv");
        cameraT_->target_ = { 30.f, -20.f,60.f };
        break;
    case 2:
        BlockManager::ClearAll();
        Stage::LoadCsv(cameraT_.get(), "Resources/Csv/stage2.csv");
        cameraT_->target_ = { 40.f, -20.f,80.f };
        arrow_[0]->worldCoordinate_.position_ = { 0,1,14 };
        arrow_[1]->worldCoordinate_.position_ = { 0,1,90 };
        arrow_[2]->worldCoordinate_.position_ = {0,15,64};
        break;
    case 3:
        BlockManager::ClearAll();
        Stage::LoadCsv(cameraT_.get(), "Resources/Csv/stage3.csv");
        cameraT_->target_ = { 40.f, -20.f,60.f };
        arrow_[0]->worldCoordinate_.position_ = { 0,1,20 };
        arrow_[1]->worldCoordinate_.position_ = { 0,1,72 };
        break;
    case 4:
        BlockManager::ClearAll();
        Stage::LoadCsv(cameraT_.get(), "Resources/Csv/stage4.csv");
        cameraT_->target_ = { 40.f, -20.f,60.f };
        arrow_[0]->worldCoordinate_.position_ = { 2,1,54 };
        arrow_[1]->worldCoordinate_.position_ = { 0,15,0 };
        break;
    case 5:
        BlockManager::ClearAll();
        Stage::LoadCsv(cameraT_.get(), "Resources/Csv/stage5.csv");
        cameraT_->target_ = { 40.f, -20.f,60.f };
        arrow_[0]->worldCoordinate_.position_ = { 2,1,28 };
        arrow_[1]->worldCoordinate_.position_ = { 2,1,58 };
        arrow_[2]->worldCoordinate_.position_ = { 0,1,88 };
        break;
    case 6:
        BlockManager::ClearAll();
        Stage::LoadCsv(cameraT_.get(), "Resources/Csv/stage8.csv");
        cameraT_->target_ = { Stage::maxBlockPosValue_.x / 2, -20.f, Stage::maxBlockPosValue_.z / 2 };

        break;
    default:
        BlockManager::ClearAll();
        Stage::LoadCsv(cameraT_.get(), "Resources/Csv/tutorial.csv");
        cameraT_->target_ = { 30.f, -20.f,50.f };
        break;
    }

  

    /*cameraT_->eye_ = { -100.f, 60.f, -100.f };
    cameraT_->target_ = { Stage::maxBlockPosValue_.x / 2, -4.f, Stage::maxBlockPosValue_.z / 2 };
    cameraPosY = cameraT_->eye_.y;*/

    //cameraT_->target_ = { Stage::maxBlockPosValue_.x / 2, -20.f, Stage::maxBlockPosValue_.z / 2 };

    targetPoint_->worldCoordinate_.position_ = cameraT_->target_;

    player_ = std::make_unique<Player>(cameraT_.get());
    player_->SetPos({ 0.0f,4.0f,0.0f });

    wireString_->SetCutStartPoint({ 297,0 });
    wireString_->SetCutLength({ 215,64 });
    wireString_->SetPosition({ 15,516 });
    wireString_->SetSize({ 215,64 });

    for (size_t i = 0; i < wireArray_.size(); i++) {
        wireArray_.at(i) = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);
        wireArray_.at(i)->SetSize({ 32,16 });
        wireArray_.at(i)->SetPosition({ 15 + i * 32.f, 580 });
    }
    reset_->SetPosition({10,10});
    select_->SetPosition({10,70});

    skydome_ = std::make_unique<Obj3d>("Resources/3dModels/skydome/skydome.obj", cameraT_.get());
    skydome_->worldCoordinate_.scale_ = { 30,30,30 };
    tutorial1_->SetPosition({ 250,600 });
    tutorial2_->SetPosition({ 250,600 });
    tutorial3_->SetPosition({ 250,600 });

    tutorial1_->SetSize({ 800,96});
    tutorial2_->SetSize({ 800,96 });
    tutorial3_->SetSize({ 800,96 });
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

    // プレイヤーがクリアしたらへステージセレクト（ゴールブロックを踏んだらステージセレクト）
    if (goalTimer<0.0f|| XPAD::IsTrigger(XPAD_START)) {
        BlockManager::ClearAll();
        std::unique_ptr<BaseScene> nextScene{ sceneManager_->CreateScene("STAGESELECT") };
        sceneManager_->RequestChangeScene(nextScene);
    }
    if (player_->GetIsGoal())
    {
        goalTimer--;
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

    if (XPAD::IsTrigger(XPAD_LB))
    {
        if (isAutoCameraMode_)
        {
            isAutoCameraMode_ = false;
        }
        else
        {
            isAutoCameraMode_ = true;
        }
    }

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

        /*cameraT_->eye_.x = (player_->GetObject3d()->worldCoordinate_.position_.x - cameraT_->target_.z);
        cameraT_->eye_.z = (player_->GetObject3d()->worldCoordinate_.position_.z - cameraT_->target_.x) * cameraSpeed;

        cameraT_->eye_.y = (player_->GetObject3d()->worldCoordinate_.position_.y + cameraPosY);*/
    }
    else {
        cameraT_->eye_ ={ -150.f, 60.f, 50.f };
    }

    skydome_->Update();;

    cameraT_->Update();
    player_->Update();

    for (size_t i = 0; i < arrow_.size(); i++)
    {
        arrow_.at(i)->Update();
    }

    BlockManager::Update();

    //targetPoint_->Update();

    reset_->Update();
    select_->Update();
    bokashi_->Update();
    wireString_->Update();
    tutorial1_->Update();
    tutorial2_->Update();
    tutorial3_->Update();
    for (size_t i = 0; i < wireArray_.size(); i++) {
        wireArray_.at(i)->Update();
    }
}

void GameScene::Draw3d(void)
{
    skydome_->Draw();
    player_->Draw3d();

  
    switch (StageSelectScene::GetStageIdx())
    {
    case 1:
       
        break;
    case 2:
          arrow_[0]->Draw();
          arrow_[1]->Draw();
          arrow_[2]->Draw();
        
        break;
    case 3:
        arrow_[0]->Draw();
        arrow_[1]->Draw();
        break;
    case 4:
        arrow_[0]->Draw();
        arrow_[1]->Draw();
        break;
    case 5:
        arrow_[0]->Draw();
        arrow_[1]->Draw();
        arrow_[2]->Draw();
        break;
    case 6:
       
        break;
    default:
       
        break;
    }
    BlockManager::Draw();

    //targetPoint_->Draw();
}

void GameScene::Draw2d(void)
{
    reset_->Draw();
    select_->Draw();
    player_->Draw2d();
    bokashi_->Draw();
    wireString_->Draw();
    if (StageSelectScene::GetStageIdx() == 0)
    {
        if (player_->GetTutorialCount() == 0)
        {
            
            tutorial1_->Draw();
        }
        else if (player_->GetTutorialCount() == 1)
        {
            tutorial2_->Draw();
        }
        else
        {
            tutorial3_->Draw();
        }
    }
    for (size_t i = 0; i < wireArray_.size(); i++) {
        if (i <= static_cast<size_t>( player_->GetRopeLength() - 1.f)) {
            wireArray_.at(i)->Draw();
        }
    }
}

void GameScene::Finalize(void)
{
}