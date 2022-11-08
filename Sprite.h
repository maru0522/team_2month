#pragma once
#include <array>
#include <DirectXMath.h>
#include <wrl.h>
#include "ConstBuffer.h"
#include <string>
#include "Window.h"
#include <d3d12.h>

#pragma comment(lib,"d3d12.lib")

enum class CMode // ConstructorMode
{
    PATH,   // 1.pathとfileの名前
    ID      // 2.設定したid
};

class Sprite
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
    struct Vertex_st
    {
        DirectX::XMFLOAT3 pos_;       // xyz座標
        //DirectX::XMFLOAT3 normal;    // 法線ベクトル
        DirectX::XMFLOAT2 uv_;        // uv座標
    };

    enum class VertNum
    {
        LeftBottom,     // 左下
        LeftTop,        // 左上
        RightBottom,    // 右下
        RightTop        // 右上
    };

public: // 関数
    Sprite(const std::string& relativePath, const std::string& fileName);
    Sprite(const std::string& pathAndFileName_or_Id, CMode mode);
    void Update(void);
    void Draw(void);

#pragma region Setter
    void SetColor(DirectX::XMFLOAT4 rgba = { 1.0f,1.0f,1.0f,1.0f });
    void SetColor(float_t r = 1.0f, float_t g = 1.0f, float_t b = 1.0f, float_t a = 1.0f);

    // RGBA値指定で色変更 0~255で指定
    void SetColor255(DirectX::XMFLOAT4 rgba = { 255.0f,255.0f,255.0f,255.0f });
    void SetColor255(float_t r = 255.0f, float_t g = 255.0f, float_t b = 255.0f, float_t a = 255.0f);

    // 親を設定
    void SetParent(Sprite* parent);

    // 座標を設定
    void SetPosition(const DirectX::XMFLOAT2& position) { position_ = position; }

    // 回転角を設定
    void SetRotation(float_t fRadians) { rotation_ = fRadians; }

    // 表示サイズ（ピクセル）を設定
    void SetSize(const DirectX::XMFLOAT2& size) { size_ = size; }
#pragma endregion
#pragma region getter
    // 座標を取得
    const DirectX::XMFLOAT2& GetPosition(void) const { return position_; }

    // 回転角を取得
    float_t GetRotation(void) const { return rotation_; }

    // 表示サイズ（ピクセル）を取得
    const DirectX::XMFLOAT2& GetSize(void) const { return size_; }
#pragma endregion
private: // 関数
    void TransferVertex(void);
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
    DirectX::XMFLOAT2 position_{ 0.0f, 0.0f }; // 座標
    float_t rotation_{ 0.0f }; // 回転角
    DirectX::XMFLOAT2 size_{ 100.0f,100.0f }; //表示サイズ（ピクセル）
};