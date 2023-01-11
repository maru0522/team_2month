#include "Sprite.h"
#include "InitDirectX.h"
#include <string>
#include "Texture.h"
#include "Window.h"
#include "Util.h"
#include "GraphicsPipeline.h"
#include<d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

// 静的変数の実体
GraphicsPipeline* Sprite::graphicsPipeline_{ GraphicsPipeline::GetGraphicsPipeLine2d() };

void Sprite::PreDraw(void)
{
    InitDirectX* iDX = InitDirectX::GetInstance();

# pragma region 共通
    // パイプラインステートとルートシグネチャの設定コマンド
    iDX->GetCommandList()->SetPipelineState(graphicsPipeline_->GetPipelineState());
    iDX->GetCommandList()->SetGraphicsRootSignature(graphicsPipeline_->GetRootSignature());
    // プリミティブ形状の設定コマンド
    iDX->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 三角形リスト

    ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetSRVHeap() };
    // SRVヒープの設定コマンド
    iDX->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
#pragma endregion
}

Sprite::Sprite(const fsPath& relativePath, const fsPath& fileName)
{
    std::string path{ fsPath{relativePath / fileName}.string() };

    try {
        if (Texture::ExistTexture(path)) {// テクスチャが存在している場合
            srvGpuHandleCopy_ = Texture::GetTextureInfo(path)->srvGpuHandle_;
            ConstructorCommon();
            SetOriginalSize(path, CMode::PATH);
            AdjustTextureSize();
        }
        else {// テクスチャが存在していない場合エラーを送出
            throw std::logic_error("Error: Specified Texture does not exist.");
        }
    }
    catch (const std::logic_error&) {
        srvGpuHandleCopy_ = Texture::GetTextureInfo("ERROR_IMAGE")->srvGpuHandle_;
        ConstructorCommon();
        SetOriginalSize("ERROR_IMAGE", CMode::PATH);
        AdjustTextureSize();
    }
}

Sprite::Sprite(const fsPath& pathAndFileName_or_Id, CMode mode)
{
    std::string path;
    if (static_cast<bool>(mode)) {// CMode == ID
        path = Texture::GetTextureKey(pathAndFileName_or_Id.string())->string();
    }
    else {// CMode == PATH
        path = pathAndFileName_or_Id.string();
    }

    try {
        if (Texture::ExistTexture(path)) {// テクスチャが存在している場合
            if (static_cast<int>(mode)) {// CModeがIDの場合
                srvGpuHandleCopy_ = Texture::GetTextureInfoById(pathAndFileName_or_Id.string())->srvGpuHandle_;
            }
            else {// CModeがPATHの場合
                srvGpuHandleCopy_ = Texture::GetTextureInfo(pathAndFileName_or_Id)->srvGpuHandle_;
            }
            ConstructorCommon();
            SetOriginalSize(pathAndFileName_or_Id, mode);
            AdjustTextureSize();
        }
        else {// テクスチャが存在していない場合エラーを送出
            throw std::logic_error("Error: Specified Texture does not exist.");
        }
    }
    catch (const std::logic_error&) {
        srvGpuHandleCopy_ = Texture::GetTextureInfo("ERROR_IMAGE")->srvGpuHandle_;
        ConstructorCommon();
        SetOriginalSize("ERROR_IMAGE", mode);
        AdjustTextureSize();
    }
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

    if (!cutLength_.x) cutLength_.x = cutEndPoint_.x - cutStartPoint_.x;
    if (!cutLength_.y) cutLength_.y = cutEndPoint_.y - cutStartPoint_.y;

    float_t tex_left = cutStartPoint_.x / originalSize_.x;
    float_t tex_rigit = (cutStartPoint_.x + cutLength_.x) / originalSize_.x;
    float_t tex_top = cutStartPoint_.y / originalSize_.y;
    float_t tex_bottom = (cutStartPoint_.y + cutLength_.y) / originalSize_.y;

    vertices_.at(static_cast<int>(VertNum::LeftBottom)).uv_ = { tex_left,tex_bottom };
    vertices_.at(static_cast<int>(VertNum::LeftTop)).uv_ = { tex_left, tex_top };
    vertices_.at(static_cast<int>(VertNum::RightBottom)).uv_ = { tex_rigit,tex_bottom };
    vertices_.at(static_cast<int>(VertNum::RightTop)).uv_ = { tex_rigit,tex_top };

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

void Sprite::ConstructorCommon(void)
{
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

#pragma region cbTransform
    SetCBTransform();
#pragma endregion

    SetColor({ 1.0f,1.0f,1.0f,1.0f });
}

void Sprite::AdjustTextureSize(void)
{
    cutLength_ = originalSize_;
    size_ = cutLength_;
}

void Sprite::SetOriginalSize(const fsPath& path, CMode mode)
{
    if (!static_cast<int>(mode)) {
        // mode == PATH
        originalSize_.x = (float_t)Texture::GetTextureInfo(path)->buff_->GetDesc().Width;
        originalSize_.y = (float_t)Texture::GetTextureInfo(path)->buff_->GetDesc().Height;
    }
    else {
        originalSize_.x = (float_t)Texture::GetTextureInfoById(path.string())->buff_->GetDesc().Width;
        originalSize_.y = (float_t)Texture::GetTextureInfoById(path.string())->buff_->GetDesc().Height;
    }
}