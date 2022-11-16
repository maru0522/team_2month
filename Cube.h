#pragma once
#include "ConstBuffer.h"
#include <DirectXMath.h>
#include <array>
#include "Window.h"
#include <string>
#include <wrl.h>
#include "Camera.h"
#include <d3d12.h>

#pragma comment(lib,"d3d12.lib")

class Cube
{
private: // 定義
#pragma region 定数バッファ用構造体定義
// 定数バッファ用データ構造体（マテリアル）
    struct CBDataMaterial_st {
        DirectX::XMFLOAT4 color_; // 色（RGBA）
    };

    // 定数バッファ用データ構造体（3D変換行列）
    struct CBDataTransform_st {
        DirectX::XMMATRIX mat_;  // 3D変換行列
    };
#pragma endregion

    // 頂点データ構造体
    struct VertexCube_st
    {
        DirectX::XMFLOAT3 pos_;     // xyz座標
        DirectX::XMFLOAT3 normal_;   // 法線ベクトル
        DirectX::XMFLOAT2 uv_;      // uv座標
    };

public: // 関数
    Cube(const std::string& pathAndFileName_or_Id);
    Cube(const std::string& pathAndFileName_or_Id, Camera* pCamera);
    void Update(void);
    void Draw(void);

    void SetColor(DirectX::XMFLOAT4 rgba = { 1.0f,1.0f,1.0f,1.0f });

private: // 関数
    void TransferMatrix(void);

    void SetCommandsBeforeDraw(void);

private: // 変数
    // エイリアステンプレート
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    // テクスチャ情報の引き出しハンドル
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandleCopy_{};

#pragma region 頂点
    std::array<VertexCube_st, 24> vertices_{};
    ComPtr<ID3D12Resource> vertBuff_{ nullptr }; // 頂点バッファの生成
    D3D12_VERTEX_BUFFER_VIEW vbView_{}; // 頂点バッファビューの作成
#pragma endregion

#pragma region 頂点インデックス
    std::array<uint16_t, 36> indices_{};
    ComPtr<ID3D12Resource> indexBuff_{ nullptr }; // インデックスバッファの生成
    D3D12_INDEX_BUFFER_VIEW ibView_{}; // インデックスバッファビューの作成
#pragma endregion

    ComPtr<ID3D12RootSignature> rootSignature_; // ルートシグネチャ
    D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_; // グラフィックスパイプライン設定
    ComPtr<ID3D12PipelineState> pipelineState_; // パイプランステートの生成

    // 定数バッファ
    ConstBuffer<CBDataMaterial_st> cbMaterial_{};
    ConstBuffer<CBDataTransform_st> cbTransform_{};

    // 行列
    DirectX::XMMATRIX matWorld_{ DirectX::XMMatrixIdentity() }; // ワールド変換
#pragma region カメラ指定がない時用
    float_t nearZ_{ 0.1f };
    float_t farZ_{ 1000.0f };
    DirectX::XMMATRIX matProjection_{ DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0f), (float)Window::width_ / Window::height_, nearZ_, farZ_) };
    //てんぽらり
    DirectX::XMFLOAT3 eye_{ 0, 0, -100 };       // 視点座標
    DirectX::XMFLOAT3 target_{ 0, 0, 0 };       // 注視点座標
    DirectX::XMFLOAT3 up_{ 0, 1, 0 };           // 上方向ベクトル
    DirectX::XMMATRIX matView_{ DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eye_), DirectX::XMLoadFloat3(&target_), DirectX::XMLoadFloat3(&up_)) };
#pragma endregion

    // objのもつ情報
    DirectX::XMFLOAT3 scale_{ 1.0f, 1.0f, 1.0f };
    DirectX::XMFLOAT3 rotation_{ 1.0f, 1.0f, 0.0f };
    DirectX::XMFLOAT3 position_{ 0.0f, 0.0f, 0.0f };

    // 親オブジェクトのポインタ
    Cube* parent_{ nullptr };

    // カメラ
    Camera* pCamera_{ nullptr };
};