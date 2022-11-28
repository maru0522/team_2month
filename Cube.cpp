#include "Cube.h"
#include "InitDirectX.h"
#include <string>
#include "Texture.h"
#include<d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

Cube::Cube(const std::string& pathAndFileName_or_Id)
{
    srvGpuHandleCopy_ = Texture::GetTextureInfo(pathAndFileName_or_Id)->srvGpuHandle_;

    // 頂点データの設定
#pragma region 頂点座標挿入
    // 前
    vertices_.at(0) = { {-5.0f, -5.0f, -5.0f }, {}, { 0.0f, 1.0f } };   // 左下 
    vertices_.at(1) = { {-5.0f,  5.0f, -5.0f }, {}, { 0.0f, 0.0f } };   // 左上
    vertices_.at(2) = { { 5.0f, -5.0f, -5.0f }, {}, { 1.0f, 1.0f } };   // 右下
    vertices_.at(3) = { { 5.0f,  5.0f, -5.0f }, {}, { 1.0f, 0.0f } };   // 右上
    // 後
    vertices_.at(4) = { {-5.0f, -5.0f,  5.0f }, {}, { 0.0f, 1.0f } };   // 左下 
    vertices_.at(5) = { {-5.0f,  5.0f,  5.0f }, {}, { 0.0f, 0.0f } };   // 左上
    vertices_.at(6) = { { 5.0f, -5.0f,  5.0f }, {}, { 1.0f, 1.0f } };   // 右下
    vertices_.at(7) = { { 5.0f,  5.0f,  5.0f }, {}, { 1.0f, 0.0f } };   // 右上
    // 左
    vertices_.at(8) = { {-5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f} };     // 左下 
    vertices_.at(9) = { {-5.0f, -5.0f,  5.0f }, {}, {0.0f, 0.0f} };     // 左上
    vertices_.at(10) = { {-5.0f,  5.0f, -5.0f }, {}, {1.0f, 1.0f} };    // 右下
    vertices_.at(11) = { {-5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f} };    // 右上
    // 右
    vertices_.at(12) = { { 5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f} };    // 左下 
    vertices_.at(13) = { { 5.0f, -5.0f,  5.0f }, {}, {0.0f, 0.0f} };    // 左上
    vertices_.at(14) = { { 5.0f,  5.0f, -5.0f }, {}, {1.0f, 1.0f} };    // 右下
    vertices_.at(15) = { { 5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f} };    // 右上
    // 上
    vertices_.at(16) = { {-5.0f,  5.0f, -5.0f }, {}, {0.0f, 1.0f} };    // 左下 
    vertices_.at(17) = { { 5.0f,  5.0f, -5.0f }, {}, {0.0f, 0.0f} };    // 左上
    vertices_.at(18) = { {-5.0f,  5.0f,  5.0f }, {}, {1.0f, 1.0f} };    // 右下
    vertices_.at(19) = { { 5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f} };    // 右上
    // 下
    vertices_.at(20) = { {-5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f} };    // 左下 
    vertices_.at(21) = { { 5.0f, -5.0f, -5.0f }, {}, {0.0f, 0.0f} };    // 左上
    vertices_.at(22) = { {-5.0f, -5.0f,  5.0f }, {}, {1.0f, 1.0f} };    // 右下
    vertices_.at(23) = { { 5.0f, -5.0f,  5.0f }, {}, {1.0f, 0.0f} };    // 右上
#pragma endregion

#pragma region インデックス配列への頂点番号挿入
    uint16_t indices[] =
    {
        // 前
            0,1,2,      //三角形1つ目
            2,1,3,      //三角形2つ目
        // 後
            4,6,5,      //三角形3つ目
            6,7,5,      //三角形4つ目
        // 左
            8,9,10,     //三角形5つ目
            10,9,11,    //三角形6つ目
        // 右
            12,14,13,   //三角形7つ目
            14,15,13,   //三角形8つ目
        // 上
            16,18,17,   //三角形9つ目
            18,19,17,   //三角形10つ目
        // 下
            20,21,22,   //三角形11つ目
            22,21,23,   //三角形12つ目
    };

    std::copy(std::begin(indices), std::end(indices), indices_.data()); // メンバにコピー
#pragma endregion

#pragma region 頂点配列のnormalへ挿入
    for (int i = 0; i < indices_.size() / 3; ++i) {
        // 三角形1つごとに計算していく
        // 三角形のインデックスを取り出して、一時的な変数に入れる
        uint16_t index0 = indices_[i * 3 + 0];
        uint16_t index1 = indices_[i * 3 + 1];
        uint16_t index2 = indices_[i * 3 + 2];
        // 三角形を構成する頂点座標をベクトルに代入
        DirectX::XMVECTOR p0 = DirectX::XMLoadFloat3(&vertices_[index0].pos_);
        DirectX::XMVECTOR p1 = DirectX::XMLoadFloat3(&vertices_[index1].pos_);
        DirectX::XMVECTOR p2 = DirectX::XMLoadFloat3(&vertices_[index2].pos_);
        // p0->p1ベクトル、p0->p2ベクトルを計算（ベクトルの減算）
        DirectX::XMVECTOR v1 = DirectX::XMVectorSubtract(p1, p0);
        DirectX::XMVECTOR v2 = DirectX::XMVectorSubtract(p2, p0);
        // 外積は両方から垂直なベクトル
        DirectX::XMVECTOR normal_ = DirectX::XMVector3Cross(v1, v2);
        // 正規化（長さを1にする）
        normal_ = DirectX::XMVector3Normalize(normal_);
        // 求めた法線を頂点データに代入
        DirectX::XMStoreFloat3(&vertices_[index0].normal_, normal_);
        DirectX::XMStoreFloat3(&vertices_[index1].normal_, normal_);
        DirectX::XMStoreFloat3(&vertices_[index2].normal_, normal_);
    }
#pragma endregion

#pragma region 頂点バッファの生成
    // 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
    uint32_t sizeVB = static_cast<uint32_t>(sizeof(vertices_[0]) * vertices_.size());

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
#pragma endregion

#pragma region 頂点バッファのマッピング
    // GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
    VertexCube_st* vertMap{ nullptr };
    r = vertBuff_->Map(0, nullptr, (void**)&vertMap);
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
    // 全頂点に対して
    for (int i = 0; i < vertices_.size(); i++)
    {
        vertMap[i] = vertices_[i];//座標をコピー
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

#pragma region 頂点シェーダー
    ComPtr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
    ComPtr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
    ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト
    // 頂点シェーダの読み込みとコンパイル
    r = D3DCompileFromFile(L"BasicVS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vsBlob, &errorBlob);

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
    r = D3DCompileFromFile(L"BasicPS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &psBlob, &errorBlob);

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
    // テクスチャレジスタ0番
    rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	// 種類
    rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
    rootParams[1].DescriptorTable.NumDescriptorRanges = 1;
    rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    // 定数バッファ1番
    rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// 種類
    rootParams[2].Descriptor.ShaderRegister = 1;					// 定数バッファ番号
    rootParams[2].Descriptor.RegisterSpace = 0;						// デフォルト値
    rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// 全てのシェーダから見える
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

    SetColor();

    //SetCBTransform();

#pragma region インデックスバッファの生成
    // インデックスデータ全体のサイズ
    uint32_t sizeIB = static_cast<uint32_t>(sizeof(uint16_t) * indices_.size());

    // リソース設定
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeIB;	// インデックス情報が入る分のサイズ
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // インデックスバッファの生成
    r = iDX->GetDevice()->CreateCommittedResource(
        &heapProp,	// ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &resDesc,	// リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&indexBuff_));
#pragma endregion

#pragma region インデックスバッファのマッピング
    // インデックスバッファをマッピング
    uint16_t* indexMap = nullptr;
    r = indexBuff_->Map(0, nullptr, (void**)&indexMap);
    // 全インデックスに対して
    for (int i = 0; i < indices_.size(); i++) {
        indexMap[i] = indices_[i];	// インデックスをコピー
    }
    // マッピング解除
    indexBuff_->Unmap(0, nullptr);
#pragma endregion

#pragma region インデックスバッファのビュー設定
    // インデックスバッファビューの作成
    ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
    ibView_.Format = DXGI_FORMAT_R16_UINT;
    ibView_.SizeInBytes = sizeIB;
#pragma endregion
}

Cube::Cube(const std::string& pathAndFileName_or_Id, Camera* pCamera)
{
    srvGpuHandleCopy_ = Texture::GetTextureInfo(pathAndFileName_or_Id)->srvGpuHandle_;

    pCamera_ = pCamera;

    // 頂点データの設定
#pragma region 頂点座標挿入
    // 前
    vertices_.at(0) = { {-5.0f, -5.0f, -5.0f }, {}, { 0.0f, 1.0f } };   // 左下 
    vertices_.at(1) = { {-5.0f,  5.0f, -5.0f }, {}, { 0.0f, 0.0f } };   // 左上
    vertices_.at(2) = { { 5.0f, -5.0f, -5.0f }, {}, { 1.0f, 1.0f } };   // 右下
    vertices_.at(3) = { { 5.0f,  5.0f, -5.0f }, {}, { 1.0f, 0.0f } };   // 右上
    // 後
    vertices_.at(4) = { {-5.0f, -5.0f,  5.0f }, {}, { 0.0f, 1.0f } };   // 左下 
    vertices_.at(5) = { {-5.0f,  5.0f,  5.0f }, {}, { 0.0f, 0.0f } };   // 左上
    vertices_.at(6) = { { 5.0f, -5.0f,  5.0f }, {}, { 1.0f, 1.0f } };   // 右下
    vertices_.at(7) = { { 5.0f,  5.0f,  5.0f }, {}, { 1.0f, 0.0f } };   // 右上
    // 左
    vertices_.at(8) = { {-5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f} };     // 左下 
    vertices_.at(9) = { {-5.0f, -5.0f,  5.0f }, {}, {0.0f, 0.0f} };     // 左上
    vertices_.at(10) = { {-5.0f,  5.0f, -5.0f }, {}, {1.0f, 1.0f} };    // 右下
    vertices_.at(11) = { {-5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f} };    // 右上
    // 右
    vertices_.at(12) = { { 5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f} };    // 左下 
    vertices_.at(13) = { { 5.0f, -5.0f,  5.0f }, {}, {0.0f, 0.0f} };    // 左上
    vertices_.at(14) = { { 5.0f,  5.0f, -5.0f }, {}, {1.0f, 1.0f} };    // 右下
    vertices_.at(15) = { { 5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f} };    // 右上
    // 上
    vertices_.at(16) = { {-5.0f,  5.0f, -5.0f }, {}, {0.0f, 1.0f} };    // 左下 
    vertices_.at(17) = { { 5.0f,  5.0f, -5.0f }, {}, {0.0f, 0.0f} };    // 左上
    vertices_.at(18) = { {-5.0f,  5.0f,  5.0f }, {}, {1.0f, 1.0f} };    // 右下
    vertices_.at(19) = { { 5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f} };    // 右上
    // 下
    vertices_.at(20) = { {-5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f} };    // 左下 
    vertices_.at(21) = { { 5.0f, -5.0f, -5.0f }, {}, {0.0f, 0.0f} };    // 左上
    vertices_.at(22) = { {-5.0f, -5.0f,  5.0f }, {}, {1.0f, 1.0f} };    // 右下
    vertices_.at(23) = { { 5.0f, -5.0f,  5.0f }, {}, {1.0f, 0.0f} };    // 右上
#pragma endregion

#pragma region インデックス配列への頂点番号挿入
    uint16_t indices[] =
    {
        // 前
            0,1,2,      //三角形1つ目
            2,1,3,      //三角形2つ目
        // 後
            4,6,5,      //三角形3つ目
            6,7,5,      //三角形4つ目
        // 左
            8,9,10,     //三角形5つ目
            10,9,11,    //三角形6つ目
        // 右
            12,14,13,   //三角形7つ目
            14,15,13,   //三角形8つ目
        // 上
            16,18,17,   //三角形9つ目
            18,19,17,   //三角形10つ目
        // 下
            20,21,22,   //三角形11つ目
            22,21,23,   //三角形12つ目
    };

    std::copy(std::begin(indices), std::end(indices), indices_.data()); // メンバにコピー
#pragma endregion

#pragma region 頂点配列のnormalへ挿入
    for (int i = 0; i < indices_.size() / 3; ++i) {
        // 三角形1つごとに計算していく
        // 三角形のインデックスを取り出して、一時的な変数に入れる
        uint16_t index0 = indices_[i * 3 + 0];
        uint16_t index1 = indices_[i * 3 + 1];
        uint16_t index2 = indices_[i * 3 + 2];
        // 三角形を構成する頂点座標をベクトルに代入
        DirectX::XMVECTOR p0 = DirectX::XMLoadFloat3(&vertices_[index0].pos_);
        DirectX::XMVECTOR p1 = DirectX::XMLoadFloat3(&vertices_[index1].pos_);
        DirectX::XMVECTOR p2 = DirectX::XMLoadFloat3(&vertices_[index2].pos_);
        // p0->p1ベクトル、p0->p2ベクトルを計算（ベクトルの減算）
        DirectX::XMVECTOR v1 = DirectX::XMVectorSubtract(p1, p0);
        DirectX::XMVECTOR v2 = DirectX::XMVectorSubtract(p2, p0);
        // 外積は両方から垂直なベクトル
        DirectX::XMVECTOR normal_ = DirectX::XMVector3Cross(v1, v2);
        // 正規化（長さを1にする）
        normal_ = DirectX::XMVector3Normalize(normal_);
        // 求めた法線を頂点データに代入
        DirectX::XMStoreFloat3(&vertices_[index0].normal_, normal_);
        DirectX::XMStoreFloat3(&vertices_[index1].normal_, normal_);
        DirectX::XMStoreFloat3(&vertices_[index2].normal_, normal_);
    }
#pragma endregion

#pragma region 頂点バッファの生成
    // 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
    uint32_t sizeVB = static_cast<uint32_t>(sizeof(vertices_[0]) * vertices_.size());

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
#pragma endregion

#pragma region 頂点バッファのマッピング
    // GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
    VertexCube_st* vertMap{ nullptr };
    r = vertBuff_->Map(0, nullptr, (void**)&vertMap);
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
    // 全頂点に対して
    for (int i = 0; i < vertices_.size(); i++)
    {
        vertMap[i] = vertices_[i];//座標をコピー
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

#pragma region 頂点シェーダー
    ComPtr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
    ComPtr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
    ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト
    // 頂点シェーダの読み込みとコンパイル
    r = D3DCompileFromFile(L"BasicVS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vsBlob, &errorBlob);

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
    r = D3DCompileFromFile(L"BasicPS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &psBlob, &errorBlob);

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
    // テクスチャレジスタ0番
    rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	// 種類
    rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
    rootParams[1].DescriptorTable.NumDescriptorRanges = 1;
    rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    // 定数バッファ1番
    rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// 種類
    rootParams[2].Descriptor.ShaderRegister = 1;					// 定数バッファ番号
    rootParams[2].Descriptor.RegisterSpace = 0;						// デフォルト値
    rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// 全てのシェーダから見える
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

    SetColor();

    //SetCBTransform();

#pragma region インデックスバッファの生成
    // インデックスデータ全体のサイズ
    uint32_t sizeIB = static_cast<uint32_t>(sizeof(uint16_t) * indices_.size());

    // リソース設定
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeIB;	// インデックス情報が入る分のサイズ
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // インデックスバッファの生成
    r = iDX->GetDevice()->CreateCommittedResource(
        &heapProp,	// ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &resDesc,	// リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&indexBuff_));
#pragma endregion

#pragma region インデックスバッファのマッピング
    // インデックスバッファをマッピング
    uint16_t* indexMap = nullptr;
    r = indexBuff_->Map(0, nullptr, (void**)&indexMap);
    // 全インデックスに対して
    for (int i = 0; i < indices_.size(); i++) {
        indexMap[i] = indices_[i];	// インデックスをコピー
    }
    // マッピング解除
    indexBuff_->Unmap(0, nullptr);
#pragma endregion

#pragma region インデックスバッファのビュー設定
    // インデックスバッファビューの作成
    ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
    ibView_.Format = DXGI_FORMAT_R16_UINT;
    ibView_.SizeInBytes = sizeIB;
#pragma endregion
}

void Cube::Update(void)
{
    // 各行列の宣言と初期化
    DirectX::XMMATRIX matScale{ DirectX::XMMatrixIdentity() }; // スケール行列
    DirectX::XMMATRIX matRot{ DirectX::XMMatrixIdentity() }; // 回転行列
    DirectX::XMMATRIX matTrans{ DirectX::XMMatrixIdentity() }; // 平行移動行列

    // スケール行列の計算
    matScale = DirectX::XMMatrixScaling(scale_.x, scale_.y, scale_.z);

    // 回転行列の計算
    matRot = DirectX::XMMatrixIdentity();
    matRot *= DirectX::XMMatrixRotationZ(rotation_.z);
    matRot *= DirectX::XMMatrixRotationX(rotation_.x);
    matRot *= DirectX::XMMatrixRotationY(rotation_.y);

    // 平行移動行列の計算
    matTrans = DirectX::XMMatrixTranslation(position_.x, position_.y, position_.z);

    // ワールド行列の合成
    matWorld_ = DirectX::XMMatrixIdentity();    // 変形をリセット
    matWorld_ *= matScale;                      // ワールド行列にスケーリングを反映
    matWorld_ *= matRot;                        // ワールド行列に回転を反映
    matWorld_ *= matTrans;                      // ワールド行列に平行移動を反映

    // 親オブジェクトがあれば
    if (parent_) {
        //親オブジェクトのワールド行列をかける
        matWorld_ *= parent_->matWorld_;
    }

    // 定数バッファへデータ転送
    TransferMatrix();
}

void Cube::Draw(void)
{
    SetCommandsBeforeDraw();

    // インスタンス取得
    InitDirectX* iDX = InitDirectX::GetInstance();

#pragma region 頂点バッファ
    // 頂点バッファの設定
    iDX->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
#pragma endregion

#pragma region インデックスバッファ
    // インデックスバッファの設定
    iDX->GetCommandList()->IASetIndexBuffer(&ibView_);
#pragma endregion

#pragma region ConstBuffer
    // 定数バッファビュー（CBV）の設定コマンド
    iDX->GetCommandList()->SetGraphicsRootConstantBufferView(0, cbMaterial_.GetBuffer()->GetGPUVirtualAddress());
    iDX->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbTransform_.GetBuffer()->GetGPUVirtualAddress());
#pragma endregion

#pragma region SRV
    //指定のヒープにあるSRVをルートパラメータ1番に設定
    iDX->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandleCopy_);
#pragma endregion

    // 描画コマンドリスト
    iDX->GetCommandList()->DrawIndexedInstanced(static_cast<uint32_t>(indices_.size()), 1, 0, 0, 0);
}

void Cube::SetColor(DirectX::XMFLOAT4 rgba)
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

void Cube::TransferMatrix(void)
{
    if (pCamera_) {
        matView_ = pCamera_->GetView();
        matProjection_ = pCamera_->GetProjection();
    }

    cbTransform_.GetConstBuffMap()->mat_ = matWorld_ * matView_ * matProjection_;
}

void Cube::SetCommandsBeforeDraw(void)
{
    // インスタンス取得
    InitDirectX* iDX = InitDirectX::GetInstance();

    // パイプラインステートとルートシグネチャの設定コマンド
    iDX->GetCommandList()->SetPipelineState(pipelineState_.Get());
    iDX->GetCommandList()->SetGraphicsRootSignature(rootSignature_.Get());

    // プリミティブ形状の設定コマンド
    iDX->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

    ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetSRVHeap() };
    // SRVヒープの設定コマンド
    iDX->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}
