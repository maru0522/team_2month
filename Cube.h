//#pragma once
//#include "ConstBuffer.h"
//#include <DirectXMath.h>
//#include <array>
//#include "Window.h"
//#include <string>
//#include <wrl.h>
//#include "Camera.h"
//#include <d3d12.h>
//
//#pragma comment(lib,"d3d12.lib")
//
//class Cube
//{
//private: // ��`
//#pragma region �萔�o�b�t�@�p�\���̒�`
//// �萔�o�b�t�@�p�f�[�^�\���́i�}�e���A���j
//    struct CBDataMaterial_st {
//        DirectX::XMFLOAT4 color_; // �F�iRGBA�j
//    };
//
//    // �萔�o�b�t�@�p�f�[�^�\���́i3D�ϊ��s��j
//    struct CBDataTransform_st {
//        DirectX::XMMATRIX mat_;  // 3D�ϊ��s��
//    };
//#pragma endregion
//
//    // ���_�f�[�^�\����
//    struct VertexCube_st
//    {
//        DirectX::XMFLOAT3 pos_;     // xyz���W
//        DirectX::XMFLOAT3 normal_;   // �@���x�N�g��
//        DirectX::XMFLOAT2 uv_;      // uv���W
//    };
//
//public: // �֐�
//    Cube(const std::string& pathAndFileName_or_Id);
//    Cube(const std::string& pathAndFileName_or_Id, Camera* pCamera);
//    void Update(void);
//    void Draw(void);
//
//    void SetColor(DirectX::XMFLOAT4 rgba = { 1.0f,1.0f,1.0f,1.0f });
//
//private: // �֐�
//    void TransferMatrix(void);
//
//    void SetCommandsBeforeDraw(void);
//
//private: // �ϐ�
//    // �G�C���A�X�e���v���[�g
//    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
//
//    // �e�N�X�`�����̈����o���n���h��
//    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandleCopy_{};
//
//#pragma region ���_
//    std::array<VertexCube_st, 24> vertices_{};
//    ComPtr<ID3D12Resource> vertBuff_{ nullptr }; // ���_�o�b�t�@�̐���
//    D3D12_VERTEX_BUFFER_VIEW vbView_{}; // ���_�o�b�t�@�r���[�̍쐬
//#pragma endregion
//
//#pragma region ���_�C���f�b�N�X
//    std::array<uint16_t, 36> indices_{};
//    ComPtr<ID3D12Resource> indexBuff_{ nullptr }; // �C���f�b�N�X�o�b�t�@�̐���
//    D3D12_INDEX_BUFFER_VIEW ibView_{}; // �C���f�b�N�X�o�b�t�@�r���[�̍쐬
//#pragma endregion
//
//    ComPtr<ID3D12RootSignature> rootSignature_; // ���[�g�V�O�l�`��
//    D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_; // �O���t�B�b�N�X�p�C�v���C���ݒ�
//    ComPtr<ID3D12PipelineState> pipelineState_; // �p�C�v�����X�e�[�g�̐���
//
//    // �萔�o�b�t�@
//    ConstBuffer<CBDataMaterial_st> cbMaterial_{};
//    ConstBuffer<CBDataTransform_st> cbTransform_{};
//
//    // �s��
//    DirectX::XMMATRIX matWorld_{ DirectX::XMMatrixIdentity() }; // ���[���h�ϊ�
//#pragma region �J�����w�肪�Ȃ����p
//    float_t nearZ_{ 0.1f };
//    float_t farZ_{ 1000.0f };
//    DirectX::XMMATRIX matProjection_{ DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0f), (float)Window::width_ / Window::height_, nearZ_, farZ_) };
//    //�Ă�ۂ��
//    DirectX::XMFLOAT3 eye_{ 0, 0, -100 };       // ���_���W
//    DirectX::XMFLOAT3 target_{ 0, 0, 0 };       // �����_���W
//    DirectX::XMFLOAT3 up_{ 0, 1, 0 };           // ������x�N�g��
//    DirectX::XMMATRIX matView_{ DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eye_), DirectX::XMLoadFloat3(&target_), DirectX::XMLoadFloat3(&up_)) };
//#pragma endregion
//
//    // obj�̂����
//    DirectX::XMFLOAT3 scale_{ 1.0f, 1.0f, 1.0f };
//    DirectX::XMFLOAT3 rotation_{ 1.0f, 1.0f, 0.0f };
//    DirectX::XMFLOAT3 position_{ 0.0f, 0.0f, 0.0f };
//
//    // �e�I�u�W�F�N�g�̃|�C���^
//    Cube* parent_{ nullptr };
//
//    // �J����
//    Camera* pCamera_{ nullptr };
//};