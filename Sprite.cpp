#include "Sprite.h"
#include "InitDirectX.h"
#include <string>
#include "Texture.h"
#include "Window.h"
#include "Util.h"
#include<d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

Sprite::Sprite(const std::string& relativePath, const std::string& fileName)
{
#pragma region gpuHandleの受け取り
    srvGpuHandleCopy_ = Texture::GetTextureInfo(relativePath, fileName)->srvGpuHandle_;
#pragma endregion

#pragma region 頂点情報処理
    // 頂点データの設定
    vertices_.at(0) = { {   0.0f, 100.0f, 0.0f }, {0.0f, 1.0f} };    // 左下
    vertices_.at(1) = { {   0.0f,   0.0f, 0.0f }, {0.0f, 0.0f} };    // 左上
    vertices_.at(2) = { { 100.0f, 100.0f, 0.0f }, {1.0f, 1.0f} };    // 右下
    vertices_.at(3) = { { 100.0f,   0.0f, 0.0f }, {1.0f, 0.0f} };    // 右上

    // 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
    uint32_t sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * vertices_.size());

    // 頂点バッファの設定
    D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

    // リソース設定
    D3D12_RESOURCE_DESC resDesc{};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeVB; // 頂点データ全体のサイズ
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    InitDirectX* iDX = InitDirectX::GetInstance();

    // 頂点バッファの生成
    HRESULT r = iDX->GetDevice()->CreateCommittedResource(
        &heapProp, // ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &resDesc, // リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertBuff_));
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG

    // GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
    VertexSprite_st* vertMap{ nullptr };
    r = vertBuff_->Map(0, nullptr, (void**)&vertMap);
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
    // 全頂点に対して
    for (int i = 0; i < vertices_.size(); i++)
    {
        vertMap[i] = vertices_[i]; // 座標をコピー
    }

    // 繋がりを解除
    vertBuff_->Unmap(0, nullptr);
#pragma endregion

#pragma region 頂点バッファのビュー設定
    // vbview
    // GPU仮想アドレス
    vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
    // 頂点バッファのサイズ
    vbView_.SizeInBytes = sizeVB;
    // 頂点1つ分のデータサイズ
    vbView_.StrideInBytes = sizeof(vertices_[0]);
#pragma endregion

#pragma region シェーダーの設定
    ComPtr<ID3DBlob> vsBlob{ nullptr }; // 頂点シェーダオブジェクト
    ComPtr<ID3DBlob> psBlob{ nullptr }; // ピクセルシェーダオブジェクト
    ComPtr<ID3DBlob> errorBlob{ nullptr }; // エラーオブジェクト
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
    D3D12_DESCRIPTOR_RANGE descriporRange{};
    // デスクリプタレンジの設定
    descriporRange.NumDescriptors = 1;
    descriporRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    descriporRange.BaseShaderRegister = 0; // テクスチャレジスタ0番
    descriporRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
#pragma endregion

#pragma region ルートパラメータ
    // ルートパラメーターの設定
    D3D12_ROOT_PARAMETER rootParams[3] = {};
    rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;    // 定数バッファビュー
    rootParams[0].Descriptor.ShaderRegister = 0;                    // 定数バッファ番号
    rootParams[0].Descriptor.RegisterSpace = 0;                     // デフォルト値
    rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;   // 全てのシェーダから見える
    // テクスチャレジスタ0番
    rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;   // 種類
    rootParams[1].DescriptorTable.pDescriptorRanges = &descriporRange;
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

    rootSigBlob->Release();
#pragma endregion

    // パイプラインにルートシグネチャをセット
    pipelineDesc_.pRootSignature = rootSignature_.Get();
#pragma endregion パイプラインの設定（上記）

    // パイプライン設定を保存
    r = iDX->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc_, IID_PPV_ARGS(&pipelineState_));
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG

#pragma region cbTransform
    SetCBTransform();
#pragma endregion

    SetColor({ 1.0f,1.0f,1.0f,1.0f });
}

Sprite::Sprite(const std::string& pathAndFileName_or_Id, CMode mode)
{
#pragma region gpuHandleの受け取り
    if (!static_cast<int>(CMode::PATH)) {
        // コンストラクタモードがPATHの場合
        srvGpuHandleCopy_ = Texture::GetTextureInfo(pathAndFileName_or_Id)->srvGpuHandle_;
    }
    else {
        // コンストラクタモードがIDの場合
        srvGpuHandleCopy_ = Texture::GetTextureInfoById(pathAndFileName_or_Id)->srvGpuHandle_;
    }
#pragma endregion

#pragma region 頂点情報処理
    // 頂点データの設定
    vertices_.at(0) = { {   0.0f, 100.0f, 0.0f }, {0.0f, 1.0f} };    // 左下
    vertices_.at(1) = { {   0.0f,   0.0f, 0.0f }, {0.0f, 0.0f} };    // 左上
    vertices_.at(2) = { { 100.0f, 100.0f, 0.0f }, {1.0f, 1.0f} };    // 右下
    vertices_.at(3) = { { 100.0f,   0.0f, 0.0f }, {1.0f, 0.0f} };    // 右上

    // 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
    uint32_t sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * vertices_.size());

    // 頂点バッファの設定
    D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

    // リソース設定
    D3D12_RESOURCE_DESC resDesc{};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeVB; // 頂点データ全体のサイズ
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // インスタンス取得
    InitDirectX* iDX = InitDirectX::GetInstance();

    // 頂点バッファの生成
    HRESULT r = iDX->GetDevice()->CreateCommittedResource(
        &heapProp, // ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &resDesc, // リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertBuff_));
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG

    // GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
    VertexSprite_st* vertMap{ nullptr };
    r = vertBuff_->Map(0, nullptr, (void**)&vertMap);
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
    // 全頂点に対して
    for (int i = 0; i < vertices_.size(); i++)
    {
        vertMap[i] = vertices_[i]; // 座標をコピー
    }

    // 繋がりを解除
    vertBuff_->Unmap(0, nullptr);
#pragma endregion

#pragma region 頂点バッファのビュー設定
    // vbview
    // GPU仮想アドレス
    vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
    // 頂点バッファのサイズ
    vbView_.SizeInBytes = sizeVB;
    // 頂点1つ分のデータサイズ
    vbView_.StrideInBytes = sizeof(vertices_[0]);
#pragma endregion

#pragma region シェーダーの設定
    ComPtr<ID3DBlob> vsBlob{ nullptr }; // 頂点シェーダオブジェクト
    ComPtr<ID3DBlob> psBlob{ nullptr }; // ピクセルシェーダオブジェクト
    ComPtr<ID3DBlob> errorBlob{ nullptr }; // エラーオブジェクト
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
    D3D12_DESCRIPTOR_RANGE descriporRange{};
    // デスクリプタレンジの設定
    descriporRange.NumDescriptors = 1;
    descriporRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    descriporRange.BaseShaderRegister = 0; // テクスチャレジスタ0番
    descriporRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
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
    rootParams[1].DescriptorTable.pDescriptorRanges = &descriporRange;
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

#pragma region cbTransform
    SetCBTransform();
#pragma endregion

    SetColor({ 1.0f,1.0f,1.0f,1.0f });
}

void Sprite::Update(void)
{
    TransferVertex();

    // ワールド行列の初期化
    matWorld_ = DirectX::XMMatrixIdentity();

    // 回転行列の宣言と初期化
    DirectX::XMMATRIX matRot{ DirectX::XMMatrixIdentity() };
    matRot *= DirectX::XMMatrixRotationZ(rotation_); // z度回転
    matWorld_ *= matRot; // ワールド行列に回転を反映

    // 平行移動行列の宣言と初期化
    DirectX::XMMATRIX matTrans{ DirectX::XMMatrixIdentity() };
    matTrans = DirectX::XMMatrixTranslation(position_.x, position_.y, 0.0f); // 平行移動
    matWorld_ *= matTrans; // ワールド行列に平行移動を反映

    // 親を設定している場合は親のワールド行列を合成
    if (parent_) {
        matWorld_ *= parent_->matWorld_;
    } // 上の設定はワールド行列のものではなくローカル行列の為の設定となる

    // 定数バッファへ転送
    TransferMatrix();
}

void Sprite::Draw(void)
{
    if (isInvisible_) {
        return;
    }

    SetCommandsBeforeDraw();

    InitDirectX* iDX = InitDirectX::GetInstance();

#pragma region 頂点バッファ
    // 頂点バッファビューの設定コマンド
    iDX->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
#pragma endregion

#pragma region ConstBuffer
    // 定数バッファビュー(CBV)の設定コマンド
    iDX->GetCommandList()->SetGraphicsRootConstantBufferView(0, cbMaterial_.GetBuffer()->GetGPUVirtualAddress());
    iDX->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbTransform_.GetBuffer()->GetGPUVirtualAddress());
#pragma endregion

#pragma region SRV
    // SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
    iDX->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandleCopy_);
#pragma endregion

    // 描画コマンド
    iDX->GetCommandList()->DrawInstanced(static_cast<uint32_t>(vertices_.size()), 1, 0, 0); // 全ての頂点を使って描画
}

void Sprite::SetColor(DirectX::XMFLOAT4 rgba)
{
    // 値が0.0fより小さい時0.0fにする
    rgba.x = (std::max)(rgba.x, 0.0f);
    rgba.y = (std::max)(rgba.y, 0.0f);
    rgba.z = (std::max)(rgba.z, 0.0f);
    rgba.w = (std::max)(rgba.w, 0.0f);

    // 値が1.0fより大きい時1.0fにする
    rgba.x = (std::min)(1.0f, rgba.x);
    rgba.y = (std::min)(1.0f, rgba.y);
    rgba.z = (std::min)(1.0f, rgba.z);
    rgba.w = (std::min)(1.0f, rgba.w);

    // 値を書き込むと自動的に転送される
    cbMaterial_.GetConstBuffMap()->color_ = rgba;
}

void Sprite::SetColor(float_t r, float_t g, float_t b, float_t a)
{
    // 値が0.0fより小さい時0.0fにする
    r = (std::max)(r, 0.0f);
    g = (std::max)(g, 0.0f);
    b = (std::max)(b, 0.0f);
    a = (std::max)(a, 0.0f);

    // 値が1.0fより大きい時1.0fにする
    r = (std::min)(1.0f, r);
    g = (std::min)(1.0f, g);
    b = (std::min)(1.0f, b);
    a = (std::min)(1.0f, a);

    // 値を書き込むと自動的に転送される
    cbMaterial_.GetConstBuffMap()->color_ = { r,g,b,a };
}

void Sprite::SetColor255(DirectX::XMFLOAT4 rgba)
{
    // 値が0.0fより小さい時0.0fにする
    rgba.x = (std::max)(rgba.x, 0.0f);
    rgba.y = (std::max)(rgba.y, 0.0f);
    rgba.z = (std::max)(rgba.z, 0.0f);
    rgba.w = (std::max)(rgba.w, 0.0f);

    // 値が255.0fより大きい時255.0fにする
    rgba.x = (std::min)(255.0f, rgba.x);
    rgba.y = (std::min)(255.0f, rgba.y);
    rgba.z = (std::min)(255.0f, rgba.z);
    rgba.w = (std::min)(255.0f, rgba.w);

    // 正規化
    rgba = {
        static_cast<float>(rgba.x / 255.0f),
        static_cast<float>(rgba.y / 255.0f),
        static_cast<float>(rgba.z / 255.0f),
        static_cast<float>(rgba.w / 255.0f)
    };

    // 値を書き込むと自動的に転送される
    cbMaterial_.GetConstBuffMap()->color_ = rgba;
}

void Sprite::SetColor255(float_t r, float_t g, float_t b, float_t a)
{
    // 値が0.0fより小さい時0.0fにする
    r = (std::max)(r, 0.0f);
    g = (std::max)(g, 0.0f);
    b = (std::max)(b, 0.0f);
    a = (std::max)(a, 0.0f);

    // 値が255.0fより大きい時255.0fにする
    r = (std::min)(255.0f, r);
    g = (std::min)(255.0f, g);
    b = (std::min)(255.0f, b);
    a = (std::min)(255.0f, a);

    // 正規化
    DirectX::XMFLOAT4 rgba{
        static_cast<float>(r / 255.0f),
        static_cast<float>(g / 255.0f),
        static_cast<float>(b / 255.0f),
        static_cast<float>(a / 255.0f)
    };

    // 値を書き込むと自動的に転送される
    cbMaterial_.GetConstBuffMap()->color_ = rgba;
}

void Sprite::SetParent(Sprite* parent)
{
    parent_ = parent;
}

void Sprite::TransferVertex(void)
{
#pragma region アンカーポイントの変更
    float_t left{ (0.0f - anchorPoint_.x) * size_.x };
    float_t right{ (1.0f - anchorPoint_.x) * size_.x };
    float_t top{ (0.0f - anchorPoint_.y) * size_.y };
    float_t bottom{ (1.0f - anchorPoint_.y) * size_.y };
#pragma endregion

#pragma region フリップの変更
    // グラフィックスパイプラインの背面カリングをオフにしていないと描画されなくなる

    // 左右フリップ
    if (isFlipX_) {
        left = -left; // 左
        right = -right; // 右
    }

    // 上下フリップ
    if (isFlipY_) {
        top = -top; // 上
        bottom = -bottom; // 下
    }
#pragma endregion

#pragma region 頂点座標の変更
    vertices_.at(static_cast<int>(VertNum::LeftBottom)).pos_ = { left, bottom, 0.0f };      // 左下
    vertices_.at(static_cast<int>(VertNum::LeftTop)).pos_ = { left, top, 0.0f };            // 左上
    vertices_.at(static_cast<int>(VertNum::RightBottom)).pos_ = { right, bottom, 0.0f };    // 右下
    vertices_.at(static_cast<int>(VertNum::RightTop)).pos_ = { right, top, 0.0f };          // 右上
#pragma endregion

#pragma region 頂点バッファビュー
    vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress(); // GPUが参照するアドレスを渡す
    vbView_.SizeInBytes = static_cast<UINT>(sizeof(vertices_[0]) * vertices_.size()); // 頂点バッファのサイズ = 頂点データ全体のサイズ
    vbView_.StrideInBytes = sizeof(vertices_[0]); // 頂点1つ分のデータサイズ
#pragma endregion

#pragma region 座標の更新
    // GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
    VertexSprite_st* vertMap{ nullptr };
    // マッピング
    HRESULT r = vertBuff_->Map(0, nullptr, (void**)&vertMap);
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG

    // 座標のコピー
    std::copy(vertices_.begin(), vertices_.end(), vertMap);

    // マッピング解除
    vertBuff_->Unmap(0, nullptr);
#pragma endregion
}

void Sprite::TransferMatrix(void)
{
    cbTransform_.GetConstBuffMap()->mat_ = matWorld_ * matProjection_;
}

void Sprite::SetCBTransform(void)
{
#pragma region cbTransform
    cbTransform_.GetConstBuffMap()->mat_ = DirectX::XMMatrixIdentity();
    cbTransform_.GetConstBuffMap()->mat_.r[0].m128_f32[0] = 2.0f / Window::width_;
    cbTransform_.GetConstBuffMap()->mat_.r[1].m128_f32[1] = -2.0f / Window::height_;
    cbTransform_.GetConstBuffMap()->mat_.r[3].m128_f32[0] = -1.0f;
    cbTransform_.GetConstBuffMap()->mat_.r[3].m128_f32[1] = 1.0f;
#pragma endregion
}

void Sprite::SetCommandsBeforeDraw(void)
{
    InitDirectX* iDX = InitDirectX::GetInstance();

# pragma region 共通
    // パイプラインステートとルートシグネチャの設定コマンド
    iDX->GetCommandList()->SetPipelineState(pipelineState_.Get());
    iDX->GetCommandList()->SetGraphicsRootSignature(rootSignature_.Get());
    // プリミティブ形状の設定コマンド
    iDX->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 三角形リスト

    ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetSRVHeap() };
    // SRVヒープの設定コマンド
    iDX->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
#pragma endregion
}
