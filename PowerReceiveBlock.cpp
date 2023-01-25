#include "PowerReceiveBlock.h"

PowerReceiveBlock::PowerReceiveBlock(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, uint32_t idxConnect, uint32_t idxReceive, Camera* pCamera)
{
    body_->worldCoordinate_.position_ = pos;
    body_->worldCoordinate_.scale_ = scale;
    radius_ = { 1.f * scale.x, 1.f * scale.y, 1.f * scale.z };
    idxReceive_ = idxReceive;
    idxConnect_ = idxConnect;
    type_ = Type::POWERRECEIVE;
    body_->SetCamera(pCamera);
    body_->SetTexture("Resources/Image/Type_POWERRECEIVE.png");
}

void PowerReceiveBlock::Update(void)
{
    body_->Update();
}

void PowerReceiveBlock::Draw(void)
{
    body_->Draw();
}
