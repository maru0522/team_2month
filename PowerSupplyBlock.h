#pragma once
#include "Obj3d.h"
#include "Camera.h"
#include <memory>
#include "IBlock.h"
#include <DirectXMath.h>

class PowerSupplyBlock : public IBlock
{
public: // ä÷êî
    PowerSupplyBlock(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, uint32_t idxSupply, Camera* pCamera);

    void Update(void) override;
    void Draw(void) override;
};

