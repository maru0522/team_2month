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
    DirectX::XMFLOAT3 vel{};

    // state���Ƃ̈ړ��������Ǘ�
    Controll(vel);

    ControllState();

    // �����蔻��
    Collision(vel);

    // �␳���ꂽvelosity�����Z
    object_->worldCoordinate_.position_.x += vel.x;
    object_->worldCoordinate_.position_.y += vel.y;
    object_->worldCoordinate_.position_.z += vel.z;
}

void Player::Jump(void)
{
    if (KEYS::IsTrigger(DIK_SPACE) && isJump_ == false && isFloat_ == false) {
        isJump_ = true;
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

void Player::Controll(DirectX::XMFLOAT3& vel)
{
    switch (state_)
    {
    case Player::State::DEFAULT:
        // ���͏���
        vel.z += (KEYS::IsDown(DIK_W) - KEYS::IsDown(DIK_S));
        vel.x += (KEYS::IsDown(DIK_D) - KEYS::IsDown(DIK_A));

        // ���K�� 
        if (std::sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z) != 0) {
            vel.x = vel.x / std::sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);
            vel.z = vel.z / std::sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);
        }

        // �ړ��ʌv�Z
        vel.x *= speed_;
        vel.z *= speed_;
        Jump();

        // �d�͉��Z
        vel.y -= gravity_;
        vel.y += jumpValue_;

        break;
    case Player::State::ROPE:
        // ���͏���
        vel.y += (KEYS::IsDown(DIK_W) - KEYS::IsDown(DIK_S));

        // ���K��
        if (std::sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z) != 0) {
            vel.y = vel.y / std::sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);
        }

        // �ړ��ʌv�Z
        vel.y *= ropeSpeed_;

        break;
    case Player::State::TARZAN:
        break;
    }
}

void Player::ControllState(void)
{
    switch (state_)
    {
    case Player::State::DEFAULT:
        if (isUnderHook_) {
            if (KEYS::IsTrigger(DIK_RETURN)) SetState(State::ROPE);
        }
        break;

    case Player::State::ROPE:
        if (isUnderHook_) {
            if (KEYS::IsTrigger(DIK_RETURN)) SetState(State::DEFAULT);
        }
        break;

    case Player::State::TARZAN:
        break;
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
                    isJump_ = false;
                }
                else {
                    // block���ɓ���Ȃ��悤vel�̒l��block�ƃs�b�^���ɂȂ�悤�ɁB
                    vel.y = -(std::abs(block->GetPos()->y - object_->worldCoordinate_.position_.y) - (block->GetRadius()->y + Player::radius_.y));
                    isJump_ = false;
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


        if (*block->GetType() == IBlock::Type::HOOK) { // block �� type �� HOOK �̏ꍇ
            // x��,z���ɂ����ăv���C���[���u���b�N���̍��W�ɂ��鎞�B
            if (std::abs(block->GetPos()->x - object_->worldCoordinate_.position_.x) - (block->GetRadius()->x + Player::radius_.x) < -0.2f && // ��<-������Ɣ͈͋��߂Ă�
                std::abs(block->GetPos()->z - object_->worldCoordinate_.position_.z) - (block->GetRadius()->z + Player::radius_.z) < -0.2f) {
                // DirectionY
                // HOOK �u���b�N���牺����6�u���b�N�ȓ��ɂ��邩
                if (block->GetPos()->y - (block->GetRadius()->y + Player::radius_.y) >= object_->worldCoordinate_.position_.y &&
                    object_->worldCoordinate_.position_.y > block->GetPos()->y - ((block->GetRadius()->y * 2) * 6 + block->GetRadius()->y)) {
                    isUnderHook_ = true;
                }
                else {
                    isUnderHook_ = false;
                }
            }
        }
    }
}