#include "Player.h"
#include "Input.h"
#include "BlockManager.h"
#include "Util.h"
#include <algorithm>
#include "Window.h"

Player::Player(Camera* pCamera)
{
    object_ = std::make_unique<Obj3d>("Resources/3dModels/cube/cube.obj", pCamera);
    cameraPtr_ = pCamera;
    ropeUsePad_sprite_->SetPosition({ Window::width_ - 103 - 10,51 });
    ropeUsePadPress_sprite_->SetPosition({ Window::width_ - 103 - 10,51 });
    connectUsePad_sprite_->SetPosition({ Window::width_ - 103 - 10,124 });
    connectUsePadPress_sprite_->SetPosition({ Window::width_ - 103 - 10,124 });
    ropeKeyTimer_->Start(1.5f);

    default_sprite_->SetSize({ 53,101 });
    default_sprite_->SetPosition({ Window::width_ - 53 - 10, 10 });
    rope_sprite_->SetSize({ 53,101 });
    rope_sprite_->SetPosition({ Window::width_ - 53 - 10, 10 });
    connect_sprite_->SetSize({ 53,53 });
    connect_sprite_->SetPosition({ Window::width_ - 53 - 10, 121 });
}

void Player::Update(void)
{
    Move();

    object_->Update();

    ControllKeyTimer();

    ropeUsePad_sprite_->Update();
    ropeUsePadPress_sprite_->Update();
    connectUsePad_sprite_->Update();
    connectUsePadPress_sprite_->Update();

    default_sprite_->Update();
    rope_sprite_->Update();
    connect_sprite_->Update();
}

void Player::Draw3d(void)
{
    object_->Draw();
}

void Player::Draw2d(void)
{
    if (isUnderHook_) {
        if (ropeKeyTimer_->GetElapsedTime() <= ropeKeyTimer_->GetEndTime() / 2.f) {
            ropeUsePad_sprite_->Draw();
        }
        else {
            ropeUsePadPress_sprite_->Draw();
        }
    }

    if (isNearSupply_ || (isConnecting_ && isNearReceive_)) {
        if (ropeKeyTimer_->GetElapsedTime() <= ropeKeyTimer_->GetEndTime() / 2.f) {
            connectUsePad_sprite_->Draw();
        }
        else {
            connectUsePadPress_sprite_->Draw();
        }
    }

    if (isConnecting_) {
        connect_sprite_->Draw();
    }

    switch (state_)
    {
    case Player::MoveState::DEFAULT:
        default_sprite_->Draw();
        break;
    case Player::MoveState::ROPE:
        rope_sprite_->Draw();
        break;
    case Player::MoveState::TARZAN:
        break;
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

    if (XPAD::IsTrigger(XPAD_A) && isJump_ == false) {
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
#pragma region カメラの正面ベクトル取得
    // 正面ベクトル取得
    DirectX::XMFLOAT3 frontVec{ cameraPtr_->target_.x - cameraPtr_->eye_.x, cameraPtr_->target_.y - cameraPtr_->eye_.y, cameraPtr_->target_.z - cameraPtr_->eye_.z };
    // ノルム取得
    float length{ std::sqrtf(frontVec.x * frontVec.x + frontVec.y * frontVec.y + frontVec.z * frontVec.z) };
    // 正規化
    if (length != 0) {
        frontVec.x /= length;
        frontVec.y /= length;
        frontVec.z /= length;
    }
    // 右ベクトル取得
    DirectX::XMFLOAT3 rightVec{ frontVec.y * cameraPtr_->up_.z - frontVec.z - cameraPtr_->up_.y,
                                frontVec.z * cameraPtr_->up_.x - frontVec.x - cameraPtr_->up_.z,
                                frontVec.x * cameraPtr_->up_.y - frontVec.y - cameraPtr_->up_.x };

    // 右ベクトルのノルム
    float crossLength{ std::sqrtf(rightVec.x * rightVec.x + rightVec.y * rightVec.y + rightVec.z * rightVec.z) };

    // 右ベクトルの正規化
    if (crossLength != 0) {
        rightVec.x /= crossLength;
        rightVec.y /= crossLength;
        rightVec.z /= crossLength;
    }
#pragma endregion

    // padのスティック入力情報処理
    DirectX::XMFLOAT2 inputLStick{ XPAD::GetLStick().x ,XPAD::GetLStick().y };

    if (inputLStick.x > 0) {
        inputLStick.x /= INT16_MAX;
    }
    else if (inputLStick.x < 0) {
        inputLStick.x /= INT16_MIN;
        inputLStick.x = -inputLStick.x;
    }
    else {
        inputLStick.x = 0.f;
    }

    if (inputLStick.y > 0) {
        inputLStick.y /= INT16_MAX;
    }
    else if (inputLStick.y < 0) {
        inputLStick.y /= INT16_MIN;
        inputLStick.y = -inputLStick.y;
    }
    else {
        inputLStick.y = 0.f;
    }

    switch (state_)
    {
    case Player::MoveState::DEFAULT:
        // 入力処理
        // y以外
        frontVec.x *= inputLStick.y/* * normalizeLStick.y*/;
        frontVec.z *= inputLStick.y/* * normalizeLStick.y*/;

        rightVec.x *= -inputLStick.x/* * normalizeLStick.x*/;
        rightVec.z *= -inputLStick.x/* * normalizeLStick.x*/;

        vel.x += frontVec.x + rightVec.x;
        vel.z += frontVec.z + rightVec.z;

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
    case Player::MoveState::ROPE:
        // 入力値（正規化済）を移動量の礎として代入
        vel.y += inputLStick.y;

        // 移動量計算
        vel.y *= ropeSpeed_;

        break;
    case Player::MoveState::TARZAN:
        break;

#ifdef _DEBUG
    case Player::MoveState::S_DEBUG:
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

            if (XPAD::IsTrigger(XPAD_RB)) {
                SetState(MoveState::ROPE);
            }
        }

        if (isNearSupply_) {
            if (XPAD::IsTrigger(XPAD_X)) {
                if (isConnecting_) {
                    isConnecting_ = false;
                }
                else {
                    isConnecting_ = true;
                }
            }
        }

        if (isNearReceive_) { // 受電ブロックが近くにあるとき
            if (XPAD::IsTrigger(XPAD_X)) { //特定のキーを押したとき
                if (isConnecting_) { // ワイヤーをつなげようとしてる最中なら
                    isConnecting_ = false; // つなげて、最中ではなくなる
                }
                else { // ワイヤーをつなげようとしてる最中でないなら
                    for (std::unique_ptr<IBlock>& block : *BlockManager::GetBlockList()) { // ブロックを全検索 ※まじで苦肉の策
                        if (*block->GetType() == IBlock::Type::POWERRECEIVE) { // 対象のブロックタイプが受電ブロックのとき
                            if (BlockManager::GetConnectMap()->at(block->GetIdxConnect())) { // 対応する[接続番号]のbool値がtrueなら
                                isConnecting_ = true; // ワイヤーをつなげようとしてる最中になる。 -> 対応するブロックのbool値は自動的にfalseになる
                            }
                        } // 要約すると、ワイヤーをつなげようとしていない時は、受電ブロックの近くでキーを押しても、[つなげようとしてる最中]の状態にならない
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
            if (XPAD::IsTrigger(XPAD_RB)) SetState(MoveState::DEFAULT);
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
    bool resultAllReceives{ false };

    for (std::unique_ptr<IBlock>& block : *BlockManager::GetBlockList()) {

        if (*block->GetType() == IBlock::Type::SWITCH) {
            if (BlockManager::GetConnectMap()->at(block->GetIdxConnect()) == false) {
                continue;
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

        if (*block->GetType() == IBlock::Type::POWERSUPPLY) { // block の type が POWERSUPPLY の場合
            // y軸おいてプレイヤーがブロック内の座標にある時。
            if (std::abs(block->GetPos()->y - object_->worldCoordinate_.position_.y) - (block->GetRadius()->y + Player::radius_.y) < -1.f) { // ほぼ同一のy座標値において
                if (std::abs(block->GetPos()->x - object_->worldCoordinate_.position_.x) - (block->GetRadius()->x + Player::radius_.x) < 1.f && // 
                    std::abs(block->GetPos()->z - object_->worldCoordinate_.position_.z) - (block->GetRadius()->z + Player::radius_.z) < 1.f) {
                    isNearSupply_ = true;
                    BlockManager::GetSupplyMap()->at(block->GetIdxSupply()) = true;
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

        if (*block->GetType() == IBlock::Type::POWERRECEIVE) { // block の type が POWERRECIEVE の場合
            // y軸おいてプレイヤーがブロック内の座標にある時。
            if (std::abs(block->GetPos()->y - object_->worldCoordinate_.position_.y) - (block->GetRadius()->y + Player::radius_.y) < -1.f) { // ほぼ同一のy座標値において
                if (std::abs(block->GetPos()->x - object_->worldCoordinate_.position_.x) - (block->GetRadius()->x + Player::radius_.x) < 1.f && // 
                    std::abs(block->GetPos()->z - object_->worldCoordinate_.position_.z) - (block->GetRadius()->z + Player::radius_.z) < 1.f) {
                    isNearReceive_ = true;
                    BlockManager::GetReceiveMap()->at(block->GetIdxReceive()) = true;
                }
                else {
                    BlockManager::GetReceiveMap()->at(block->GetIdxReceive()) = false;

                    for (std::pair<const BlockManager::IdxReceive, bool>& receive : *BlockManager::GetReceiveMap()) {
                        if (receive.second) {
                            resultAllReceives = true;
                            break;
                        }
                    }
                    if (resultAllReceives == false) {
                        isNearReceive_ = false;
                    }
                }
            }
            else {
                BlockManager::GetReceiveMap()->at(block->GetIdxReceive()) = false;

                for (std::pair<const BlockManager::IdxReceive, bool>& receive : *BlockManager::GetReceiveMap()) {
                    if (receive.second) {
                        resultAllReceives = true;
                        break;
                    }
                }
                if (resultAllReceives == false) {
                    isNearReceive_ = false;
                }
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

        if (*block->GetType() == IBlock::Type::HOOK) { // block の type が HOOK の場合
            // x軸,z軸においてプレイヤーがブロック内の座標にある時。
            if (std::abs(block->GetPos()->x - object_->worldCoordinate_.position_.x) - (block->GetRadius()->x + Player::radius_.x) < -0.4f && // ※<-ちょっと範囲狭めてる
                std::abs(block->GetPos()->z - object_->worldCoordinate_.position_.z) - (block->GetRadius()->z + Player::radius_.z) < -0.4f) {
                // DirectionY
                // HOOK ブロックから下方向10ブロック以内にいるか
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

        if (*block->GetType() == IBlock::Type::FAN) {
            if (BlockManager::GetConnectMap()->at(block->GetIdxConnect()) == true) {
                // x軸,z軸においてプレイヤーがブロック内の座標にある時。
                if (std::abs(block->GetPos()->x - object_->worldCoordinate_.position_.x) - (block->GetRadius()->x + Player::radius_.x) < 0.f && // ※<-ちょっと範囲狭めてる
                    std::abs(block->GetPos()->z - object_->worldCoordinate_.position_.z) - (block->GetRadius()->z + Player::radius_.z) < 0.f) {
                    // DirectionY
                    // FAN ブロックから上方向10ブロック以内にいるか
                    if (block->GetPos()->y + (block->GetRadius()->y + Player::radius_.y) <= object_->worldCoordinate_.position_.y &&
                        object_->worldCoordinate_.position_.y < block->GetPos()->y - ((block->GetRadius()->y * 2) * 10 + block->GetRadius()->y)) {
                        vel.y = IBlock::fanUpVecSpeed_;
                    }
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

    ImGui::Spacing();

    ImGui::Text("XPAD_LStick");
    ImGui::Text("LS_X : %f, LS_Y : %f", XPAD::GetLStick().x, XPAD::GetLStick().y);

    ImGui::End();
#endif // _DEBUG
}
