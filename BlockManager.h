#pragma once
#include "IBlock.h"

class BlockManager
{
public:
    static void Register(IBlock* blockPtr);

    static std::list<std::unique_ptr<IBlock>>* GetBlockList(void) { return &blocks_; }

    static void Update(void);
    static void Draw(void);

private:
    static std::list<std::unique_ptr<IBlock>> blocks_;
};