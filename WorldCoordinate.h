#pragma once
#include <DirectXMath.h>
#include "Camera.h"
#include "ConstBuffer.h"
#include "SharedStruct.h"
#include <memory>

class WorldCoordinate
{
private: // �ÓI�萔
#pragma region �J�����w�肪�Ȃ����p
    static constexpr float_t nearZ_{ 0.1f };
    static constexpr float_t farZ_{ 1000.0f };
    static const DirectX::XMMATRIX matProjection_;

    static constexpr DirectX::XMFLOAT3 eye_{ 0, 0, -100 };       // ���_���W
    static constexpr DirectX::XMFLOAT3 target_{ 0, 0, 0 };       // �����_���W
    static constexpr DirectX::XMFLOAT3 up_{ 0, 1, 0 };           // ������x�N�g��
    static const DirectX::XMMATRIX matView_;
#pragma endregion

public: // �֐�
    WorldCoordinate(void);
    WorldCoordinate(Camera* pCamera);

    void Update(void);

#pragma region setter
    void SetParent(WorldCoordinate* parent) { parent_ = parent; }

    void SetCamera(Camera* pCamera) { pCamera_ = pCamera; }
#pragma endregion

#pragma region getter
    ConstBuffer<CBDataB0>* GetCBData(void) { return cbData_.get(); }
#pragma endregion

public: // �ϐ�
    DirectX::XMFLOAT3 scale_{ 1.0f, 1.0f, 1.0f };
    DirectX::XMFLOAT3 rotation_{ 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 position_{ 0.0f, 0.0f, 0.0f };

private: // �֐�
    void TransferMatrix(void);

private: // �ϐ�
    DirectX::XMMATRIX matWorld_{ DirectX::XMMatrixIdentity() }; 

    std::unique_ptr<ConstBuffer<CBDataB0>> cbData_{};

    WorldCoordinate* parent_{ nullptr };
    Camera* pCamera_{ nullptr };
};