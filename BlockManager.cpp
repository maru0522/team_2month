#include "BlockManager.h"

std::list<std::unique_ptr<IBlock>> BlockManager::blocks_{};

void BlockManager::Register(IBlock* blockPtr)
{
    blocks_.emplace_back(blockPtr);
}

void BlockManager::Update(void)
{
    for (std::unique_ptr<IBlock>& block : blocks_) {
        block->Update();
    }
}

void BlockManager::Draw(void)
{
    for (std::unique_ptr<IBlock>& block : blocks_) {
        block->Draw();
    }
}
