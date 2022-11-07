#pragma once
#include <array>
#include <DirectXMath.h>
#include <wrl.h>
#include "ConstBuffer.h"
#include <string>
#include "Window.h"
#include <d3d12.h>

#pragma comment(lib,"d3d12.lib")

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
struct Vertex_st
{
    DirectX::XMFLOAT3 pos_;       // xyz座標
    //DirectX::XMFLOAT3 normal;    // 法線ベクトル
    DirectX::XMFLOAT2 uv_;        // uv座標
};

enum class CMode // ConstructorMode
{
    PATH,   // 1.pathとfileの名前
    ID      // 2.設定したid
};

class Sprite
{
public: // 関数
    Sprite(const std::string& relativePath, const std::string& fileName);
    Sprite(const std::string& pathAndFileName_or_Id, CMode mode);
    void Update(void);
    void Draw(void);

    // RGBA値指定で色変更 0~255で指定
    void SetColor(DirectX::XMFLOAT4 rgba = { 255.0f,255.0f,255.0f,255.0f });
    void SetColor(float_t r = 255.0f, float_t g = 255.0f, float_t b = 255.0f, float_t a = 255.0f);

    void SetParent(Sprite* parent);

private: // 関数
    void TransferMatrix(void);

    void SetCBTransform(void); // CBTrans
    void SetCommandsBeforeDraw(void); // 共通設定のコマンドリスト積み込み

private: // 変数
    // エイリアステンプレート
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    // テクスチャ情報の引き出しハンドル
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandleCopy_{};

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
    ConstBuffer<CBDataTransform_st> cbTransform_{};

    // 親
    Sprite* parent_{ nullptr };

    // 行列
    DirectX::XMMATRIX matWorld_{ DirectX::XMMatrixIdentity() }; // ワールド変換
    float_t nearZ_{ 0.0f };
    float_t farZ_{ 1.0f };
    DirectX::XMMATRIX matProjection_{ DirectX::XMMatrixOrthographicOffCenterLH(0.0f,static_cast<float_t>(Window::width_),static_cast<float_t>(Window::height_),0.0f,nearZ_,farZ_) }; // 平行投影

    // Spriteに対する情報
    DirectX::XMFLOAT3 position_{ 0,0,0 };
    float_t rotate_{ 0.0f };
};