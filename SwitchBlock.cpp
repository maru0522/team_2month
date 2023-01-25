#include "SwitchBlock.h"

SwitchBlock::SwitchBlock(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, uint32_t idxConnect, Camera* pCamera)
{
    body_->worldCoordinate_.position_ = pos;
    body_->worldCoordinate_.scale_ = scale;
    radius_ = { 1.f * scale.x, 1.f * scale.y, 1.f * scale.z };
    idxConnect_ = idxConnect;
    type_ = Type::SWITCH;
    body_->SetCamera(pCamera);
    body_->SetTexture("Resources/Image/Type_SWITCH.png");
}

void SwitchBlock::Update(void)
{
    body_->Update();
}

void SwitchBlock::Draw(void)
{
    body_->Draw();
}