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

    // ïœêîèâä˙âª
    cameraT_ = std::make_unique<Camera>();
    cameraT_->eye_ = { 0,10, -50 };
    cameraT_->target_ = { 0 ,10, 0 };

    player_ = std::make_unique<Player>(cameraT_.get());
    player_->SetPos({ 2.0f,4.0f,2.0f });

    //obj3dT_ = std::make_unique<Obj3d>("Resources/3dModels/cube/cube.obj", cameraT_.get());

    //DirectX::XMFLOAT3 pos{ 2,0,2 };
    //blockT_ = std::make_unique<NormalBlock>(pos, cameraT_.get());

    Stage::LoadCsv(cameraT_.get(),"Resources/Csv/tutorial.csv");

    /*for (size_t i = 0; i < blocks_.size(); i++) {
        if (i < 3) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::START, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 2.0f * i - 2.0f, 0.0f,  -2.0f };
        }
        if (3 <= i && i <= 5) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::START, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 2 * (i % 3) - 2.0f, 0.0f , 0.0f };
        }
        if (6 <= i && i <= 8) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::START, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 2 * (i % 3) - 2.0f, 0.0f , 2.0f };
        }

        if (9 <= i && i <= 14) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::PATH, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 2.0f * i - 14.0f, 0.0f , 0.0f };
        }

        if (15 <= i && i <= 21) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::PATH, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 2.0f * i - 8.0f , 0.0f , 0.0f };
        }

        if (22 <= i && i <= 24) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::PATH, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 2.0f * i - 14.0f, 0.0f , 2.0f };
        }

        if (25 <= i && i <= 27) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::PATH, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 34.0f, 2.0f * (i - 25) + 2.0f , 2.0f };
        }

        if (28 <= i && i <= 31) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::PATH, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 30.0f, 2.0f * (i - 28) , 4.0f };
        }

        if (32 <= i && i <= 35) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::PATH, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 32.0f, 2.0f * (i - 32) , 4.0f };
        }

        if (36 <= i && i <= 39) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::PATH, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 34.0f, 2.0f * (i - 36) , 4.0f };
        }

        if (40 <= i && i <= 50) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::PATH, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 2.0f * (i - 40) + 36.0f, 6.0f , 4.0f };
        }

        if (51 <= i && i <= 51) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::PATH, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 56.0f, 6.0f , 6.0f };
        }

        if (52 <= i && i <= 54) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::PATH, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 2.0f * (i - 52) + 54.0f, 6.0f , 8.0f };
        }

        if (55 <= i && i <= 55) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::PATH, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 48.0f, 8.0f , 6.0f };
        }

        if (56 <= i && i <= 59) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::PATH, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 2.0f * (i - 56) + 46.0f, 10.0f , 8.0f };
        }

        if (60 <= i && i <= 60) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::PATH, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 48.0f, 4.0f , 2.0f };
        }

        if (61 <= i && i <= 64) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::PATH, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 2.0f * (i - 61) + 46.0f, 2.0f , 0.0f };
        }

        if (65 <= i && i <= 66) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::PATH, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 2.0f * (i - 65) + 60.0f, 2.0f , 0.0f };
        }

        if (67 <= i && i <= 70) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::PATH, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 64.0f, 2.0f , 2.0f * (i - 67) + 2.0f };
        }

        if (71 <= i && i <= 74) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::PATH, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 64.0f, 2.0f * (i - 71) + 4.0f, 8.0f };
        }

        if (75 <= i && i <= 76) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::PATH, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 62.0f - 2.0f * (i - 75), 10.0f, 8.0f };
        }

        if (77 <= i && i <= 82) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::PATH, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 56.0f,2.0f * (i - 77) + 6.0f, 10.0f };
        }

        if (83 <= i && i <= 85) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::PATH, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 2.0f * (i - 83) + 64.0f,-2.0f, 0.0f };
        }

        if (86 <= i && i <= 88) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::START, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 2.0f * (i - 86) + 70.0f,-2.0f, -2.0f };
        }

        if (89 <= i && i <= 91) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::START, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 2.0f * (i - 89) + 70.0f,-2.0f, 0.0f };
        }

        if (92 <= i && i <= 94) {
            blocks_[i] = std::make_unique<NormalBlock>(NormalBlock::Type::START, cameraT_.get());
            blocks_[i]->object_->worldCoordinate_.position_ = { 2.0f * (i - 92) + 70.0f,-2.0f, 2.0f };
        }
    }*/
}

void GameScene::Update(void)
{
    if (KEYS::IsTrigger(DIK_R)) {
        std::unique_ptr<BaseScene> nextScene{ sceneManager_->CreateScene("GAMEPLAY") };
        sceneManager_->RequestChangeScene(nextScene);
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
    if (KEYS::IsDown(DIK_UPARROW)) {
        cameraT_->eye_.y += 2;
        cameraT_->target_.y += 2;
    }
    if (KEYS::IsDown(DIK_DOWNARROW)) {
        cameraT_->eye_.y -= 5;
        cameraT_->target_.y -= 5;
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

    cameraT_->Update();
    player_->Update();

    //obj3dT_->Update();
    //blockT_->Update();

    BlockManager::Update();

    reset_->Update();

    cameraT_->eye_.x = player_->GetObject3d()->worldCoordinate_.position_.x;
    cameraT_->target_.x = player_->GetObject3d()->worldCoordinate_.position_.x;
}

void GameScene::Draw3d(void)
{
    player_->Draw();

    //obj3dT_->Draw();
    //blockT_->Draw();

    BlockManager::Draw();
}

void GameScene::Draw2d(void)
{
    reset_->Draw();
}

void GameScene::Finalize(void)
{
}

//void GameScene::Col(void)
//{
//    for (size_t i = 0; i < 95; i++) {
//        if (std::abs(blocks_[i]->object_->worldCoordinate_.position_.x - player_->GetObject3d()->worldCoordinate_.position_.x) <= rangeIntoPlayer_ &&
//            std::abs(blocks_[i]->object_->worldCoordinate_.position_.y - player_->GetObject3d()->worldCoordinate_.position_.y) <= rangeIntoPlayer_ &&
//            std::abs(blocks_[i]->object_->worldCoordinate_.position_.z - player_->GetObject3d()->worldCoordinate_.position_.z) <= rangeIntoPlayer_) {
//            blocks_[i]->Update();
//
//            try {
//                // yï˚å¸
//                if (blocks_[i]->object_->worldCoordinate_.position_.x - (NormalBlock::radius_ + Player::radius_.x) < player_->GetObject3d()->worldCoordinate_.position_.x &&
//                    player_->GetObject3d()->worldCoordinate_.position_.x < blocks_[i]->object_->worldCoordinate_.position_.x + (NormalBlock::radius_ + Player::radius_.x) && // <- or?
//                    blocks_[i]->object_->worldCoordinate_.position_.z - (NormalBlock::radius_ + Player::radius_.z) < player_->GetObject3d()->worldCoordinate_.position_.z &&
//                    player_->GetObject3d()->worldCoordinate_.position_.z < blocks_[i]->object_->worldCoordinate_.position_.z + (NormalBlock::radius_ + Player::radius_.z)
//                    ) {
//                    // è„Ç©ÇÁêNì¸ÇµÇΩèÍçá
//                    if (blocks_[i]->object_->worldCoordinate_.position_.y - NormalBlock::radius_ < player_->GetObject3d()->worldCoordinate_.position_.y - (Player::radius_.y + Player::gravity_) &&
//                        player_->GetObject3d()->worldCoordinate_.position_.y - (Player::radius_.y + Player::gravity_) < blocks_[i]->object_->worldCoordinate_.position_.y + NormalBlock::radius_) {
//                        while ((player_->GetObject3d()->worldCoordinate_.position_.y - Player::radius_.y) - (blocks_[i]->object_->worldCoordinate_.position_.y + NormalBlock::radius_) < 0.0f)
//                        {
//                            player_->SetPosY(player_->GetObject3d()->worldCoordinate_.position_.y + 0.01f);
//                        }
//                        player_->SetFloatFlag(false);
//                    }
//
//                    // â∫Ç©ÇÁêNì¸ÇµÇΩèÍçá
//                    if (blocks_[i]->object_->worldCoordinate_.position_.y - NormalBlock::radius_ < player_->GetObject3d()->worldCoordinate_.position_.y + Player::radius_.y &&
//                        player_->GetObject3d()->worldCoordinate_.position_.y + Player::radius_.y < blocks_[i]->object_->worldCoordinate_.position_.y + NormalBlock::radius_) {
//                        while ((blocks_[i]->object_->worldCoordinate_.position_.y - NormalBlock::radius_) - (player_->GetObject3d()->worldCoordinate_.position_.y + Player::radius_.y) < 0.0f)
//                        {
//                            player_->SetPosY(player_->GetObject3d()->worldCoordinate_.position_.y - 0.01f);
//                        }
//                    }
//                }
//            }
//            catch() {
//
//            }
//
//            // xï˚å¸
//            if (blocks_[i]->object_->worldCoordinate_.position_.y - (NormalBlock::radius_ + Player::radius_.y) < player_->GetObject3d()->worldCoordinate_.position_.y &&
//                player_->GetObject3d()->worldCoordinate_.position_.y < blocks_[i]->object_->worldCoordinate_.position_.y + (NormalBlock::radius_ + Player::radius_.y) && // <- or?
//                blocks_[i]->object_->worldCoordinate_.position_.z - (NormalBlock::radius_ + Player::radius_.z) < player_->GetObject3d()->worldCoordinate_.position_.z &&
//                player_->GetObject3d()->worldCoordinate_.position_.z < blocks_[i]->object_->worldCoordinate_.position_.z + (NormalBlock::radius_ + Player::radius_.z)
//                ) {
//                // ç∂Ç©ÇÁêNì¸ÇµÇΩèÍçá
//                if (blocks_[i]->object_->worldCoordinate_.position_.x - NormalBlock::radius_ < player_->GetObject3d()->worldCoordinate_.position_.x + Player::radius_.x &&
//                    player_->GetObject3d()->worldCoordinate_.position_.x + Player::radius_.x < blocks_[i]->object_->worldCoordinate_.position_.x + NormalBlock::radius_) {
//                    //while ((blocks_[i]->object_->worldCoordinate_.position_.x - Block::radius_) - (player_->GetObject3d()->worldCoordinate_.position_.x + Player::radius_.x) < 0.0f)
//                    //{
//                    //    player_->SetPosX(player_->GetObject3d()->worldCoordinate_.position_.x - 0.01f);
//                    //}
//                }
//                // âEÇ©ÇÁêNì¸ÇµÇΩèÍçá
//                if (blocks_[i]->object_->worldCoordinate_.position_.x - NormalBlock::radius_ < player_->GetObject3d()->worldCoordinate_.position_.x - Player::radius_.x &&
//                    player_->GetObject3d()->worldCoordinate_.position_.x - Player::radius_.x < blocks_[i]->object_->worldCoordinate_.position_.x + NormalBlock::radius_) {
//                    //while ((player_->GetObject3d()->worldCoordinate_.position_.x - Player::radius_.x) + (blocks_[i]->object_->worldCoordinate_.position_.x + Block::radius_) - (player_->GetObject3d()->worldCoordinate_.position_.x + Player::radius_.x) < 0.0f)
//                    //{
//                    //    player_->SetPosX(player_->GetObject3d()->worldCoordinate_.position_.x + 0.01f);
//                    //}
//                }
//            }
//
//            // zï˚å¸
//            if (blocks_[i]->object_->worldCoordinate_.position_.x - (NormalBlock::radius_ + Player::radius_.x) < player_->GetObject3d()->worldCoordinate_.position_.x &&
//                player_->GetObject3d()->worldCoordinate_.position_.x < blocks_[i]->object_->worldCoordinate_.position_.x + (NormalBlock::radius_ + Player::radius_.x) && // <- or?
//                blocks_[i]->object_->worldCoordinate_.position_.y - (NormalBlock::radius_ + Player::radius_.y) < player_->GetObject3d()->worldCoordinate_.position_.y &&
//                player_->GetObject3d()->worldCoordinate_.position_.y < blocks_[i]->object_->worldCoordinate_.position_.y + (NormalBlock::radius_ + Player::radius_.y)
//                ) {
//                // éËëOÇ©ÇÁêNì¸ÇµÇΩèÍçá
//                if (blocks_[i]->object_->worldCoordinate_.position_.z - NormalBlock::radius_ < player_->GetObject3d()->worldCoordinate_.position_.z + (Player::radius_.z + Player::speed_)&&
//                    player_->GetObject3d()->worldCoordinate_.position_.z + (Player::radius_.z + Player::speed_) < blocks_[i]->object_->worldCoordinate_.position_.z + NormalBlock::radius_) {
//                    while ((blocks_[i]->object_->worldCoordinate_.position_.z - NormalBlock::radius_) - (player_->GetObject3d()->worldCoordinate_.position_.z + Player::radius_.z) < 0.0f)
//                    {
//                        player_->SetPosZ(player_->GetObject3d()->worldCoordinate_.position_.z - 0.01f);
//                    }
//                }
//
//                // âúÇ©ÇÁêNì¸ÇµÇΩèÍçá
//                if (blocks_[i]->object_->worldCoordinate_.position_.z - NormalBlock::radius_ < player_->GetObject3d()->worldCoordinate_.position_.z - (Player::radius_.z + Player::speed_) &&
//                    player_->GetObject3d()->worldCoordinate_.position_.z - (Player::radius_.z + Player::speed_) < blocks_[i]->object_->worldCoordinate_.position_.z + NormalBlock::radius_) {
//                    while ((blocks_[i]->object_->worldCoordinate_.position_.z + NormalBlock::radius_) - (player_->GetObject3d()->worldCoordinate_.position_.z - Player::radius_.z) < 0.0f)
//                    {
//                        player_->SetPosZ(player_->GetObject3d()->worldCoordinate_.position_.z + 0.01f);
//                    }
//                }
//            }
//        }
//    }
//}
