#pragma once
#include "IBlock.h"

class BlockManager
{
public:
    using IdxHook = uint32_t;
    using IdxConnect = uint32_t;

    // íËêî
    static constexpr uint32_t maxHookBlock{ 15 };

    static void Register(IBlock* blockPtr);

    static std::list<std::unique_ptr<IBlock>>* GetBlockList(void) { return &blocks_; }
    static std::map<IdxHook, bool>* GetUnderHooksMap(void) { return &isUnderHooksMp_; }
    static std::map<IdxConnect, bool>* GetConnectMap(void) { return &isConnectMp_; }

    static void Update(void);
    static void Draw(void);

    static void DrawImgui(void);

    static void ClearAll(void);

private:
    static std::list<std::unique_ptr<IBlock>> blocks_;
    static std::map<IdxHook, bool> isUnderHooksMp_;
    static std::map<IdxConnect,bool> isConnectMp_;
};