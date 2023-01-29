#pragma once
#include <Windows.h>
#include "InitDirectX.h"
#include <cassert>
#include <algorithm>
#include <wrl.h>
#include <DirectXMath.h>
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")

template<typename CBType> class ConstBuffer
{
public: // �֐�
    ConstBuffer(void) {
        InitDirectX* iDX = InitDirectX::GetInstance();

        // �萔�o�b�t�@�����p�̐ݒ�
        // �q�[�v�ݒ�
        D3D12_HEAP_PROPERTIES cbHeapProp{};
        cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;               // GPU�ւ̓]���p
        // ���\�[�X�ݒ�
        D3D12_RESOURCE_DESC cbResourceDesc{};
        cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        cbResourceDesc.Width = (sizeof(CBType) + 0xff) & ~0xff; //256�o�C�g�A���C�������g
        cbResourceDesc.Height = 1;
        cbResourceDesc.DepthOrArraySize = 1;
        cbResourceDesc.MipLevels = 1;
        cbResourceDesc.SampleDesc.Count = 1;
        cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        // �萔�o�b�t�@�̐���
        HRESULT r = iDX->GetDevice()->CreateCommittedResource(
            &cbHeapProp,    // �q�[�v�ݒ�
            D3D12_HEAP_FLAG_NONE,
            &cbResourceDesc,    // ���\�[�X�ݒ�
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&buffer_));
        assert(SUCCEEDED(r));

        buffer_->SetName(L"constbufferclassBuffer");

        // �萔�o�b�t�@�̃}�b�s���O
        CBMap();
        //SetColor�͎g�p��ɈϔC
    }

    ~ConstBuffer(void) {
    }

    // private�ϐ�: cBTypeMap_ ���擾
    CBType* GetConstBuffMap(void) { return cbTypeMap_; } // �l���������p�擾�i�}�e���A���J���[�̃Z�b�e�B���O�Ƃ��j

    // private�ϐ�: cBMaterial ���擾
    ID3D12Resource* GetBuffer(void) { return buffer_.Get(); }

private: // �ϐ�
    // �G�C���A�X�e���v���[�g
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    ComPtr<ID3D12Resource> buffer_{ nullptr };

    CBType* cbTypeMap_{ nullptr };

private: // �֐�
    // cBMaterial��cMapMaterial�̃}�b�s���O
    void CBMap(void) {
        // �萔�o�b�t�@�̃}�b�s���O
        HRESULT r = buffer_->Map(0, nullptr, (void**)&cbTypeMap_);       // �}�b�s���O
        assert(SUCCEEDED(r));
    }

    // �r���ŐF��ω�������\�肪�Ȃ��Ȃ珉������Ɏg��
    // �}�b�s���O�̉���
    void CBUnMap(void) {
        buffer_->Unmap(0, nullptr);
    }
};