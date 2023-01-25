#pragma once
#include "IGimmickBlock.h"
#include "Obj3d.h"
#include "Camera.h"
#include <memory>
#include <DirectXMath.h>

class PowerReceiveBlock : public IGimmickBlock
{
public: // ä÷êî
    PowerReceiveBlock(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, uint32_t idxConnect, uint32_t idxReceive, Camera* pCamera);

    void Update(void) override;
    void Draw(void) override;
};

