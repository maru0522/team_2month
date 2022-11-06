#pragma once
#include <array>
#include <DirectXMath.h>
#include <wrl.h>
#include "ConstBuffer.h"
#include <d3d12.h>

#pragma comment(lib,"d3d12.lib")

// 定数バッファ用データ構造体（マテリアル）
struct CBDataMaterial_st {
    DirectX::XMFLOAT4 color_; // 色（RGBA）
};

// 頂点データ構造体
struct Vertex_st
{
    DirectX::XMFLOAT3 pos_;       // xyz座標
    //DirectX::XMFLOAT3 normal;    // 法線ベクトル
    DirectX::XMFLOAT2 uv_;        // uv座標
};

class Sprite
{
public: // 関数
    Sprite(void);
    void Draw(void);

    // RGBA値指定で色変更 0~255で指定
    void SetColor(DirectX::XMFLOAT4 rgba = {255.0f,255.0f,255.0f,255.0f});
    void SetColor(float_t r = 255.0f, float_t g = 255.0f, float_t b = 255.0f, float_t a = 255.0f);

private: // 関数
    void SetCommandsBeforeDraw(void); // 共通設定のコマンドリスト積み込み

private: // 変数
    // エイリアステンプレート
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#pragma region 頂点
    std::array<Vertex_st, 4> vertices_{}; // 頂点データ
    ComPtr<ID3D12Resource> vertBuff_{ nullptr }; // 頂点バッファの生成
    D3D12_VERTEX_BUFFER_VIEW vbView_{}; // 頂点バッファビューの作成
#pragma endregion

    ComPtr<ID3D12RootSignature> rootSignature_; // ルートシグネチャ
    D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_; // グラフィックスパイプライン設定
    ComPtr<ID3D12PipelineState> pipelineState_; // パイプランステートの生成

    // 定数バッファ
    ConstBuffer<CBDataMaterial_st> cbMaterial_{};
};