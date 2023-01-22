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

    // stateごとの移動処理を管理
    Controll(vel);

    ControllState();

    // 当たり判定
    Collision(vel);

    if (vel.y < 0) {
        isJump_ = true;
    }

    // 補正されたvelosityを加算
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
        // 入力処理
        vel.z += (KEYS::IsDown(DIK_W) - KEYS::IsDown(DIK_S));
        vel.x += (KEYS::IsDown(DIK_D) - KEYS::IsDown(DIK_A));

        // 正規化 
        if (std::sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z) != 0) {
            vel.x = vel.x / std::sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);
            vel.z = vel.z / std::sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);
        }

        // 移動量計算
        vel.x *= speed_;
        vel.z *= speed_;
        Jump();

        // 重力加算
        vel.y -= gravity_;
        vel.y += jumpValue_;

        break;
    case Player::State::ROPE:
        // 入力処理
        vel.y += (KEYS::IsDown(DIK_W) - KEYS::IsDown(DIK_S));

        // 正規化
        if (std::sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z) != 0) {
            vel.y = vel.y / std::sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);
        }

        // 移動量計算
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

        // y軸,z軸においてプレイヤーがブロック内の座標にある時。
        if (std::abs(block->GetPos()->y - object_->worldCoordinate_.position_.y) - (block->GetRadius()->y + Player::radius_.y) < 0.f &&
            std::abs(block->GetPos()->z - object_->worldCoordinate_.position_.z) - (block->GetRadius()->z + Player::radius_.z) < 0.f) {
            // DirectionX
            // 移動先(+vel)がblock内かどうか
            if (std::abs(block->GetPos()->x - (object_->worldCoordinate_.position_.x + vel.x)) - (block->GetRadius()->x + Player::radius_.x) <= 0) {
                if (vel.x > 0) {
                    // block内に入らないようvelの値がblockとピッタリになるように。
                    vel.x = std::abs(block->GetPos()->x - object_->worldCoordinate_.position_.x) - (block->GetRadius()->x + Player::radius_.x);
                }
                else {
                    // block内に入らないようvelの値がblockとピッタリになるように。
                    vel.x = -(std::abs(block->GetPos()->x - object_->worldCoordinate_.position_.x) - (block->GetRadius()->x + Player::radius_.x));
                }
            }
        }

        // x軸,z軸においてプレイヤーがブロック内の座標にある時。
        if (std::abs(block->GetPos()->x - object_->worldCoordinate_.position_.x) - (block->GetRadius()->x + Player::radius_.x) < 0.f &&
            std::abs(block->GetPos()->z - object_->worldCoordinate_.position_.z) - (block->GetRadius()->z + Player::radius_.z) < 0.f) {
            // DirectionY
            // 移動先(+vel)がblock内かどうか
            if (std::abs(block->GetPos()->y - (object_->worldCoordinate_.position_.y + vel.y)) - (block->GetRadius()->y + Player::radius_.y) <= 0) {
                if (vel.y > 0) {
                    // block内に入らないようvelの値がblockとピッタリになるように。
                    vel.y = std::abs(block->GetPos()->y - object_->worldCoordinate_.position_.y) - (block->GetRadius()->y + Player::radius_.y);
                }
                else {
                    // block内に入らないようvelの値がblockとピッタリになるように。
                    vel.y = -(std::abs(block->GetPos()->y - object_->worldCoordinate_.position_.y) - (block->GetRadius()->y + Player::radius_.y));
                }
            }
        }

        // x軸,y軸においてプレイヤーがブロック内の座標にある時。
        if (std::abs(block->GetPos()->x - object_->worldCoordinate_.position_.x) - (block->GetRadius()->x + Player::radius_.x) < 0.f &&
            std::abs(block->GetPos()->y - object_->worldCoordinate_.position_.y) - (block->GetRadius()->y + Player::radius_.y) < 0.f) {
            // DirectionZ
            // 移動先(+vel)がblock内かどうか
            if (std::abs(block->GetPos()->z - (object_->worldCoordinate_.position_.z + vel.z)) - (block->GetRadius()->z + Player::radius_.z) <= 0) {
                if (vel.z > 0) {
                    // block内に入らないようvelの値がblockとピッタリになるように。
                    vel.z = std::abs(block->GetPos()->z - object_->worldCoordinate_.position_.z) - (block->GetRadius()->z + Player::radius_.z);
                }
                else {
                    // block内に入らないようvelの値がblockとピッタリになるように。
                    vel.z = -(std::abs(block->GetPos()->z - object_->worldCoordinate_.position_.z) - (block->GetRadius()->z + Player::radius_.z));
                }
            }
        }


        if (*block->GetType() == IBlock::Type::HOOK) { // block の type が HOOK の場合
            // x軸,z軸においてプレイヤーがブロック内の座標にある時。
            if (std::abs(block->GetPos()->x - object_->worldCoordinate_.position_.x) - (block->GetRadius()->x + Player::radius_.x) < -0.4f && // ※<-ちょっと範囲狭めてる
                std::abs(block->GetPos()->z - object_->worldCoordinate_.position_.z) - (block->GetRadius()->z + Player::radius_.z) < -0.4f) {
                // DirectionY
                // HOOK ブロックから下方向6ブロック以内にいるか
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
