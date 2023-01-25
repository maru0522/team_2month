#pragma once
#include "IBlock.h"

class BlockManager
{
public:
    using IdxConnect = uint32_t;

    static void Register(IBlock* blockPtr);

    static std::list<std::unique_ptr<IBlock>>* GetBlockList(void) { return &blocks_; }
    static std::map<IdxConnect, bool>* GetConnectMap(void) { return &isConnectMp_; }

    static void Update(void);
    static void Draw(void);

    static void DrawImgui(void);

    static void ClearAll(void);

private:
    static std::list<std::unique_ptr<IBlock>> blocks_;
    static std::map<IdxConnect,bool> isConnectMp_;
};