#include "BlockManager.h"

std::list<std::unique_ptr<IBlock>> BlockManager::blocks_{};
std::map<BlockManager::IdxHook, bool> BlockManager::isUnderHooksMp_{};
std::map<BlockManager::IdxSupply, bool> BlockManager::isSupplyMp_{};
std::map<BlockManager::IdxReceive, bool> BlockManager::isReceiveMp_{};
std::map<BlockManager::IdxConnect, bool> BlockManager::isConnectMp_{}; // receive‚ªÚ‘±‚³‚ê‚Ä‚¢‚é‚©‚Ç‚¤‚©‚ð•\‚·

void BlockManager::Register(IBlock* blockPtr)
{
    if (*blockPtr->GetType() == IBlock::Type::HOOK) {
        isUnderHooksMp_.emplace(blockPtr->GetIdxHook(), false);
    }
    if (*blockPtr->GetType() == IBlock::Type::POWERSUPPLY) {
        isSupplyMp_.emplace(blockPtr->GetIdxSupply(), false);
    }
    if (*blockPtr->GetType() == IBlock::Type::POWERRECEIVE) {
        isReceiveMp_.emplace(blockPtr->GetIdxReceive(), false);
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
#ifdef _DEBUG
    ImGui::Begin("isConnectMp");
    ImGui::Text("isConnectMp");
    for (std::pair<const IdxConnect,bool>& connectPair : isConnectMp_) {
        ImGui::Text("IdxConnect : %d", connectPair.first);
        ImGui::SameLine();
        ImGui::Text(connectPair.second ? "bool : true" : "bool : false");
    }
    ImGui::End();

    ImGui::Begin("isUnderHooksMp");
    ImGui::Text("isUnderHooksMp");
    for (std::pair<const IdxHook, bool>& hookPair : isUnderHooksMp_) {
        ImGui::Text("IdxHooks : %d", hookPair.first);
        ImGui::SameLine();
        ImGui::Text(hookPair.second ? "bool : true" : "bool : false");
    }
    ImGui::End();

    ImGui::Begin("isSupplyMp");
    ImGui::Text("isSupplyMp");
    for (std::pair<const IdxHook, bool>& supplyPair : isSupplyMp_) {
        ImGui::Text("IdxSupplies : %d", supplyPair.first);
        ImGui::SameLine();
        ImGui::Text(supplyPair.second ? "bool : true" : "bool : false");
    }
    ImGui::End();

    ImGui::Begin("isReceiveMp");
    ImGui::Text("isReceiveMp");
    for (std::pair<const IdxReceive, bool>& receivePair : isReceiveMp_) {
        ImGui::Text("IdxRecieves : %d", receivePair.first);
        ImGui::SameLine();
        ImGui::Text(receivePair.second ? "bool : true" : "bool : false");
    }
    ImGui::End();
#endif // _DEBUG
}

void BlockManager::ClearAll(void)
{
    blocks_.clear();
    isConnectMp_.clear();
    isUnderHooksMp_.clear();
    isSupplyMp_.clear();
    isReceiveMp_.clear();
}