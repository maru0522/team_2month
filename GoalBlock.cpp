#include "GoalBlock.h"

GoalBlock::GoalBlock(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, Camera* pCamera)
{
    body_->worldCoordinate_.position_ = pos;
    body_->worldCoordinate_.scale_ = scale;
    radius_ = { 1.f * scale.x, 1.f * scale.y, 1.f * scale.z };
    type_ = Type::GOAL;
    body_->SetCamera(pCamera);
    body_->SetTexture("Resources/Image/Type_GOAL.png");
}

void GoalBlock::Update(void)
{
    body_->Update();
}

void GoalBlock::Draw(void)
{
    body_->Draw();
}
