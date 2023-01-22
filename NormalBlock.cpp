#include "NormalBlock.h"

NormalBlock::NormalBlock(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, Camera* pCamera)
{
    body_->worldCoordinate_.position_ = pos;
    body_->worldCoordinate_.scale_ = scale;
    radius_ = { 1.f * scale.x,1.f * scale.y, 1.f * scale.z };
    type_ = Type::NORMAL;
    body_->SetCamera(pCamera);
    body_->SetTexture("Resources/Image/Type_PATH.png");
}

void NormalBlock::Update(void)
{
    body_->Update();
}

void NormalBlock::Draw(void)
{
    body_->Draw();
}
