#include "Player.h"
#include "Input.h"
#include "BlockManager.h"

Player::Player(Camera* pCamera)
{
    object_ = std::make_unique<Obj3d>("Resources/3dModels/cube/cube.obj", pCamera);
}

void Player::Update(void)
{
    Move();

    object_->Update();
}

void Player::Draw(void)
{
    object_->Draw();
}

void Player::Move(void)
{
    Jump();

    DirectX::XMFLOAT3 vel{};

#pragma region ���K���ƈړ��ʓ���
    vel.z += (KEYS::IsDown(DIK_W) - KEYS::IsDown(DIK_S));
    vel.x += (KEYS::IsDown(DIK_D) - KEYS::IsDown(DIK_A));

    if (std::sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z) != 0) {
        vel.x = vel.x / std::sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);
        vel.z = vel.z / std::sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);
    }

    vel.x *= speed_;
    vel.z *= speed_;
#pragma endregion

    // �d�͉��Z
    vel.y -= gravity_;
    vel.y += jumpValue_;

    // �����蔻��
    Collision(vel);

    object_->worldCoordinate_.position_.x += vel.x;
    object_->worldCoordinate_.position_.y += vel.y;
    object_->worldCoordinate_.position_.z += vel.z;
}

void Player::Jump(void)
{
    if (KEYS::IsTrigger(DIK_SPACE) && isJump_ == false && isFloat_ == false) {
        isJump_ = true;
        isFloat_ = true;
        jumpValue_ = jumpPower_;
    }

    if (isJump_) {
        if (jumpValue_ > 0.0f) {
            jumpValue_ -= jumpAttenuation_;
        }
        else {
            isJump_ = false;
            jumpValue_ = 0.0f;
        }
    }
}

void Player::Collision(DirectX::XMFLOAT3& vel)
{
    for (std::unique_ptr<IBlock>& block : *BlockManager::GetBlockList()) {

        // y��,z���ɂ����ăv���C���[���u���b�N���̍��W�ɂ��鎞�B
        if (std::abs(block->GetPos()->y - object_->worldCoordinate_.position_.y) - (block->GetRadius()->y + Player::radius_.y) < 0 &&
            std::abs(block->GetPos()->z - object_->worldCoordinate_.position_.z) - (block->GetRadius()->z + Player::radius_.z) < 0) {
            // DirectionX
            // �ړ���(+vel)��block�����ǂ���
            if (std::abs(block->GetPos()->x - (object_->worldCoordinate_.position_.x + vel.x)) - (block->GetRadius()->x + Player::radius_.x) <= 0) {
                if (vel.x > 0) {
                    // block���ɓ���Ȃ��悤vel�̒l��block�ƃs�b�^���ɂȂ�悤�ɁB
                    vel.x = std::abs(block->GetPos()->x - object_->worldCoordinate_.position_.x) - (block->GetRadius()->x + Player::radius_.x);
                }
                else {
                    // block���ɓ���Ȃ��悤vel�̒l��block�ƃs�b�^���ɂȂ�悤�ɁB
                    vel.x = -(std::abs(block->GetPos()->x - object_->worldCoordinate_.position_.x) - (block->GetRadius()->x + Player::radius_.x));
                }
            }
        }

        // x��,z���ɂ����ăv���C���[���u���b�N���̍��W�ɂ��鎞�B
        if (std::abs(block->GetPos()->x - object_->worldCoordinate_.position_.x) - (block->GetRadius()->x + Player::radius_.x) < 0 &&
            std::abs(block->GetPos()->z - object_->worldCoordinate_.position_.z) - (block->GetRadius()->z + Player::radius_.z) < 0) {
            // DirectionY
            // �ړ���(+vel)��block�����ǂ���
            if (std::abs(block->GetPos()->y - (object_->worldCoordinate_.position_.y + vel.y)) - (block->GetRadius()->y + Player::radius_.y) <= 0) {
                if (vel.y > 0) {
                    // block���ɓ���Ȃ��悤vel�̒l��block�ƃs�b�^���ɂȂ�悤�ɁB
                    vel.y = std::abs(block->GetPos()->y - object_->worldCoordinate_.position_.y) - (block->GetRadius()->y + Player::radius_.y);
                }
                else {
                    // block���ɓ���Ȃ��悤vel�̒l��block�ƃs�b�^���ɂȂ�悤�ɁB
                    vel.y = -(std::abs(block->GetPos()->y - object_->worldCoordinate_.position_.y) - (block->GetRadius()->y + Player::radius_.y));
                }
            }
        }

        // x��,y���ɂ����ăv���C���[���u���b�N���̍��W�ɂ��鎞�B
        if (std::abs(block->GetPos()->x - object_->worldCoordinate_.position_.x) - (block->GetRadius()->x + Player::radius_.x) < 0 &&
            std::abs(block->GetPos()->y - object_->worldCoordinate_.position_.y) - (block->GetRadius()->y + Player::radius_.y) < 0) {
            // DirectionZ
            // �ړ���(+vel)��block�����ǂ���
            if (std::abs(block->GetPos()->z - (object_->worldCoordinate_.position_.z + vel.z)) - (block->GetRadius()->z + Player::radius_.z) <= 0) {
                if (vel.z > 0) {
                    // block���ɓ���Ȃ��悤vel�̒l��block�ƃs�b�^���ɂȂ�悤�ɁB
                    vel.z = std::abs(block->GetPos()->z - object_->worldCoordinate_.position_.z) - (block->GetRadius()->z + Player::radius_.z);
                }
                else {
                    // block���ɓ���Ȃ��悤vel�̒l��block�ƃs�b�^���ɂȂ�悤�ɁB
                    vel.z = -(std::abs(block->GetPos()->z - object_->worldCoordinate_.position_.z) - (block->GetRadius()->z + Player::radius_.z));
                }
            }
        }
    }
}