#pragma once
#include <DirectXMath.h>
#include "Camera.h"
#include "ConstBuffer.h"
#include "SharedStruct.h"

class WorldCoordinate
{
private: // 静的定数
#pragma region カメラ指定がない時用
    static constexpr float_t nearZ_{ 0.1f };
    static constexpr float_t farZ_{ 1000.0f };
    static const DirectX::XMMATRIX matProjection_;

    static constexpr DirectX::XMFLOAT3 eye_{ 0, 0, -100 };       // 視点座標
    static constexpr DirectX::XMFLOAT3 target_{ 0, 0, 0 };       // 注視点座標
    static constexpr DirectX::XMFLOAT3 up_{ 0, 1, 0 };           // 上方向ベクトル
    static const DirectX::XMMATRIX matView_;
#pragma endregion

public: // 関数
    WorldCoordinate(void);
    WorldCoordinate(Camera* pCamera);

    void Update(void);

#pragma region setter
    void SetParent(WorldCoordinate* parent) { parent_ = parent; }

    void SetCamera(Camera* pCamera) { pCamera_ = pCamera; }
#pragma endregion

#pragma region getter
    ConstBuffer<CBDataB0>* GetCBData(void) { return &cbData_; }
#pragma endregion

public: // 変数
    DirectX::XMFLOAT3 scale_{ 1.0f, 1.0f, 1.0f };
    DirectX::XMFLOAT3 rotation_{ 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 position_{ 0.0f, 0.0f, 0.0f };

private: // 関数
    void TransferMatrix(void);

private: // 変数
    DirectX::XMMATRIX matWorld_{ DirectX::XMMatrixIdentity() }; 

    ConstBuffer<CBDataB0> cbData_{};

    WorldCoordinate* parent_{ nullptr };
    Camera* pCamera_{ nullptr };
};