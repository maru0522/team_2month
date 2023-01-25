#pragma once
#include "Camera.h"
#include <DirectXMath.h>
#include <memory>
#include "Obj3d.h"
#include "IBlock.h"

class Hook : public IBlock
{
public: // ä÷êî
    Hook(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, uint32_t idxHook, Camera* pCamera);

    void Update(void) override;
    void Draw(void) override;

};

