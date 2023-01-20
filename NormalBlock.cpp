#include "NormalBlock.h"

NormalBlock::NormalBlock(DirectX::XMFLOAT3 pos, Camera* pCamera)
{
    body_->worldCoordinate_.position_ = pos;
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
