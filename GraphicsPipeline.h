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

public: // 関数
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

