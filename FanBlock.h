#pragma once
#include "IGimmickBlock.h"
class FanBlock :
    public IGimmickBlock
{
public: // �֐�
    FanBlock(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, uint32_t idxConnect, Camera* pCamera);

    void Update(void) override;
    void Draw(void) override;
};

