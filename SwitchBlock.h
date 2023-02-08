#pragma once
#include "IGimmickBlock.h"
#include "IBlock.h"
#include "BlockManager.h"
class SwitchBlock :
    public IGimmickBlock
{
public: // ä÷êî
    SwitchBlock(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, uint32_t idxConnect, Camera* pCamera);

    void Update(void) override;
    void Draw(void) override;
public:
    bool isConnect_;
    IBlock* idxConnect = nullptr;
};

