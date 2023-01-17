#include "Player.h"
#include "Input.h"

Player::Player(Camera* pCamera)
{
    object_ = std::make_unique<Obj3d>("Resources/3dModels/cube/cube.obj", pCamera);
    object_->SetTexture("Resources/mario.jpg");
    ropeObj_ = std::make_unique<Obj3d>("Resources/3dModels/cube/cube.obj", pCamera);

    ropeObj_->worldCoordinate_.rotation_ = { 0.0f,0.0f,Util::Convert::ToRadian(90.0f) };
}

void Player::Update(void)
{
    oldPos_ = object_->worldCoordinate_.position_;

    ropeObj_->worldCoordinate_.scale_ = { ropeUpLimit_,1.0f,1.0f };

    if (KEYS::IsTrigger(DIK_RETURN))
    {
        if (isThrow_ == false)
        {

            LimitDecrease_ = true;
            isThrow_ = true;

        }
        else
        {
            isThrow_ = false;
        }
    }

    if (KEYS::IsTrigger(DIK_1))
    {
        if (isConduction_ == false)
        {


            isConduction_ = true;

        }
        else
        {
            isConduction_ = false;
        }
    }

    if (isThrow_ == false)
    {

        Move();
        ropeObj_->worldCoordinate_.position_.x = object_->worldCoordinate_.position_.x + 3.0f;
        ropeObj_->worldCoordinate_.position_.y = object_->worldCoordinate_.position_.y + 5.0f;
        ropeObj_->worldCoordinate_.position_.z = object_->worldCoordinate_.position_.z;

        if (ropeUpLimit_ <= 0.0f)
        {
            ropeUpLimit_ = 0.0f;
        }

    }
    else
    {
        Throw();
    }

    if (LimitDecrease_ == true)
    {
        ropeUpLimit_--;
        LimitDecrease_ = false;
    }
    if (LimitDecrease_ == false)
    {

    }
    Gravity();
    Jump();

    object_->Update();

    ropeObj_->Update();
}

void Player::Draw(void)
{
    if (isThrow_ == true)
    {
        if (ropeObj_->worldCoordinate_.scale_.x >= 1.0f)
        {
            ropeObj_->Draw();
        }

    }
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

    object_->worldCoordinate_.position_.y += jumpValue_;
}

void Player::Gravity(void)
{
    object_->worldCoordinate_.position_.y -= gravity_;
}

void Player::Throw(void)
{

    DirectX::XMFLOAT3 move{};

    if (isThrow_ == true)
    {
        if (KEYS::IsDown(DIK_W)) {
            if (object_->worldCoordinate_.position_.y <= ropeObj_->worldCoordinate_.position_.y + ropeUpLimit_ - 1.0f)
            {
                move.y += speed_ / 2.5f;
            }

        }
        if (KEYS::IsDown(DIK_S)) {
            move.y -= speed_ / 2.5f;
        }
        if (isJump_ == true)
        {
            if (KEYS::IsDown(DIK_A)) {
                move.x -= speed_;
            }
            if (KEYS::IsDown(DIK_D)) {
                move.x += speed_;
            }
        }
        object_->worldCoordinate_.position_.x += move.x;
        object_->worldCoordinate_.position_.y += move.y;
    }


    /* ropeSp_->SetRotation(90/3.14f);*/
     /*ropeSp_->SetCutEndPoint(ropePos_);*/

}
