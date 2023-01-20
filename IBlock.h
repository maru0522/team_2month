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
        INIT,
        NORMAL,
        START,
        GOAL,
    };

    virtual ~IBlock(void) = default;

    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;

    void SetModel(const fsPath& path);

    const DirectX::XMFLOAT3* GetPos(void) { return &body_->worldCoordinate_.position_; }
    const DirectX::XMFLOAT3* GetRadius(void) { return &radius_; }

protected:
    std::unique_ptr<Obj3d> body_{ std::make_unique<Obj3d>("Resources/3dModels/cube/cube.obj") };
    DirectX::XMFLOAT3 radius_{1.0f,1.0f,1.0f}; // À•W’†S“_‚©‚ç‚Ì‹——£ ¦”¼Œa
    Type type_{};
};