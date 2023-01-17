#include "GameScene.h"
#include "Texture.h"
#include "Model.h"
#include "Input.h"

void GameScene::Initialize(SceneManager* pSceneManager)
{
    BaseScene::Initialize(pSceneManager);

    // ïœêîèâä˙âª
    cameraT_ = std::make_unique<Camera>();
    cameraT_->eye_ = { 0,0, -100 };
    cameraT_->target_ = { 0 ,0, 0 };

    player_ = std::make_unique<Player>(cameraT_.get());
    player_->SetPos({ 0.0f,4.0f,0.0f });

    for (size_t i = 0; i < blocks_.size(); i++) {
        blocks_[i] = std::make_unique<Block>(cameraT_.get());
        blocks_[i]->object_->worldCoordinate_.position_ = { 2.0f * i,0,0 };
    }
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

    Col();
}

void GameScene::Draw3d(void)
{
    player_->Draw();

    for (size_t i = 0; i < blocks_.size(); i++) {
        blocks_[i]->Draw();
    }
}

void GameScene::Draw2d(void)
{
}

void GameScene::Finalize(void)
{
}

void GameScene::Col(void)
{
    for (size_t i = 0; i < blocks_.size(); i++) {
        if (std::abs(blocks_[i]->object_->worldCoordinate_.position_.x - player_->GetObject3d()->worldCoordinate_.position_.x) <= rangeIntoPlayer_ ||
            std::abs(blocks_[i]->object_->worldCoordinate_.position_.y - player_->GetObject3d()->worldCoordinate_.position_.y) <= rangeIntoPlayer_ ||
            std::abs(blocks_[i]->object_->worldCoordinate_.position_.z - player_->GetObject3d()->worldCoordinate_.position_.z) <= rangeIntoPlayer_) {
            blocks_[i]->Update();

            // yï˚å¸
            if (blocks_[i]->object_->worldCoordinate_.position_.x - (Block::radius_ + Player::radius_.x) < player_->GetObject3d()->worldCoordinate_.position_.x &&
                player_->GetObject3d()->worldCoordinate_.position_.x < blocks_[i]->object_->worldCoordinate_.position_.x + (Block::radius_ + Player::radius_.x) && // <- or?
                blocks_[i]->object_->worldCoordinate_.position_.z - (Block::radius_ + Player::radius_.z) < player_->GetObject3d()->worldCoordinate_.position_.z &&
                player_->GetObject3d()->worldCoordinate_.position_.z < blocks_[i]->object_->worldCoordinate_.position_.z + (Block::radius_ + Player::radius_.z)
                ) {
                // è„Ç©ÇÁêNì¸ÇµÇΩèÍçá
                if (blocks_[i]->object_->worldCoordinate_.position_.y - Block::radius_ <= player_->GetObject3d()->worldCoordinate_.position_.y - Player::radius_.y &&
                    player_->GetObject3d()->worldCoordinate_.position_.y - Player::radius_.y <= blocks_[i]->object_->worldCoordinate_.position_.y + Block::radius_) {
                    player_->SetPosY(player_->GetOldPos().y);
                    player_->SetFloatFlag(false);
                }

                // â∫Ç©ÇÁêNì¸ÇµÇΩèÍçá
                if (blocks_[i]->object_->worldCoordinate_.position_.y - Block::radius_ <= player_->GetObject3d()->worldCoordinate_.position_.y + Player::radius_.y &&
                    player_->GetObject3d()->worldCoordinate_.position_.y + Player::radius_.y <= blocks_[i]->object_->worldCoordinate_.position_.y + Block::radius_) {
                    player_->SetPosY(player_->GetOldPos().y);
                }
            }

            // xï˚å¸
            if (blocks_[i]->object_->worldCoordinate_.position_.y - (Block::radius_ + Player::radius_.y) < player_->GetObject3d()->worldCoordinate_.position_.y &&
                player_->GetObject3d()->worldCoordinate_.position_.y < blocks_[i]->object_->worldCoordinate_.position_.y + (Block::radius_ + Player::radius_.y) && // <- or?
                blocks_[i]->object_->worldCoordinate_.position_.z - (Block::radius_ + Player::radius_.z) < player_->GetObject3d()->worldCoordinate_.position_.z &&
                player_->GetObject3d()->worldCoordinate_.position_.z < blocks_[i]->object_->worldCoordinate_.position_.z + (Block::radius_ + Player::radius_.z)
                ) {
                // ç∂Ç©ÇÁêNì¸ÇµÇΩèÍçá
                if (blocks_[i]->object_->worldCoordinate_.position_.x - Block::radius_ < player_->GetObject3d()->worldCoordinate_.position_.x + Player::radius_.x &&
                    player_->GetObject3d()->worldCoordinate_.position_.x + Player::radius_.x < blocks_[i]->object_->worldCoordinate_.position_.x + Block::radius_) {
                    player_->SetPosX(player_->GetOldPos().x);
                }
                // âEÇ©ÇÁêNì¸ÇµÇΩèÍçá
                if (blocks_[i]->object_->worldCoordinate_.position_.x - Block::radius_ < player_->GetObject3d()->worldCoordinate_.position_.x - Player::radius_.x &&
                    player_->GetObject3d()->worldCoordinate_.position_.x - Player::radius_.x < blocks_[i]->object_->worldCoordinate_.position_.x + Block::radius_) {
                    player_->SetPosX(player_->GetOldPos().x);
                }
            }

            // zï˚å¸
            if (blocks_[i]->object_->worldCoordinate_.position_.x - (Block::radius_ + Player::radius_.x) < player_->GetObject3d()->worldCoordinate_.position_.x &&
                player_->GetObject3d()->worldCoordinate_.position_.x < blocks_[i]->object_->worldCoordinate_.position_.x + (Block::radius_ + Player::radius_.x) && // <- or?
                blocks_[i]->object_->worldCoordinate_.position_.y - (Block::radius_ + Player::radius_.y) < player_->GetObject3d()->worldCoordinate_.position_.y &&
                player_->GetObject3d()->worldCoordinate_.position_.y < blocks_[i]->object_->worldCoordinate_.position_.y + (Block::radius_ + Player::radius_.y)
                ) {
                // éËëOÇ©ÇÁêNì¸ÇµÇΩèÍçá
                if (blocks_[i]->object_->worldCoordinate_.position_.z - Block::radius_ < player_->GetObject3d()->worldCoordinate_.position_.z + Player::radius_.z &&
                    player_->GetObject3d()->worldCoordinate_.position_.z + Player::radius_.z < blocks_[i]->object_->worldCoordinate_.position_.z + Block::radius_) {
                    player_->SetPosZ(player_->GetOldPos().z);
                }

                // âúÇ©ÇÁêNì¸ÇµÇΩèÍçá
                if (blocks_[i]->object_->worldCoordinate_.position_.z - Block::radius_ < player_->GetObject3d()->worldCoordinate_.position_.z - Player::radius_.z &&
                    player_->GetObject3d()->worldCoordinate_.position_.z - Player::radius_.z < blocks_[i]->object_->worldCoordinate_.position_.z + Block::radius_) {
                    player_->SetPosZ(player_->GetOldPos().z);
                }
            }
        }
    }
}
