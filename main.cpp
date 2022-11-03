#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<d3dcompiler.h>
#include<DirectXMath.h>
#include<math.h>
using namespace DirectX;
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include<vector>
#include<string>
#include<DirectXTex.h>
#include<wrl.h>

#include "Window.h"
#include "Input.h"
#include "InitDirectX.h"
#include "Texture.h"
#include <utility>

using namespace Microsoft::WRL;

// 定数バッファ用データ構造体（マテリアル）
struct ConstBufferDataMaterial {
    XMFLOAT4 color;		// 色（RGBA）
};
// 定数バッファ用データ構造体（3D変換行列）
struct ConstBufferDataTransform {
    XMMATRIX mat;		// 3D変換行列
};

// 3Dオブジェクト型
struct Object3d
{
    // 定数バッファ（行列用）
    ComPtr<ID3D12Resource> constBuffTransform;
    // 定位数バッファマップ（行列用)
    ConstBufferDataTransform* constMapTransform = {};

    // アフィン変換情報
    XMFLOAT3 scale = { 1,1,1 };
    XMFLOAT3 rotation = { 0,0,0 };
    XMFLOAT3 position = { 0,0,0 };
    // ワールド変換行列
    XMMATRIX matWorld = {};
    // 親オブジェクトへのポインタ
    Object3d* parent = nullptr;
};

// 3Dオブジェクト初期化
void InitializeObject3d(Object3d* object, ID3D12Device* device);

void UpdateObject3d(Object3d* object, XMMATRIX& matView, XMMATRIX& matProjection);

void DrawObject3d(Object3d* object, ID3D12GraphicsCommandList* commandList, D3D12_VERTEX_BUFFER_VIEW& vbView, D3D12_INDEX_BUFFER_VIEW& ibView, UINT numIndices);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

#pragma region WindowsAPI初期化

    Window* wnd = Window::GetInstance();

#pragma endregion

#pragma region DirectX初期化
    //DirectX初期化
    HRESULT result;

    InitDirectX* iDX = InitDirectX::GetInstance();
    iDX->Initialize();

#pragma region DirectInput初期化

    KEYS::Initialize();

#pragma endregion

#pragma region srv
    // SRVの最大個数
    const size_t kMaxSRVCount = 2056;

    // デスクリプタヒープの設定
    D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// シェーダから見えるように
    srvHeapDesc.NumDescriptors = kMaxSRVCount;

    // 設定を元にSRV用デスクリプタヒープを作成
    ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;
    result = iDX->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
    assert(SUCCEEDED(result));

    // SRVヒープの先頭ハンドルを取得
    D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
#pragma endregion
#pragma endregion

#pragma region 描画初期化処理

    // 頂点データ構造体
    struct Vertex
    {
        XMFLOAT3 pos;       // xyz座標
        XMFLOAT3 normal;    // 法線ベクトル
        XMFLOAT2 uv;        // uv座標
    };

    // 頂点データ
    // 前
    Vertex vertices[] = {
        {{-5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f}},		// 左下 
        {{-5.0f,  5.0f, -5.0f }, {}, {0.0f, 0.0f}},		// 左上
        {{ 5.0f, -5.0f, -5.0f }, {}, {1.0f, 1.0f}},		// 右下
        {{ 5.0f,  5.0f, -5.0f }, {}, {1.0f, 0.0f}},		// 右上
    // 後
        {{-5.0f, -5.0f,  5.0f }, {}, {0.0f, 1.0f}},		// 左下 
        {{-5.0f,  5.0f,  5.0f }, {}, {0.0f, 0.0f}},		// 左上
        {{ 5.0f, -5.0f,  5.0f }, {}, {1.0f, 1.0f}},		// 右下
        {{ 5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f}},		// 右上
    // 左
        {{-5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f}},		// 左下 
        {{-5.0f, -5.0f,  5.0f }, {}, {0.0f, 0.0f}},		// 左上
        {{-5.0f,  5.0f, -5.0f }, {}, {1.0f, 1.0f}},		// 右下
        {{-5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f}},		// 右上
    // 右
        {{ 5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f}},		// 左下 
        {{ 5.0f, -5.0f,  5.0f }, {}, {0.0f, 0.0f}},		// 左上
        {{ 5.0f,  5.0f, -5.0f }, {}, {1.0f, 1.0f}},		// 右下
        {{ 5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f}},		// 右上
    // 上
        {{-5.0f,  5.0f, -5.0f }, {}, {0.0f, 1.0f}},		// 左下 
        {{ 5.0f,  5.0f, -5.0f }, {}, {0.0f, 0.0f}},		// 左上
        {{-5.0f,  5.0f,  5.0f }, {}, {1.0f, 1.0f}},		// 右下
        {{ 5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f}},		// 右上
    // 下
        {{-5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f}},		// 左下 
        {{ 5.0f, -5.0f, -5.0f }, {}, {0.0f, 0.0f}},		// 左上
        {{-5.0f, -5.0f,  5.0f }, {}, {1.0f, 1.0f}},		// 右下
        {{ 5.0f, -5.0f,  5.0f }, {}, {1.0f, 0.0f}},		// 右上
    };

    // インデックスデータ
    unsigned short indices[] =
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

    for (int i = 0; i < _countof(indices) / 3; i++) {
        // 三角形1つごとに計算していく
        // 三角形のインデックスを取り出して、一時的な変数に入れる
        unsigned short indexZero = indices[i * 3 + 0];
        unsigned short indexOne = indices[i * 3 + 1];
        unsigned short indexTwo = indices[i * 3 + 2];
        // 三角形を構成する頂点座標をベクトルに代入
        XMVECTOR p0 = XMLoadFloat3(&vertices[indexZero].pos);
        XMVECTOR p1 = XMLoadFloat3(&vertices[indexOne].pos);
        XMVECTOR p2 = XMLoadFloat3(&vertices[indexTwo].pos);
        // p0->p1ベクトル、p0->p2ベクトルを計算（ベクトルの減算）
        XMVECTOR v1 = XMVectorSubtract(p1, p0);
        XMVECTOR v2 = XMVectorSubtract(p2, p0);
        // 外積は両方から垂直なベクトル
        XMVECTOR normal = XMVector3Cross(v1, v2);
        // 正規化（長さを1にする）
        normal = XMVector3Normalize(normal);
        // 求めた法線を頂点データに代入
        XMStoreFloat3(&vertices[indexZero].normal, normal);
        XMStoreFloat3(&vertices[indexOne].normal, normal);
        XMStoreFloat3(&vertices[indexTwo].normal, normal);
    }

    // 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
    UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

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

    // 頂点バッファの生成
    ComPtr<ID3D12Resource> vertBuff = nullptr;
    result = iDX->GetDevice()->CreateCommittedResource(
        &heapProp, // ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &resDesc, // リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertBuff));
    assert(SUCCEEDED(result));

    // GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
    Vertex* vertMap = nullptr;
    result = vertBuff->Map(0, nullptr, (void**)&vertMap);
    assert(SUCCEEDED(result));
    for (int i = 0; i < _countof(vertices); i++) {
        vertMap[i] = vertices[i];//座標をコピー
    }
    // 繋がりを解除
    vertBuff->Unmap(0, nullptr);

    // 頂点バッファビューの作成
    D3D12_VERTEX_BUFFER_VIEW vbView{};
    // GPU仮想アドレス
    vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
    // 頂点バッファのサイズ
    vbView.SizeInBytes = sizeVB;
    // 頂点1つ分のデータサイズ
    vbView.StrideInBytes = sizeof(vertices[0]);

#pragma region 頂点シェーダー
    ComPtr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
    ComPtr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
    ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト
    // 頂点シェーダの読み込みとコンパイル
    result = D3DCompileFromFile(
        L"BasicVS.hlsl", // シェーダファイル名
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
        "main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
        0,
        &vsBlob, &errorBlob);

    // エラーなら
    if (FAILED(result)) {
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
    result = D3DCompileFromFile(
        L"BasicPS.hlsl", // シェーダファイル名
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
        "main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
        0,
        &psBlob, &errorBlob);

    // エラーなら
    if (FAILED(result)) {
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

#pragma region グラフィックスパイプライン設定
    // グラフィックスパイプライン設定
    D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

    // シェーダーの設定
    pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
    pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
    pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
    pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

    // サンプルマスクの設定
    pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

    // ラスタライザの設定
    pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;   // 背面をカリング
    pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;  // ポリゴン内塗りつぶし
    pipelineDesc.RasterizerState.DepthClipEnable = true;            // 深度クリッピングを有効に

    // ブレンドステート
    /*pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;*/
    D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
    blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画

    // 共通設定(アルファ値)
    blenddesc.BlendEnable = true;					// ブレンドを有効にする
    blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	// 加算
    blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		// ソースの値を100%使う
    blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	// デストの値を0%使う

    // 加算合成(RGB値)
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;		// 加算
    blenddesc.SrcBlend = D3D12_BLEND_ONE;		// ソースの値を100%使う
    blenddesc.DestBlend = D3D12_BLEND_ONE;		// デストの値を100%使う

    // 減算合成(RGB値)
    blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;		// デストカラソースヲを減算
    blenddesc.SrcBlend = D3D12_BLEND_ONE;					// ソースの値を100%使う
    blenddesc.DestBlend = D3D12_BLEND_ONE;					// デストの値を100%使う

    // 色反転(RGB値)
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;					// 加算
    blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;		// 1.0f - デストカラーの値
    blenddesc.DestBlend = D3D12_BLEND_ZERO;					// 使わない

    // 半透明合成
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;					// 加算
    blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;				// ソースのアルファ値
    blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		// 1.0f - ソースのアルファ値

    // 頂点レイアウトの設定
    pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
    pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

    // 図形の形状設定
    pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    // その他の設定
    pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
    pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
    pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

    // デスクリプタレンジの設定
    D3D12_DESCRIPTOR_RANGE descriptorRange{};
    descriptorRange.NumDescriptors = 1;
    descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    descriptorRange.BaseShaderRegister = 0;	// テクスチャレジスタ0番
    descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

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

    //ルートシグネチャ
    ComPtr<ID3D12RootSignature> rootSignature;

    //ルートシグネチャの設定
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSignatureDesc.pParameters = rootParams;//ルートパラメータの先頭アドレス
    rootSignatureDesc.NumParameters = _countof(rootParams);//ルートパラメータ数
    rootSignatureDesc.pStaticSamplers = &samplerDesc;
    rootSignatureDesc.NumStaticSamplers = 1;

    // ルートシグネチャのシリアライズ
    ID3D10Blob* rootSigBlob = nullptr;
    result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
        &rootSigBlob, &errorBlob);
    assert(SUCCEEDED(result));
    result = iDX->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
        IID_PPV_ARGS(&rootSignature));
    assert(SUCCEEDED(result));
    rootSigBlob->Release();
    // パイプラインにルートシグネチャをセット
    pipelineDesc.pRootSignature = rootSignature.Get();

    // デプスステンシルステートの設定
    pipelineDesc.DepthStencilState.DepthEnable = true;  // 深度テストを行う
    pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;     // 書き込み許可
    pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;          // 小さければ合格
    pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;                                 // 深度値フォーマット
#pragma endregion

#pragma region パイプラインステートの生成
    // パイプランステートの生成
    ComPtr<ID3D12PipelineState> pipelineState = nullptr;
    result = iDX->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
    assert(SUCCEEDED(result));
#pragma endregion

    // 定数バッファ生成用の設定
    // ヒープ設定
    D3D12_HEAP_PROPERTIES cbHeapProp{};
    cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;				// GPUへの転送用
    // リソース設定
    D3D12_RESOURCE_DESC cbResourceDesc{};
    cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256バイトアラインメント
    cbResourceDesc.Height = 1;
    cbResourceDesc.DepthOrArraySize = 1;
    cbResourceDesc.MipLevels = 1;
    cbResourceDesc.SampleDesc.Count = 1;
    cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // 定数バッファの生成準備
    ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
    // 定数バッファの生成
    result = iDX->GetDevice()->CreateCommittedResource(
        &cbHeapProp,	// ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &cbResourceDesc,	// リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&constBuffMaterial));
    assert(SUCCEEDED(result));

    // 定数バッファのマッピング
    ConstBufferDataMaterial* constMapMaterial = nullptr;
    result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);		// マッピング
    assert(SUCCEEDED(result));

    // 値を書き込むと自動的に転送される
    constMapMaterial->color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);		// RGBAで半透明の赤

#pragma region 構造化に伴いコメントアウト
    //// 定数バッファの生成準備0番（3D変換行列）
    //ID3D12Resource* constBuffTransform0 = nullptr;
    //ConstBufferDataTransform* constMapTransform0 = nullptr;
    //// 定数バッファの生成準備1番（3D変換行列）
    //ID3D12Resource* constBuffTransform1 = nullptr;
    //ConstBufferDataTransform* constMapTransform1 = nullptr;


    //float angle = 0.0f;		// カメラの回転角
    //XMFLOAT3 position = { 0.0f,0.0f,0.0f };
    //XMFLOAT3 rotate = { 0.0f,0.0f,0.0f };
    //XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
#pragma endregion 

    // 3Dオブジェクトの数
    const size_t kObjectCount = 50;
    // 3Dオブジェクトの配列
    Object3d object3ds[kObjectCount];

    // 配列内の全オブジェクトに対して
    for (int i = 0; i < _countof(object3ds); i++) {
        // 初期化
        InitializeObject3d(&object3ds[i], iDX->GetDevice());

        // ここからは親子構造のサンプル
        // 先頭以外なら
        if (i > 0) {
            //1つ前のオブジェクトを親オブジェクトとする
            //object3ds[i].parent = &object3ds[i-1];
            // 親オブジェクトの９割の大きさ
            //object3ds[i].scale = { 0.9f,0.9f,0.9f };
            object3ds[i].scale = { 1.0f,1.0f,1.0f };
            // 親オブジェクトに対してZ 軸周りに30度回転
            object3ds[i].rotation = { 0.0f, 0.0f, 30.0f * i };

            // 親オブジェクトに対してZ方向-8.0ずらす
            object3ds[i].position = { 0.0f,0.0f,-8.0f };
        }
    }

    // 射影変換行列（透視投影）
    XMMATRIX matProjection = XMMatrixPerspectiveFovLH(
        XMConvertToRadians(45.0f),				// 上下画角45度
        static_cast<float>(Window::width_ / Window::height_),	// アスペクト比（画面横幅/画面縦幅)
        0.1f, 1000.0f							// 前端,奥端
    );

    // ビュー変換行列（グローバル変数）
    XMMATRIX matView;
    XMFLOAT3 eye(0, 0, -100);		// 視点座標
    XMFLOAT3 target(0, 0, 0);		// 注視点座標
    XMFLOAT3 up(0, 1, 0);			// 上方向ベクトル
    matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
#pragma region 構造化に伴いコメントアウト
    //{
    //    // 定数バッファの生成（設定）
    //    // ヒープ設定
    //    D3D12_HEAP_PROPERTIES cbHeapProp{};
    //    cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;					// GPUへの転送
    //    // リソース設定
    //    D3D12_RESOURCE_DESC cbResourceDesc{};
    //    cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    //    cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;	// 256バイトアラインメント
    //    cbResourceDesc.Height = 1;
    //    cbResourceDesc.DepthOrArraySize = 1;
    //    cbResourceDesc.MipLevels = 1;
    //    cbResourceDesc.SampleDesc.Count = 1;
    //    cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    //    // 定数バッファの生成0番
    //    result = device->CreateCommittedResource(
    //        &cbHeapProp,	// ヒープ設定
    //        D3D12_HEAP_FLAG_NONE,
    //        &cbResourceDesc,	// リソース設定
    //        D3D12_RESOURCE_STATE_GENERIC_READ,
    //        nullptr,
    //        IID_PPV_ARGS(&constBuffTransform0)
    //    );
    //    assert(SUCCEEDED(result));

    //    // 定数バッファのマッピング0番
    //    result = constBuffTransform0->Map(0, nullptr, (void**)&constMapTransform0);	// マッピング
    //    assert(SUCCEEDED(result));

    //    // 単位行列を代入
    //    constMapTransform0->mat = XMMatrixIdentity();


    //    // 定数バッファの生成1番
    //    result = device->CreateCommittedResource(
    //        &cbHeapProp,	// ヒープ設定
    //        D3D12_HEAP_FLAG_NONE,
    //        &cbResourceDesc,	// リソース設定
    //        D3D12_RESOURCE_STATE_GENERIC_READ,
    //        nullptr,
    //        IID_PPV_ARGS(&constBuffTransform1)
    //    );
    //    assert(SUCCEEDED(result));
    //    // 定数バッファのマッピング1番
    //    result = constBuffTransform1->Map(0, nullptr, (void**)&constMapTransform1);	// マッピング
    //    assert(SUCCEEDED(result));

    //    // 単位行列を代入
    //    constMapTransform1->mat = XMMatrixIdentity();
    //}
#pragma endregion

#pragma region テクスチャ読み込み

    Texture::Initialize();
    Texture::Load("Resources/reimu.png");

#pragma endregion

#pragma region キーボード入力設定
    // mInputクラスへ移行
#pragma endregion

    // インデックスデータ全体のサイズ
    UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

    // リソース設定
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeIB;	// インデックス情報が入る分のサイズ
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // インデックスバッファの生成
    ComPtr<ID3D12Resource> indexBuff = nullptr;
    result = iDX->GetDevice()->CreateCommittedResource(
        &heapProp,	// ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &resDesc,	// リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&indexBuff));

    // インデックスバッファをマッピング
    uint16_t* indexMap = nullptr;
    result = indexBuff->Map(0, nullptr, (void**)&indexMap);
    // 全インデックスに対して
    for (int i = 0; i < _countof(indices); i++) {
        indexMap[i] = indices[i];	// インデックスをコピー
    }
    // マッピング解除
    indexBuff->Unmap(0, nullptr);

    // インデックスバッファビューの作成
    D3D12_INDEX_BUFFER_VIEW ibView{};
    ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
    ibView.Format = DXGI_FORMAT_R16_UINT;
    ibView.SizeInBytes = sizeIB;

#pragma endregion


    // ゲームループ
    while (true) {

#pragma region ウィンドウメッセージ処理

        if (!wnd->GetKeepWindow()) {
            break;
        }

#pragma endregion

#pragma region DirextX毎フレーム処理
        // DirectX毎フレーム処理　ここから

#pragma region キーボード情報の取得

        KEYS::Update();

#pragma endregion

        matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
#pragma region 構造化に伴いコメントアウト
        //if (keys[DIK_D] || keys[DIK_A]) {
        //    if (keys[DIK_D]) { angle += XMConvertToRadians(1.0f); }
        //    else if (keys[DIK_A]) { angle -= XMConvertToRadians(1.0f); }

        //    // angleラジアンだけY軸周りに回転。半径は-100
        //    eye.x = -100 * sinf(angle);
        //    eye.z = -100 * cosf(angle);
        //    // ビュー変換行列
        //    matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
        //}
        //if (keys[DIK_W] || keys[DIK_S]) {
        //    if (keys[DIK_W]) { angle += XMConvertToRadians(10.0f); }
        //    else if (keys[DIK_S]) { angle -= XMConvertToRadians(10.0f); }

        //    // angleラジアンだけY軸周りに回転。 半径は -100
        //    eye.y = -100 * sinf(angle);
        //    eye.z = -100 * cosf(angle);

        //    matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
        //}

        //// いずれかのキーを押していたら
        //if (keys[DIK_UP] || keys[DIK_DOWN] || keys[DIK_RIGHT] || keys[DIK_LEFT]) {
        //    // 座標を変更する処理（Z座標）
        //    if (keys[DIK_UP]) { position.z += 1.0f; }
        //    else if (keys[DIK_DOWN]) { position.z -= 1.0f; }
        //    if (keys[DIK_RIGHT]) { position.x += 1.0f; }
        //    else if (keys[DIK_LEFT]) { position.x -= 1.0f; }
        //}

        //// ワールド変換行列
        //XMMATRIX matWorld0;
        //matWorld0 = XMMatrixIdentity();
        //// スケーリング行列
        //XMMATRIX matScale0;
        //matScale0 = XMMatrixScaling(scale.x, scale.y, scale.z);
        //// 回転行列
        //XMMATRIX matRot0;
        //matRot0 = XMMatrixIdentity();//単位行列を代入
        //matRot0 *= XMMatrixRotationZ(XM_PI / 4.0f);	// Z軸周りに45度回転
        //// 平行移動行列
        //XMMATRIX matTrans0;
        //matTrans0 = XMMatrixTranslation(position.x, position.y, position.z);
        //// ワールド行列を合成
        //matWorld0 = matScale0 * matRot0 * matTrans0;
        //// ワールド、ビュー、射影行列を合成してシェーダに転送
        //constMapTransform0->mat = matWorld0 * matView * matProjection;


        //// ワールド変換行列
        //XMMATRIX matWorld1;
        //matWorld1 = XMMatrixIdentity();
        //// スケーリング行列
        //XMMATRIX matScale1;
        //matScale1 = XMMatrixScaling(1.0f, 1.0f, 1.0f);
        ////回転行列
        //XMMATRIX matRot1;
        //matRot1 = XMMatrixIdentity(); //単位行列を代入
        //matRot1 *= XMMatrixRotationY(XM_PI / 4.0f);	// Y軸周りに45度回転
        //// 平行移動行列
        //XMMATRIX matTrans1;
        //matTrans1 = XMMatrixTranslation(-20, 0, 0);
        //// ワールド行列を合成
        //matWorld1 = matScale1 * matRot1 * matTrans1;
        //// ワールド、ビュー、射影行列を合成してシェーダに転送
        //constMapTransform1->mat = matWorld1 * matView * matProjection;
#pragma endregion

        if (KEYS::IsDown(DIK_W) || KEYS::IsDown(DIK_S) || KEYS::IsDown(DIK_A) || KEYS::IsDown(DIK_D)) {
            if (KEYS::IsDown(DIK_W)) { object3ds[0].position.y += 1.0f; }
            else if (KEYS::IsDown(DIK_S)) { object3ds[0].position.y -= 1.0f; }
            if (KEYS::IsDown(DIK_A)) { object3ds[0].position.x -= 1.0f; }
            else if (KEYS::IsDown(DIK_D)) { object3ds[0].position.x += 1.0f; }
        }

        for (size_t i = 0; i < _countof(object3ds); i++) {
            UpdateObject3d(&object3ds[i], matView, matProjection);
        }

        // DirectX毎フレーム処理　ここまで
#pragma endregion

#pragma region 描画開始

        iDX->PreDraw();

#pragma endregion

        // パイプラインステートとルートシグネチャの設定コマンド
        iDX->GetCommandList()->SetPipelineState(pipelineState.Get());
        iDX->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

        // プリミティブ形状の設定コマンド
        iDX->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

        // 頂点バッファビューの設定コマンド
        iDX->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

        // インデックスバッファビューの設定コマンド
        iDX->GetCommandList()->IASetIndexBuffer(&ibView);


        iDX->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
        // SRVヒープの設定コマンド
        // // デスクリプタヒープの配列
        ID3D12DescriptorHeap* ppHeaps[] = { srvHeap.Get() };
        iDX->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
        //commandList->SetDescriptorHeaps(1, &srvHeap);



        iDX->GetCommandList()->SetGraphicsRootDescriptorTable(1, Texture::GetTexSRVGpuH("Resources/reimu.png"));

        // 全オブジェクトについて処理
        for (int i = 0; i < _countof(object3ds); i++) {
            DrawObject3d(&object3ds[i], iDX->GetCommandList(), vbView, ibView, _countof(indices));
        }

#pragma region 構造化に伴いコメントアウト
        //// 0番定数バッファビュー(CBV)の設定コマンド
        //commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform0->GetGPUVirtualAddress());

        //// 描画コマンド
        //commandList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0); // 全ての頂点を使って描画

        //// 1番定数バッファビュー(CBV)の設定コマンド
        //commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform1->GetGPUVirtualAddress());

        //// 描画コマンド
        //commandList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0); // 全ての頂点を使って描画
#pragma endregion

        // ４．描画コマンドここまで

#pragma region 描画終了

        iDX->PostDraw();

#pragma endregion
    }

    // ウィンドウクラスを登録解除
    wnd->DelWindow();

    return 0;
}

void InitializeObject3d(Object3d* object, ID3D12Device* device)
{
    HRESULT result;

    // 定数バッファのヒープ設定
    D3D12_HEAP_PROPERTIES heapProp{};
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
    // 定数バッファのリソース設定
    D3D12_RESOURCE_DESC resDesc{};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // 定数バッファの生成
    result = device->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&object->constBuffTransform)
    );
    assert(SUCCEEDED(result));

    // 定数バッファのマッピング
    result = object->constBuffTransform->Map(0, nullptr, (void**)&object->constMapTransform);
    assert(SUCCEEDED(result));
}

void UpdateObject3d(Object3d* object, XMMATRIX& matView, XMMATRIX& matProjection)
{
    XMMATRIX matScale, matRot, matTrans;

    // スケール、回転、平行移動行列の計算
    matScale = XMMatrixScaling(object->scale.x, object->scale.y, object->scale.z);
    matRot = XMMatrixIdentity();
    matRot *= XMMatrixRotationZ(object->rotation.z);
    matRot *= XMMatrixRotationX(object->rotation.x);
    matRot *= XMMatrixRotationY(object->rotation.y);
    matTrans = XMMatrixTranslation(
        object->position.x, object->position.y, object->position.z
    );

    // ワールド行列の合成
    object->matWorld = XMMatrixIdentity();  // 変形をリセット
    object->matWorld *= matScale;           // ワールド行列にスケーリングを反映
    object->matWorld *= matRot;             // ワールド行列に回転を反映
    object->matWorld *= matTrans;           // ワールド行列に平行移動を反映

    // 親オブジェクトがあれば
    if (object->parent != nullptr) {
        //親オブジェクトのワールド行列をかける
        object->matWorld *= object->parent->matWorld;
    }

    // 定数バッファへデータ転送
    object->constMapTransform->mat = object->matWorld * matView * matProjection;
}

void DrawObject3d(Object3d* object, ID3D12GraphicsCommandList* commandList, D3D12_VERTEX_BUFFER_VIEW& vbView, D3D12_INDEX_BUFFER_VIEW& ibView, UINT numIndices)
{
    // 頂点バッファの設定
    commandList->IASetVertexBuffers(0, 1, &vbView);
    // インデックスバッファの設定
    commandList->IASetIndexBuffer(&ibView);
    // 定数バッファビュー（CBV）の設定コマンド
    commandList->SetGraphicsRootConstantBufferView(2, object->constBuffTransform->GetGPUVirtualAddress());

    // 描画コマンドリスト
    commandList->DrawIndexedInstanced(numIndices, 1, 0, 0, 0);
}