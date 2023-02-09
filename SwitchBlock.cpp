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
    for (std::unique_ptr<IBlock>& block : *BlockManager::GetBlockList())
    {
        if (*block->GetType() == IBlock::Type::SWITCH) {
            isConnect_ = BlockManager::GetConnectMap()->at(block->GetIdxConnect());
        }
    }
   
    if (isConnect_ == false)
    {
        body_->SetTexture("Resources/Image/Type_SWITCH.png");
    }
    else
    {
        body_->SetTexture("Resources/Image/selecttile.png");
    }
        
    body_->Update();
}

void SwitchBlock::Draw(void)
{
    body_->Draw();
}
