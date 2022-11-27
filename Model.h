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
public: // 定義
    using MODEL_KEY = std::experimental::filesystem::path;
    struct MODEL_VALUE {
        std::vector<VertexModel3d_st> vertices_{};
        std::vector<uint16_t> indices_{};

        Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_{ nullptr };
        Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff_{ nullptr };

        D3D12_VERTEX_BUFFER_VIEW vbView_{};
        D3D12_INDEX_BUFFER_VIEW ibView_{};

        MaterialModel3d_st material_{};
        ConstBuffer<CBDataB1> constBuffB1_{}; // マテリアル
    };

private: // 定義
    // エイリアステンプレート
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    using fsPath = std::experimental::filesystem::path;

public: // 静的関数
    static void Load(const fsPath& pathAndObjName); // .objまで入れる

#pragma region getter
    static const MODEL_VALUE* GetMODEL_VALUE(const fsPath& pathAndObjName);
#pragma endregion

private: // 静的関数
    static void LoadMaterial(Model& model, const fsPath& pathAndObjName);

private: // 静的変数
    static std::map<MODEL_KEY, MODEL_VALUE> models_;

public: // 関数
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
private: // 関数

    void UpdateMaterial(void); // 定数バッファのほうに値を送る。

private: // 変数
    MODEL_KEY name_;
    MODEL_VALUE info_;
};