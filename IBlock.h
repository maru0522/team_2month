#pragma once
#include "Obj3d.h"
#include <memory>
#include <list>

class IBlock
{
private:
    using fsPath = std::experimental::filesystem::path;

public:
    enum class Type
    {
        INIT,           // 0
        NORMAL,         // 1
        START,          // 2
        GOAL,           // 3
        HOOK,           // 4
        POWERSUPPLY,    // 5
        POWERRECEIVE,   // 6
        SWITCH,         // 7
    };

    virtual ~IBlock(void) = default;

    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;

    void SetModel(const fsPath& path);

    const DirectX::XMFLOAT3* GetPos(void) { return &body_->worldCoordinate_.position_; }
    const DirectX::XMFLOAT3* GetRadius(void) { return &radius_; }
    const Type* GetType(void) { return &type_; }
    
    const uint32_t GetIdxHook(void) { return idxHook_; }
    const uint32_t GetIdxSupply(void) { return idxSupply_; }
    const uint32_t GetIdxReceive(void) { return idxReceive_; }
    const uint32_t GetIdxConnect(void) { return idxConnect_; }

protected:
    std::unique_ptr<Obj3d> body_{ std::make_unique<Obj3d>("Resources/3dModels/cube/cube.obj") };
    DirectX::XMFLOAT3 radius_{1.0f,1.0f,1.0f}; // À•W’†S“_‚©‚ç‚Ì‹——£ ¦”¼Œa
    Type type_{};
    uint32_t idxHook_{}; // Hook‚Ì”Ô†
    uint32_t idxSupply_{}; // Supply‚Ì”Ô†
    uint32_t idxReceive_{}; // Receive‚Ì”Ô†
    uint32_t idxConnect_{}; // PowerReceive‚ÆGimmick‚ğ‚Â‚È‚®‚à‚Ì
};