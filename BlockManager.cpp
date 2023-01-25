#include "BlockManager.h"

std::list<std::unique_ptr<IBlock>> BlockManager::blocks_{};
std::map<BlockManager::IdxHook, bool> BlockManager::isUnderHooksMp_{};
std::map<BlockManager::IdxConnect, bool> BlockManager::isConnectMp_{}; // receiveが接続されているかどうかを表す

void BlockManager::Register(IBlock* blockPtr)
{
    if (*blockPtr->GetType() == IBlock::Type::HOOK) {
        isUnderHooksMp_.emplace(blockPtr->GetIdxHook(), false);
    }
    if (*blockPtr->GetType() == IBlock::Type::POWERRECEIVE) {
        isConnectMp_.emplace(blockPtr->GetIdxConnect(), false);
    }
    blocks_.emplace_back(blockPtr);
}

void BlockManager::Update(void)
{
    for (std::unique_ptr<IBlock>& block : blocks_) {
        block->Update();
    }

    DrawImgui();
}

void BlockManager::Draw(void)
{
    for (std::unique_ptr<IBlock>& block : blocks_) {
        block->Draw();
    }
}

void BlockManager::DrawImgui(void)
{
    ImGui::Begin("BlockManager");
    ImGui::Text("isConnectMp");
    for (std::pair<const IdxConnect,bool>& connectPair : isConnectMp_) {
        ImGui::Text("IdxConnect : %d", connectPair.first);
        ImGui::SameLine();
        ImGui::Text(connectPair.second ? "bool : true" : "bool : false");
    }
    ImGui::End();
}

void BlockManager::ClearAll(void)
{
    blocks_.clear();
    isConnectMp_.clear();
}