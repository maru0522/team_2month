#include "PowerSupplyBlock.h"

PowerSupplyBlock::PowerSupplyBlock(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, Camera* pCamera)
{
    body_->worldCoordinate_.position_ = pos;
    body_->worldCoordinate_.scale_ = scale;
    radius_ = { 1.f * scale.x, 1.f * scale.y, 1.f * scale.z };
    type_ = Type::POWERSUPPLY;
    body_->SetCamera(pCamera);
    body_->SetTexture("Resources/Image/Type_POWERSUPPLY.png");
}

void PowerSupplyBlock::Update(void)
{
    body_->Update();
}

void PowerSupplyBlock::Draw(void)
{
    body_->Draw();
}
