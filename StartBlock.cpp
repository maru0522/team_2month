#include "StartBlock.h"

StartBlock::StartBlock(DirectX::XMFLOAT3 pos, Type type, Camera* pCamera)
{
    body_->worldCoordinate_.position_ = pos;
    type_ = type;
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
