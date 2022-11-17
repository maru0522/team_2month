#include "GraphicsPipeline.h"
#include <string>
#include <cassert>
#include "InitDirectX.h"
#include <d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

//静的変数の実体
GraphicsPipeline GraphicsPipeline::gPipeline2d_{};
GraphicsPipeline GraphicsPipeline::gPipeline3d_{};

void GraphicsPipeline::Initialize(void)
{
    gPipeline2d_ = GraphicsPipeline(GraphicsPipeline::GPType::SPRITE);
    gPipeline3d_ = GraphicsPipeline(GraphicsPipeline::GPType::MODEL);
}

GraphicsPipeline::GraphicsPipeline(GPType type)
{
    // インスタンス取得
    InitDirectX* iDX = InitDirectX::GetInstance();
    HRESULT r = S_FALSE;

#pragma region SPRITEのGraphicsPipeline
    if (type == GPType::SPRITE) {
#pragma region 頂点レイアウトの設定
        // 頂点レイアウト
        D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
            {   // xyz座標(1行で書いたほうが見やすい)
                "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
            },
            { // uv座標(1行で書いたほうが見やすい)
                "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
            },
        };
#pragma endregion

#pragma region シェーダーの設定
        ComPtr<ID3DBlob> vsBlob{ nullptr }; // 頂点シェーダオブジェクト
        ComPtr<ID3DBlob> psBlob{ nullptr }; // ピクセルシェーダオブジェクト
        ComPtr<ID3DBlob> errorBlob{ nullptr }; // エラーオブジェクト

#pragma region 頂点シェーダ
        // 頂点シェーダの読み込みとコンパイル
        r = D3DCompileFromFile(L"Resources/Shaders/SpriteVS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vsBlob, &errorBlob);

        // エラーなら
        if (FAILED(r))
        {
            // errorBlobからエラー内容をstring型にコピー
            std::string error;
            error.resize(errorBlob->GetBufferSize());
            std::copy_n((char*)errorBlob->GetBufferPointer(),
                errorBlob->GetBufferSize(),
                error.begin());
            error += "\n";
            // エラー内容を出力ウィンドウに表示
            OutputDebugStringA(error.c_str());
            assert(0);
        }
#pragma endregion

#pragma region ピクセルシェーダ
        // ピクセルシェーダの読み込みとコンパイル
        r = D3DCompileFromFile(L"Resources/Shaders/SpritePS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &psBlob, &errorBlob);

        // エラーなら
        if (FAILED(r))
        {
            // errorBlobからエラー内容をstring型にコピー
            std::string error;
            error.resize(errorBlob->GetBufferSize());
            std::copy_n((char*)errorBlob->GetBufferPointer(),
                errorBlob->GetBufferSize(),
                error.begin());
            error += "\n";
            // エラー内容を出力ウィンドウに表示
            OutputDebugStringA(error.c_str());
            assert(0);
        }
#pragma endregion
#pragma endregion

#pragma region パイプラインの設定（下記）
        // シェーダーの設定
        pipelineDesc_.VS.pShaderBytecode = vsBlob->GetBufferPointer();
        pipelineDesc_.VS.BytecodeLength = vsBlob->GetBufferSize();
        pipelineDesc_.PS.pShaderBytecode = psBlob->GetBufferPointer();
        pipelineDesc_.PS.BytecodeLength = psBlob->GetBufferSize();

        // サンプルマスクの設定
        pipelineDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

        // ラスタライザの設定
        pipelineDesc_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // カリングしない
        pipelineDesc_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
        pipelineDesc_.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

#pragma region ブレンド設定
    // レンダーターゲットのブレンド設定（ブレンドステート）※これメンバにした方がいいんじゃね？？
        D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc_.BlendState.RenderTarget[0];
        blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA全てのチャンネルを描画

        // 共通設定（アルファ値）
        blenddesc.BlendEnable = true;                           // ブレンドを有効にする
        blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;            // 加算
        blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;              // ソースの値を100%使う
        blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;            // デストの値を0%使う

        // 加算合成
        blenddesc.BlendOp = D3D12_BLEND_OP_ADD;                 // 加算
        blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;             // ソースの値を100%使う
        blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;        // デストの値を100%使う

        // 減算合成(RGB値)
        blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;        // デストからソースを減算
        blenddesc.SrcBlend = D3D12_BLEND_ONE;                   // ソースの値を100%使う
        blenddesc.DestBlend = D3D12_BLEND_ONE;                  // デストの値を100%使う

        // 色反転(RGB値)
        blenddesc.BlendOp = D3D12_BLEND_OP_ADD;                 // 加算
        blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;        // 1.0f - デストカラーの値
        blenddesc.DestBlend = D3D12_BLEND_ZERO;                 // 使わない

        // 半透明合成
        blenddesc.BlendOp = D3D12_BLEND_OP_ADD;                 // 加算
        blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;             // ソースのアルファ値
        blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;        // 1.0f - ソースのアルファ値
#pragma endregion

        // 頂点レイアウトの設定
        pipelineDesc_.InputLayout.pInputElementDescs = inputLayout;
        pipelineDesc_.InputLayout.NumElements = _countof(inputLayout);

        // 図形の形状設定
        pipelineDesc_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

        // その他の設定
        pipelineDesc_.NumRenderTargets = 1; // 描画対象は1つ
        pipelineDesc_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
        pipelineDesc_.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

#pragma region デスクリプタレンジ
    // デスクリプタレンジの変数宣言
        D3D12_DESCRIPTOR_RANGE descriptorRange{};
        // デスクリプタレンジの設定
        descriptorRange.NumDescriptors = 1;
        descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        descriptorRange.BaseShaderRegister = 0; // テクスチャレジスタ0番
        descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
#pragma endregion

#pragma region ルートパラメータ
        // ルートパラメーターの設定
        D3D12_ROOT_PARAMETER rootParams[3] = {};
        rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;    // 定数バッファビュー
        rootParams[0].Descriptor.ShaderRegister = 0;                    // 定数バッファ番号
        rootParams[0].Descriptor.RegisterSpace = 0;                     // デフォルト値
        rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;   // 全てのシェーダから見える
        // テクスチャレジスタ0番
        rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	// 種類
        rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
        rootParams[1].DescriptorTable.NumDescriptorRanges = 1;
        rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        // 定数バッファ1番
        rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;    // 種類
        rootParams[2].Descriptor.ShaderRegister = 1;                    // 定数バッファ番号
        rootParams[2].Descriptor.RegisterSpace = 0;                     // デフォルト値
        rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;   // 全てのシェーダから見える
#pragma endregion

#pragma region サンプラーの設定
    //テクスチャサンプラーの変数宣言
        D3D12_STATIC_SAMPLER_DESC samplerDesc{};
        //テクスチャサンプラーの設定
        samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//横繰り返し(タイリング)
        samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//縦繰り返し（タイリング）
        samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//奥行繰り返し（タイリング）
        samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//ボーダーの時は黒
        samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//全てシニア補間
        samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;//ミップマップ最大値
        samplerDesc.MinLOD = 0.0f;//ミップマップ最小値
        samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
        samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//ピクセルシェーダからのみ使用可能
#pragma endregion

#pragma region ルートシグネチャ
    // ルートシグネチャの変数宣言
        D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
        // ルートシグネチャの設定
        rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        rootSignatureDesc.pParameters = rootParams; // ルートパラメータの先頭アドレス
        rootSignatureDesc.NumParameters = static_cast<uint32_t>(std::size(rootParams));
        rootSignatureDesc.pStaticSamplers = &samplerDesc;
        rootSignatureDesc.NumStaticSamplers = 1;

        ComPtr<ID3DBlob> rootSigBlob{ nullptr }; // ルートシグネチャのシリアライズ

        r = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
            &rootSigBlob, &errorBlob);
#ifdef _DEBUG
        assert(SUCCEEDED(r));
#endif // _DEBUG

        r = iDX->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
            IID_PPV_ARGS(&rootSignature_));
#ifdef _DEBUG
        assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

        // パイプラインにルートシグネチャをセット
        pipelineDesc_.pRootSignature = rootSignature_.Get();
#pragma endregion パイプラインの設定（上記）

        // パイプライン設定を保存
        r = iDX->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc_, IID_PPV_ARGS(&pipelineState_));
#ifdef _DEBUG
        assert(SUCCEEDED(r));
#endif // _DEBUG
    }
#pragma endregion
#pragma region MODELのGraphicsPipeline
    else if (type == GPType::MODEL) { // MODELタイプ
#pragma region 頂点レイアウト
    // 頂点レイアウト
    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        { // xyz座標(1行で書いたほうが見やすい)
            "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
        },
        { // 法線ベクトル（1行書いたほうがわかりやすい)
            "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        { // uv座標(1行で書いたほうが見やすい)
            "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
        },
    };
#pragma endregion

#pragma region シェーダーの設定
#pragma region 頂点シェーダー
    ComPtr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
    ComPtr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
    ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト
    // 頂点シェーダの読み込みとコンパイル
    r = D3DCompileFromFile(L"Resources/Shaders/ModelVS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vsBlob, &errorBlob);

    // エラーなら
    if (FAILED(r)) {
        // errorBlobからエラー内容をstring型にコピー
        std::string error;
        error.resize(errorBlob->GetBufferSize());
        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            error.begin());
        error += "\n";
        // エラー内容を出力ウィンドウに表示
        OutputDebugStringA(error.c_str());
        assert(0);
    }
#pragma endregion

#pragma region ピクセルシェーダ
    // ピクセルシェーダの読み込みとコンパイル
    r = D3DCompileFromFile(L"Resources/Shaders/ModelPS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &psBlob, &errorBlob);

    // エラーなら
    if (FAILED(r)) {
        // errorBlobからエラー内容をstring型にコピー
        std::string error;
        error.resize(errorBlob->GetBufferSize());
        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            error.begin());
        error += "\n";
        // エラー内容を出力ウィンドウに表示
        OutputDebugStringA(error.c_str());
        assert(0);
    }
#pragma endregion
#pragma endregion

#pragma region パイプラインの設定（下記）
    // シェーダーの設定
    pipelineDesc_.VS.pShaderBytecode = vsBlob->GetBufferPointer();
    pipelineDesc_.VS.BytecodeLength = vsBlob->GetBufferSize();
    pipelineDesc_.PS.pShaderBytecode = psBlob->GetBufferPointer();
    pipelineDesc_.PS.BytecodeLength = psBlob->GetBufferSize();

    // サンプルマスクの設定
    pipelineDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

    // ラスタライザの設定
    pipelineDesc_.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;   // 背面をカリング
    pipelineDesc_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;  // ポリゴン内塗りつぶし
    pipelineDesc_.RasterizerState.DepthClipEnable = true;            // 深度クリッピングを有効に

#pragma region ブレンド設定
    // レンダーターゲットのブレンド設定（ブレンドステート）※これメンバにした方がいいんじゃね？？
    D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = pipelineDesc_.BlendState.RenderTarget[0];
    blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画

    // 共通設定(アルファ値)
    blendDesc.BlendEnable = true;					// ブレンドを有効にする
    blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	// 加算
    blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;		// ソースの値を100%使う
    blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;	// デストの値を0%使う

    // 加算合成(RGB値)
    blendDesc.BlendOp = D3D12_BLEND_OP_ADD;		// 加算
    blendDesc.SrcBlend = D3D12_BLEND_ONE;		// ソースの値を100%使う
    blendDesc.DestBlend = D3D12_BLEND_ONE;		// デストの値を100%使う

    // 減算合成(RGB値)
    blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;		// デストカラソースヲを減算
    blendDesc.SrcBlend = D3D12_BLEND_ONE;					// ソースの値を100%使う
    blendDesc.DestBlend = D3D12_BLEND_ONE;					// デストの値を100%使う

    // 色反転(RGB値)
    blendDesc.BlendOp = D3D12_BLEND_OP_ADD;					// 加算
    blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;		// 1.0f - デストカラーの値
    blendDesc.DestBlend = D3D12_BLEND_ZERO;					// 使わない

    // 半透明合成
    blendDesc.BlendOp = D3D12_BLEND_OP_ADD;					// 加算
    blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;				// ソースのアルファ値
    blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		// 1.0f - ソースのアルファ値
#pragma endregion

    // 頂点レイアウトの設定
    pipelineDesc_.InputLayout.pInputElementDescs = inputLayout;
    pipelineDesc_.InputLayout.NumElements = _countof(inputLayout);

    // 図形の形状設定
    pipelineDesc_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    // その他の設定
    pipelineDesc_.NumRenderTargets = 1; // 描画対象は1つ
    pipelineDesc_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
    pipelineDesc_.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

#pragma region デスクリプタレンジ
    // デスクリプタレンジの設定
    D3D12_DESCRIPTOR_RANGE descriptorRange{};
    descriptorRange.NumDescriptors = 1;
    descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    descriptorRange.BaseShaderRegister = 0;	// テクスチャレジスタ0番
    descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
#pragma endregion

#pragma region ルートパラメータ
    // ルートパラメーターの設定
    D3D12_ROOT_PARAMETER rootParams[3] = {};
    // 定数バッファ0番
    rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// 定数バッファビュー
    rootParams[0].Descriptor.ShaderRegister = 0;					// 定数バッファ番号
    rootParams[0].Descriptor.RegisterSpace = 0;						// デフォルト値
    rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// 全てのシェーダから見える
    // 定数バッファ1番
    rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// 種類
    rootParams[1].Descriptor.ShaderRegister = 1;					// 定数バッファ番号
    rootParams[1].Descriptor.RegisterSpace = 0;						// デフォルト値
    rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// 全てのシェーダから見える
    // テクスチャレジスタ0番
    rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	// 種類
    rootParams[2].DescriptorTable.pDescriptorRanges = &descriptorRange;
    rootParams[2].DescriptorTable.NumDescriptorRanges = 1;
    rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
#pragma endregion

#pragma region サンプラーの設定
    //テクスチャサンプラーの設定
    D3D12_STATIC_SAMPLER_DESC samplerDesc{};
    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//横繰り返し(タイリング)
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//縦繰り返し（タイリング）
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//奥行繰り返し（タイリング）
    samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//ボーダーの時は黒
    samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//全てシニア補間
    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;//ミップマップ最大値
    samplerDesc.MinLOD = 0.0f;//ミップマップ最小値
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//ピクセルシェーダからのみ使用可能
#pragma endregion

#pragma region ルートシグネチャ
    //ルートシグネチャの設定
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSignatureDesc.pParameters = rootParams;//ルートパラメータの先頭アドレス
    rootSignatureDesc.NumParameters = _countof(rootParams);//ルートパラメータ数
    rootSignatureDesc.pStaticSamplers = &samplerDesc;
    rootSignatureDesc.NumStaticSamplers = 1;

    ComPtr<ID3DBlob> rootSigBlob{ nullptr }; // ルートシグネチャのシリアライズ
    r = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
        &rootSigBlob, &errorBlob);
    assert(SUCCEEDED(r));
    r = iDX->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
        IID_PPV_ARGS(&rootSignature_));
    assert(SUCCEEDED(r));
#pragma endregion

    // パイプラインにルートシグネチャをセット
    pipelineDesc_.pRootSignature = rootSignature_.Get();

#pragma region 深度設定
    // デプスステンシルステートの設定
    pipelineDesc_.DepthStencilState.DepthEnable = true;  // 深度テストを行う
    pipelineDesc_.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;     // 書き込み許可
    pipelineDesc_.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;          // 小さければ合格
    pipelineDesc_.DSVFormat = DXGI_FORMAT_D32_FLOAT;                                 // 深度値フォーマット
#pragma endregion
#pragma endregion

#pragma region パイプラインステートの生成
    // パイプライン設定の保存
    r = iDX->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc_, IID_PPV_ARGS(&pipelineState_));
    assert(SUCCEEDED(r));
#pragma endregion
    }
#pragma endregion
}
