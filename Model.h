#pragma once
#include <DirectXMath.h>
#include <vector>
#include <wrl.h>
#include "Window.h"
#include "Camera.h"
#include "WorldCoordinate.h"
#if _MSC_VER > 1922 && !defined(_SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING)
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#endif
#include <experimental/filesystem>
#include <map>
#include "SharedStruct.h"
#include "ConstBuffer.h"
#include "GraphicsPipeline.h"
#include <d3d12.h>

#pragma comment(lib,"d3d12.lib")

class Model
{
public: // ��`
    using MODEL_KEY = std::experimental::filesystem::path;
    struct MODEL_VALUE {
        std::vector<VertexModel3d_st> vertices_{};
        std::vector<uint16_t> indices_{};

        Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_{ nullptr };
        Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff_{ nullptr };

        D3D12_VERTEX_BUFFER_VIEW vbView_{};
        D3D12_INDEX_BUFFER_VIEW ibView_{};

        MaterialModel3d_st material_{};
        ConstBuffer<CBDataB1> constBuffB1_{}; // �}�e���A��
    };

private: // ��`
    // �G�C���A�X�e���v���[�g
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    using fsPath = std::experimental::filesystem::path;

public: // �ÓI�֐�
    static void Load(const fsPath& pathAndObjName); // .obj�܂œ����

    static void Finalize(void);

#pragma region getter
    static const MODEL_VALUE* GetMODEL_VALUE(const fsPath& pathAndObjName);
#pragma endregion

private: // �ÓI�֐�
    static void LoadMaterial(Model& model, const fsPath& pathAndObjName);

private: // �ÓI�ϐ�
    static std::map<MODEL_KEY, MODEL_VALUE> models_;

public: // �֐�
    Model(void) {}
    void Update(void);

#pragma region setter
    void SetMODEL_KEY(const fsPath& pathAndObjName) { name_ = pathAndObjName; }
    void SetMODEL_VALUE(const MODEL_VALUE& info) { info_ = info; }
#pragma endregion

#pragma region getter
    MODEL_KEY* GetModelKeyName(void) { return &name_; }
    MODEL_VALUE* GetModelValueInfo(void) { return &info_; }
#pragma endregion
private: // �֐�

    void UpdateMaterial(void); // �萔�o�b�t�@�̂ق��ɒl�𑗂�B

private: // �ϐ�
    MODEL_KEY name_;
    MODEL_VALUE info_;
};