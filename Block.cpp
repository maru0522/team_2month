#include "Block.h"

Block::Block(Camera* pCamera)
{
    object_ = std::make_unique<Obj3d>("Resources/3dModels/cube/cube.obj", pCamera);
    object_->SetTexture("Resources/thinking.png");
}

void Block::Update(void)
{
    object_->Update();
}

void Block::Draw(void)
{
    object_->Draw();
}
