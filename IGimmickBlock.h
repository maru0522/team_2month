#pragma once
#include "IBlock.h"
class IGimmickBlock :
    public IBlock
{
public:
    virtual ~IGimmickBlock(void) = default;

    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;

};

