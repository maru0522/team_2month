#include "Block.h"

Block::Block(Type type, Camera* pCamera)
{
    switch (type) {
    case Type::START:
        object_ = std::make_unique<Obj3d>("Resources/3dModels/cube/cube.obj", pCamera);
        object_->SetTexture("Resources/Image/Type_START.png");
        break;

    case Type::PATH:
        object_ = std::make_unique<Obj3d>("Resources/3dModels/cube/cube.obj", pCamera);
        object_->SetTexture("Resources/Image/Type_PATH.png");
        break;
    }
}

void Block::Update(void)
{
    object_->Update();
}

void Block::Draw(void)
{
    object_->Draw();
}
