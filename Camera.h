#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include "Window.h"
#include "Util.h"
#include <d3d12.h>

#pragma comment(lib,"d3d12.lib")

class Camera
{
public: // ��`
    enum class Projection
    {
        ORTHOGRAPHIC,
        PERSPECTIVE,
    };

    // �֐�
    Camera(void);
    void Update(void);

    void SetProjection(Camera::Projection projectionType);
    void SwitchProjection(void);

    const DirectX::XMMATRIX& GetProjection(void) { return matProjection_; }
    const DirectX::XMMATRIX& GetView(void) { return matView_; }
    const Camera::Projection& GetProjectionState(void) { return projectionState_; }

public: // �ϐ�
    DirectX::XMFLOAT3 eye_{ 0.0f, 0.0f, -100.0f };       // ���_���W
    DirectX::XMFLOAT3 target_{ 0.0f, 0.0f, 0.0f };       // �����_���W
    DirectX::XMFLOAT3 up_{ 0.0f, 1.0f, 0.0f };           // ������x�N�g��

private: // �ϐ�
        // �G�C���A�X�e���v���[�g
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    // �������e
    float_t nearZ_{ 0.1f }; // �O�[
    float_t farZ_{ 1000.0f }; // ���[
    DirectX::XMMATRIX matProjection_{ DirectX::XMMatrixPerspectiveFovLH(Util::Convert::ToRadian(45.0f), (float)Window::width_ / Window::height_, nearZ_, farZ_) };
    Camera::Projection projectionState_{ Camera::Projection::PERSPECTIVE };

    // �r���[�ϊ��s��
    DirectX::XMMATRIX matView_{ DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eye_), DirectX::XMLoadFloat3(&target_), DirectX::XMLoadFloat3(&up_)) };
};