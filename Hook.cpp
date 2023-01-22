#include "Hook.h"

Hook::Hook(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, Camera* pCamera)
{
    body_->worldCoordinate_.position_ = pos;
    body_->worldCoordinate_.scale_ = scale;
    radius_ = { 1.f * scale.x, 1.f * scale.y, 1.f * scale.z };
    type_ = Type::HOOK;
    body_->SetCamera(pCamera);
    body_->SetTexture("Resources/Image/Type_HOOK.png");
    //body_->SetModel("フックのパス");
}

void Hook::Update(void)
{
    body_->Update();
}

void Hook::Draw(void)
{
    body_->Draw();
}
