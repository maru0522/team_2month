#include "Player.h"
#include "Input.h"
#include "BlockManager.h"
#include "Util.h"

Player::Player(Camera* pCamera)
{
    object_ = std::make_unique<Obj3d>("Resources/3dModels/cube/cube.obj", pCamera);
    ropeUseKey_sprite_->SetPosition({ 1098,634 });
    ropeUseKeyPress_sprite_->SetPosition({ 1098,640 });
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
        if (ropeKeyTimer_->GetElapsedTime() <= ropeKeyTimer_->GetEndTime() / 2.f) {
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
    case Player::MoveState::DEFAULT:
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
    case Player::MoveState::ROPE:
        // ���͏���
        vel.y += (KEYS::IsDown(DIK_W) - KEYS::IsDown(DIK_S));

        // ���K��
        if (std::sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z) != 0) {
            vel.y = vel.y / std::sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);
        }

        // �ړ��ʌv�Z
        vel.y *= ropeSpeed_;

        break;
    case Player::MoveState::TARZAN:
        break;

#ifdef _DEBUG
    case Player::MoveState::S_DEBUG:
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


        if (KEYS::IsDown(DIK_UPARROW)) {
            vel.y += speed_;
        }
        if (KEYS::IsDown(DIK_DOWNARROW)) {
            vel.y -= speed_;
        }

        break;
#endif // _DEBUG

    }
}

void Player::ControllState(void)
{
    oldConnecting_ = isConnecting_;

    switch (state_)
    {
    case Player::MoveState::DEFAULT:
        if (isUnderHook_) {
            if (KEYS::IsTrigger(DIK_RETURN)) {
                SetState(MoveState::ROPE);
            }
        }

        if (isNearSupply_) {
            if (KEYS::IsTrigger(DIK_NUMPAD7)) {
                if (isConnecting_) {
                    isConnecting_ = false;
                }
                else {
                    isConnecting_ = true;
                }
            }
        }

        if (isNearReceive_) { // ��d�u���b�N���߂��ɂ���Ƃ�
            if (KEYS::IsTrigger(DIK_NUMPAD7)) { //����̃L�[���������Ƃ�
                if (isConnecting_) { // ���C���[���Ȃ��悤�Ƃ��Ă�Œ��Ȃ�
                    isConnecting_ = false; // �Ȃ��āA�Œ��ł͂Ȃ��Ȃ�
                }
                else { // ���C���[���Ȃ��悤�Ƃ��Ă�Œ��łȂ��Ȃ�
                    for (std::unique_ptr<IBlock>& block : *BlockManager::GetBlockList()) { // �u���b�N��S���� ���܂��ŋ���̍�
                        if (*block->GetType() == IBlock::Type::POWERRECEIVE) { // �Ώۂ̃u���b�N�^�C�v����d�u���b�N�̂Ƃ�
                            if (BlockManager::GetConnectMap()->at(block->GetIdxConnect())) { // �Ή�����[�ڑ��ԍ�]��bool�l��true�Ȃ�
                                isConnecting_ = true; // ���C���[���Ȃ��悤�Ƃ��Ă�Œ��ɂȂ�B -> �Ή�����u���b�N��bool�l�͎����I��false�ɂȂ�
                            }
                        } // �v�񂷂�ƁA���C���[���Ȃ��悤�Ƃ��Ă��Ȃ����́A��d�u���b�N�̋߂��ŃL�[�������Ă��A[�Ȃ��悤�Ƃ��Ă�Œ�]�̏�ԂɂȂ�Ȃ�
                    }
                }
            }
        }

#ifdef _DEBUG
        if (KEYS::IsDown(DIK_LSHIFT)) {
            if (KEYS::IsTrigger(DIK_NUMPAD9)) {
                SetState(MoveState::S_DEBUG);
            }
        }
#endif // _DEBUG

        break;

    case Player::MoveState::ROPE:
        if (isUnderHook_) {
            if (KEYS::IsTrigger(DIK_RETURN)) SetState(MoveState::DEFAULT);
        }
        break;

    case Player::MoveState::TARZAN:
        break;

#ifdef _DEBUG
    case Player::MoveState::S_DEBUG:
        if (KEYS::IsDown(DIK_LSHIFT)) {
            if (KEYS::IsTrigger(DIK_NUMPAD9)) {
                SetState(MoveState::DEFAULT);
            }
        }
#endif // _DEBUG
    }
}

void Player::Collision(DirectX::XMFLOAT3& vel)
{
    bool resultAllHooks{ false };
    bool resultAllSupplies{ false };

    for (std::unique_ptr<IBlock>& block : *BlockManager::GetBlockList()) {

        if (*block->GetType() == IBlock::Type::SWITCH) {
            if (BlockManager::GetConnectMap()->at(block->GetIdxConnect()) == false) {
                continue;
            }
        }

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

        if (*block->GetType() == IBlock::Type::POWERSUPPLY) { // block �� type �� POWERSUPPLY �̏ꍇ
            // y�������ăv���C���[���u���b�N���̍��W�ɂ��鎞�B
            if (std::abs(block->GetPos()->y - object_->worldCoordinate_.position_.y) - (block->GetRadius()->y + Player::radius_.y) < -1.f) { // �قړ����y���W�l�ɂ�����
                if (std::abs(block->GetPos()->x - object_->worldCoordinate_.position_.x) - (block->GetRadius()->x + Player::radius_.x) < 1.f && // 
                    std::abs(block->GetPos()->z - object_->worldCoordinate_.position_.z) - (block->GetRadius()->z + Player::radius_.z) < 1.f) {
                    isNearSupply_ = true;
                    BlockManager::GetSupplyMap()->at(block->GetIdxSupply()) = true;
                }
                else {
                    BlockManager::GetSupplyMap()->at(block->GetIdxSupply()) = false;
                }
            }
            else {
                BlockManager::GetSupplyMap()->at(block->GetIdxSupply()) = false;

                for (std::pair<const BlockManager::IdxSupply, bool>& supply : *BlockManager::GetSupplyMap()) {
                    if (supply.second) {
                        resultAllSupplies = true;
                        break;
                    }
                }
                if (resultAllSupplies == false) {
                    isNearSupply_ = false;
                }
            }
        }

        if (*block->GetType() == IBlock::Type::POWERRECEIVE) { // block �� type �� POWERRECIEVE �̏ꍇ
            // y�������ăv���C���[���u���b�N���̍��W�ɂ��鎞�B
            if (std::abs(block->GetPos()->y - object_->worldCoordinate_.position_.y) - (block->GetRadius()->y + Player::radius_.y) < -1.f) { // �قړ����y���W�l�ɂ�����
                if (std::abs(block->GetPos()->x - object_->worldCoordinate_.position_.x) - (block->GetRadius()->x + Player::radius_.x) < 1.f && // 
                    std::abs(block->GetPos()->z - object_->worldCoordinate_.position_.z) - (block->GetRadius()->z + Player::radius_.z) < 1.f) {
                    isNearReceive_ = true;
                }
                else {
                    isNearReceive_ = false;
                }
            }
            else {
                isNearReceive_ = false;
            }

            if (isNearReceive_) {
                if (!oldConnecting_ && isConnecting_) {
                    BlockManager::GetConnectMap()->at(block->GetIdxConnect()) = false;
                }
                else if (oldConnecting_ && !isConnecting_) {
                    BlockManager::GetConnectMap()->at(block->GetIdxConnect()) = true;
                }
            }
        }

        if (*block->GetType() == IBlock::Type::HOOK) { // block �� type �� HOOK �̏ꍇ
            // x��,z���ɂ����ăv���C���[���u���b�N���̍��W�ɂ��鎞�B
            if (std::abs(block->GetPos()->x - object_->worldCoordinate_.position_.x) - (block->GetRadius()->x + Player::radius_.x) < -0.4f && // ��<-������Ɣ͈͋��߂Ă�
                std::abs(block->GetPos()->z - object_->worldCoordinate_.position_.z) - (block->GetRadius()->z + Player::radius_.z) < -0.4f) {
                // DirectionY
                // HOOK �u���b�N���牺����10�u���b�N�ȓ��ɂ��邩
                if (block->GetPos()->y - (block->GetRadius()->y + Player::radius_.y) >= object_->worldCoordinate_.position_.y &&
                    object_->worldCoordinate_.position_.y > block->GetPos()->y - ((block->GetRadius()->y * 2) * 10 + block->GetRadius()->y)) {
                    isUnderHook_ = true;
                    BlockManager::GetUnderHooksMap()->at(block->GetIdxHook()) = true;
                }
            }
            else {
                BlockManager::GetUnderHooksMap()->at(block->GetIdxHook()) = false;

                for (std::pair<const BlockManager::IdxHook, bool>& hook : *BlockManager::GetUnderHooksMap()) {
                    if (hook.second) {
                        resultAllHooks = true;
                        break;
                    }
                }
                if (resultAllHooks == false) {
                    isUnderHook_ = false;
                }
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
#ifdef _DEBUG
    ImGui::Begin("Player info");

    switch (state_)
    {
    case Player::MoveState::DEFAULT:
        ImGui::Text("PlayerState : DEFAULT");
        break;
    case Player::MoveState::ROPE:
        ImGui::Text("PlayerState : ROPE");
        break;
    case Player::MoveState::TARZAN:
        ImGui::Text("PlayerState : TARZAN");
        break;
    case Player::MoveState::S_DEBUG:
        ImGui::Text("PlayerState : S_DEBUG");
        break;
    }
    ImGui::Spacing();

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

    ImGui::Spacing();

    ImGui::Text(isNearSupply_ ? "isNearSupply_ : true" : "isNearSupply_ : false");
    ImGui::Text(isNearReceive_ ? "isNearReceive_ : true" : "isNearReceive_ : false");
    ImGui::Text(isConnecting_ ? "isConnecting_ : true" : "isConnecting_ : false");

    ImGui::End();
#endif // _DEBUG
}
