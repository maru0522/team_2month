#pragma once
#include <wrl.h>
#include <d3d12.h>

#pragma comment(lib,"d3d12.lib")

class GraphicsPipeline
{
public: // 定義
    enum class GPType
    {
        SPRITE,
        MODEL
    };

private: // 定義
    // エイリアステンプレート
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // 静的関数
    static void Initialize(void);

    static GraphicsPipeline* GetGraphicsPipeLine2d(void) { return &gPipeline2d_; }
    static GraphicsPipeline* GetGraphicsPipeLine3d(void) { return &gPipeline3d_; }

private: // 静的変数
    static GraphicsPipeline gPipeline2d_;
    static GraphicsPipeline gPipeline3d_;

public: // 関数
    GraphicsPipeline() {} // 静的変数用途
    GraphicsPipeline(GPType type);

#pragma region getter
    ID3D12RootSignature* GetRootSignature(void) { return rootSignature_.Get(); }
    ID3D12PipelineState* GetPipelineState(void) { return pipelineState_.Get(); }
#pragma endregion

private: // 変数
    ComPtr<ID3D12RootSignature> rootSignature_{}; // ルートシグネチャ
    D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_{}; // グラフィックスパイプライン設定
    ComPtr<ID3D12PipelineState> pipelineState_{}; // パイプランステートの生成
};

