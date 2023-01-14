#include "Player.h"
#include "Input.h"

Player::Player(Camera* pCamera)
{
    object_ = std::make_unique<Obj3d>("Resources/3dModels/cube/cube.obj", pCamera);
}

void Player::Update(void)
{
    Move();
    Jump();

    object_->Update();
}

void Player::Draw(void)
{
    object_->Draw();
}

void Player::Move(void)
{
    DirectX::XMFLOAT3 move{};

    if (KEYS::IsDown(DIK_W)) {
        move.z += speed_;
    }
    if (KEYS::IsDown(DIK_S)) {
        move.z -= speed_;
    }
    if (KEYS::IsDown(DIK_A)) {
        move.x -= speed_;
    }
    if (KEYS::IsDown(DIK_D)) {
        move.x += speed_;
    }

    object_->worldCoordinate_.position_.x += move.x;
    object_->worldCoordinate_.position_.z += move.z;
}

void Player::Jump(void)
{
    if (KEYS::IsTrigger(DIK_SPACE)) {
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

    object_->worldCoordinate_.position_.y += jumpValue_;
}
