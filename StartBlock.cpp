#include "StartBlock.h"

StartBlock::StartBlock(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, Camera* pCamera)
{
    body_->worldCoordinate_.position_ = pos;
    body_->worldCoordinate_.scale_ = scale;
    radius_ = { 1.f * scale.x, 1.f * scale.y, 1.f * scale.z };
    type_ = Type::START;
    body_->SetCamera(pCamera);
    body_->SetTexture("Resources/Image/Type_START.png");
}

void StartBlock::Update(void)
{
    body_->Update();
}

void StartBlock::Draw(void)
{
    body_->Draw();
}
