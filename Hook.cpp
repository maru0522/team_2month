#include "Hook.h"

Hook::Hook(DirectX::XMFLOAT3 pos, Camera* pCamera)
{
    body_->worldCoordinate_.position_ = pos;
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
