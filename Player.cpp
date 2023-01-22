#include "Player.h"
#include "Input.h"
#include "BlockManager.h"
#include "Util.h"

Player::Player(Camera* pCamera)
{
    object_ = std::make_unique<Obj3d>("Resources/3dModels/cube/cube.obj", pCamera);
    ropeUseKey_sprite_->SetPosition({ 1118,654 });
    ropeUseKeyPress_sprite_->SetPosition({ 1118,660 });
    ropeKeyTimer_->Start(1.5f);
}

void Player::Update(void)
{
    Move();

    object_->Update();

    ControllKeyTimer();

    ropeUseKey_sprite_->Update();
    ropeUseKeyPress_sprite_->Update();
}

void Player::Draw3d(void)
{
    object_->Draw();
}

void Player::Draw2d(void)
{
    if (isUnderHook_) {
        if (ropeKeyTimer_->GetElapsedTime() <= ropeKeyTimer_->GetEndTime() / 2.f ) {
            ropeUseKey_sprite_->Draw();
        }
        else {
            ropeUseKeyPress_sprite_->Draw();
        }
    }
}

void Player::Move(void)
{
    DirectX::XMFLOAT3 vel{};

    // state���Ƃ̈ړ��������Ǘ�
    Controll(vel);

    ControllState();

    // �����蔻��
    Collision(vel);

    if (vel.y < 0) {
        isJump_ = true;
    }

    // �␳���ꂽvelosity�����Z
    object_->worldCoordinate_.position_.x += vel.x;
    object_->worldCoordinate_.position_.y += vel.y;
    object_->worldCoordinate_.position_.z += vel.z;

#ifdef _DEBUG
    DrawImgui(vel);
#endif // _DEBUG
}

void Player::Jump(void)
{
    if (KEYS::IsTrigger(DIK_SPACE) && isJump_ == false) {
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
            if (KEYS::IsTrigger(DIK_RETURN)) {
                SetState(State::ROPE);
            }
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
        if (std::abs(block->GetPos()->y - object_->worldCoordinate_.position_.y) - (block->GetRadius()->y + Player::radius_.y) < 0.f &&
            std::abs(block->GetPos()->z - object_->worldCoordinate_.position_.z) - (block->GetRadius()->z + Player::radius_.z) < 0.f) {
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
        if (std::abs(block->GetPos()->x - object_->worldCoordinate_.position_.x) - (block->GetRadius()->x + Player::radius_.x) < 0.f &&
            std::abs(block->GetPos()->z - object_->worldCoordinate_.position_.z) - (block->GetRadius()->z + Player::radius_.z) < 0.f) {
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
        if (std::abs(block->GetPos()->x - object_->worldCoordinate_.position_.x) - (block->GetRadius()->x + Player::radius_.x) < 0.f &&
            std::abs(block->GetPos()->y - object_->worldCoordinate_.position_.y) - (block->GetRadius()->y + Player::radius_.y) < 0.f) {
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
            if (std::abs(block->GetPos()->x - object_->worldCoordinate_.position_.x) - (block->GetRadius()->x + Player::radius_.x) < -0.4f && // ��<-������Ɣ͈͋��߂Ă�
                std::abs(block->GetPos()->z - object_->worldCoordinate_.position_.z) - (block->GetRadius()->z + Player::radius_.z) < -0.4f) {
                // DirectionY
                // HOOK �u���b�N���牺����6�u���b�N�ȓ��ɂ��邩
                if (block->GetPos()->y - (block->GetRadius()->y + Player::radius_.y) >= object_->worldCoordinate_.position_.y &&
                    object_->worldCoordinate_.position_.y > block->GetPos()->y - ((block->GetRadius()->y * 2) * 6 + block->GetRadius()->y)) {
                    isUnderHook_ = true;
                }
            }
            else {
                isUnderHook_ = false;
            }
        }
    }
}

void Player::ControllKeyTimer(void)
{
    if (ropeKeyTimer_->GetIsEnd()) ropeKeyTimer_->Start(1.5f);
}

void Player::DrawImgui(const DirectX::XMFLOAT3& vel)
{
    ImGui::Begin("Player info");;
    ImGui::Text(isUnderHook_ ? "isUnderHook_ : true" : "isUnderHook_ : false");
    ImGui::RadioButton("true", (int*)&isUnderHook_, true);
    ImGui::SameLine();
    ImGui::RadioButton("false", (int*)&isUnderHook_, false);

    ImGui::Spacing();

    ImGui::Text("PlayerCoordinate : %.3f,%.3f,%.3f", object_->worldCoordinate_.position_.x, object_->worldCoordinate_.position_.y, object_->worldCoordinate_.position_.z);
    ImGui::Text("PlayerVec : %.3f,%.3f,%.3f", vel.x, vel.y, vel.z);

    ImGui::Spacing();

    ImGui::Text("ropeKeyTimer_elapsedTime : ropeKeyTimer_endTime");
    ImGui::Text("%.6f : %.6f", ropeKeyTimer_->GetElapsedTime(), ropeKeyTimer_->GetEndTime());
    ImGui::Text(ropeKeyTimer_->GetIsEnd() ? "isEnd_ : true" : "isEnd_ : false");

    ImGui::Spacing();

    ImGui::Text(isJump_ ? "isJump_ : true" : "isJump_ : false");

    ImGui::End();
}
