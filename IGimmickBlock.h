#pragma once
#include "IBlock.h"
class IGimmickBlock :
    public IBlock
{
public:
    virtual ~IGimmickBlock(void) = default;

    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;

    const uint32_t GetIdxConnect(void) { return idxConnect_; }
protected:
    uint32_t idxConnect_{}; // PowerReceive‚ÆGimmick‚ð‚Â‚È‚®‚à‚Ì
};

