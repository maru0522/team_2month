#include "Player.h"
#include "Input.h"

Player::Player(Camera* pCamera)
{
    object_ = std::make_unique<Obj3d>("Resources/3dModels/cube/cube.obj", pCamera);
    ropeSp_ = std::make_unique<Sprite>("Resources/rope.png", CMode::PATH);

}

void Player::Update(void)
{
    if (KEYS::IsTrigger(DIK_RETURN))
    {
        if (isThrow_ == false)
        {
            isThrow_ = true;
        }
        else
        {
            isThrow_ = false;
        }
    }

    if (isThrow_ == false)
    {
        Move();

    }
    else
    {
        Throw();
    }

    Jump();
    ropeSp_->SetPosition({ object_->worldCoordinate_.position_.x,object_->worldCoordinate_.position_.y });
    ropeSp_->Update();
    object_->Update();
}

void Player::Draw(void)
{
    object_->Draw();

   
}

void Player::Draw2D(void)
{
    if (isThrow_ == true)
    {
        ropeSp_->Draw();
    }
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

void Player::Throw(void)
{

    DirectX::XMFLOAT3 move{};

    if (isThrow_ == true)
    {
        if (KEYS::IsDown(DIK_W)) {
            move.y += speed_ / 2.5f;
        }
        if (KEYS::IsDown(DIK_S)) {
            move.y -= speed_ / 2.5f;
        }
        object_->worldCoordinate_.position_.y += move.y;
    }

    
   /* ropeSp_->SetRotation(90/3.14f);*/
    /*ropeSp_->SetCutEndPoint(ropePos_);*/

}
