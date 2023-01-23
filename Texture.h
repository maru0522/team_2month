#pragma once
#include <string>
#include <wrl.h>
#include <d3d12.h>
#include <array>
#include <map>
#include <stdexcept>
#if _MSC_VER > 1922 && !defined(_SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING)
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#endif
#include <experimental/filesystem>

class Texture
{
public: // ��`
    // map�̌��ƒl
    using TEXTURE_KEY = std::experimental::filesystem::path;
    struct TEXTURE_VALUE
    {
        Microsoft::WRL::ComPtr<ID3D12Resource> buff_{ nullptr };
        D3D12_CPU_DESCRIPTOR_HANDLE srvCpuHandle_{};
        D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle_{};
    };
    // map�̌���ID
    using TEX_KEY_ID = std::string;

private: // ��`
    using fsPath = std::experimental::filesystem::path;

public: // �ÓI�֐�

    // srv�̏�����
    static void Initialize(void);
    static void Finalize(void);

    // �Ǎ� 
    static void Load(const fsPath& pathAndFileName);
    static void Load(const fsPath& pathAndFileName, const std::string& id);

    static const bool ExistTexture(const std::string& path);

    // �Ǎ��ς̃e�N�X�`���̏�񂾂��Ԃ�
    static const TEXTURE_VALUE* GetTextureInfo(const fsPath& pathAndFileName);
    static const TEXTURE_VALUE* GetTextureInfoById(const std::string& id); // �֐����ς������Ȃ��������ǉ���Ďv�������΂񂩂����B

    // �e�N�X�`���̃p�X�ɖ��̂�ݒ肵�ČĂяo�����ȗ����B���������t���Ă�邭�炢�̊��o�ł���
    static void SetId(const fsPath& pathAndFileName, const std::string& id);

    // �ݒ肵��ID����MAP_KEY���擾����Bpublic�ɂ��Ă͂��邪��{�֐����Ŏg��
    static const TEXTURE_KEY* GetTextureKey(const std::string& id);

    // private�ϐ�: srvHeap ���擾
    static ID3D12DescriptorHeap* GetSRVHeap(void) { return srvHeap_.Get(); }

    // ���
    static void Eject(const fsPath& pathAndFileName);

    // ID�폜
    static void DeleteId(const std::string& id);

private:
    static void GenerateMissingTexture(void);

private: // �ÓI�ϐ�
    // �G�C���A�X�e���v���[�g
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;

    static constexpr size_t maxSRVDesc_{ 256 }; // �e�N�X�`���̍ő�Ǎ���

    static std::map<TEXTURE_KEY, TEXTURE_VALUE> textures_; // SRVDesc��MAP_KEY�����Ă���
    static std::map<TEX_KEY_ID, TEXTURE_KEY> mapKeys_; // MAP_KEY��ID��R�Â��ē���Ă���
    static uint32_t indexNextDescHeap_; // ���ɏ�������DescHeap�̔ԍ����w������

private: // �ϐ�
    // ���O
    TEXTURE_KEY name_{};
    TEXTURE_VALUE info_{};

    //=============== TEXTURE_VALUE ==============//
    // ComPtr<ID3D12Resource> buffer_;            //
    // D3D12_CPU_DESCRIPTOR_HANDLE srvCPUHandle_; //
    // D3D12_GPU_DESCRIPTOR_HANDLE srvGPUHandle_; //
    //============================================//

private: // �֐�
    void SetMapKey(const fsPath& keyName);
};