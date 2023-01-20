#pragma once
#include "Obj3d.h"
#include "Camera.h"
#include <memory>
#include "IBlock.h"
#include <DirectXMath.h>

class StartBlock : public IBlock
{
public: // ä÷êî
    StartBlock(DirectX::XMFLOAT3 pos, Type type, Camera* pCamera);

    void Update(void) override;
    void Draw(void) override;
};

