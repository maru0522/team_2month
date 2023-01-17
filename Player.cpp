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
            isHold_ = true;
            isThrow_ = true;

        }
        else
        {
            isHold_ = false;
            isThrow_ = false;
            LimitDecrease_ = true;
        }
    }

    if (isThrow_ == false)
    {

        Move();
       
        ropeObj_->worldCoordinate_.position_.x = object_->worldCoordinate_.position_.x + 3.0f;
        ropeObj_->worldCoordinate_.position_.y = object_->worldCoordinate_.position_.y + ropeUpLimit_;
        ropeObj_->worldCoordinate_.position_.z = object_->worldCoordinate_.position_.z;

        if (ropeUpLimit_ <= 0.0f)
        {
            ropeUpLimit_ = 0.0f;
        }

    }
    else
    {
        if (std::abs(ropeObj_->worldCoordinate_.position_.x - object_->worldCoordinate_.position_.x) < 2.0f &&
            /*std::abs(ropeObj_->worldCoordinate_.position_.y - object_->worldCoordinate_.position_.y) < 2.0f+ropeUpLimit_ &&*/
            std::abs(ropeObj_->worldCoordinate_.position_.z - object_->worldCoordinate_.position_.z) < 2.0f)
        {
            Throw();
        }
        else
        {
            Move();
        }
      
    }

    if (isHold_ != true)
    {
        Gravity();
    }


    if (LimitDecrease_ == true)
    {
        ropeUpLimit_--;
        LimitDecrease_ = false;
    }
    
  
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
        if (isHold_ == true)
        {
            isHold_ = false;
        }
        isJump_ = true;
        isFloat_ = true;
        jumpValue_ = jumpPower_;
    }

    if (isJump_) {
       
        if (jumpValue_ > 0.0f) {
            jumpValue_ -= jumpAttenuation_;
        }
        else {
           
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
     
}
