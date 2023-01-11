#include "Texture.h"
#include <cassert>
#include "Util.h"
#include <DirectXTex.h>
#include "InitDirectX.h"

// 静的変数の実体生成
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> Texture::srvHeap_ = nullptr;
std::map<Texture::TEXTURE_KEY, Texture::TEXTURE_VALUE> Texture::textures_{};
std::map<Texture::TEX_KEY_ID, Texture::TEXTURE_KEY> Texture::mapKeys_{};
uint32_t Texture::indexNextDescHeap_{ 0 };

void Texture::Initialize(void)
{
    InitDirectX* iDX_ = InitDirectX::GetInstance();

    // デスクリプタヒープの設定
    D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
    srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// シェーダから見えるように
    srvDescHeapDesc.NumDescriptors = maxSRVDesc_;

    // 設定を元にSRV用デスクリプタヒープを作成
    HRESULT r = iDX_->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&srvHeap_));
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG

    GenerateMissingTexture();
}

void Texture::Load(const fsPath& pathAndFileName)
{
#pragma region いろいろ確認
    // srvDescの空き確認
    assert(indexNextDescHeap_ < maxSRVDesc_);
    uint32_t handle{ indexNextDescHeap_ };

    Texture tmp{}; // 一時obj

    tmp.SetMapKey(pathAndFileName);

    // 既に読み込んだテクスチャとの重複確認。
    if (textures_.count(tmp.name_)) {
        // 重複があった場合は読み込みはしない。
        OutputDebugString(L"WARNING: An image with the same name was loaded.");
        return;
    }
    else {
        // 重複がなかった場合は次のテクスチャのためにハンドルの指標を1進める。
        indexNextDescHeap_++;
    }
#pragma endregion

#pragma region テクスチャのロード
    //画像イメージデータ配列
    DirectX::TexMetadata metadata{};
    DirectX::ScratchImage scratchImg{};

    // WICテクスチャのロードに使う pathを文字列変換
    std::string path{ tmp.name_.string() };
    std::wstring wPath{ path.begin(),path.end() };
    const wchar_t* szFile{ wPath.c_str() };

    // WICテクスチャのロード
    HRESULT r = LoadFromWICFile(
        szFile,   //「Resources」フォルダの「texture.png」
        DirectX::WIC_FLAGS_NONE,
        &metadata, scratchImg);
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

#pragma region ミップマップ
    DirectX::ScratchImage mipChain{};

    // ミップマップ生成
    r = GenerateMipMaps(scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(), DirectX::TEX_FILTER_DEFAULT, 0, mipChain);
    if (SUCCEEDED(r)) {
        scratchImg = std::move(mipChain);
        metadata = scratchImg.GetMetadata();
    }
#pragma endregion

    // 読み込んだディフューズテクスチャをSRGBとして扱う
    metadata.format = DirectX::MakeSRGB(metadata.format);

#pragma region ヒープ設定とデスクリプタ設定
    // ヒープ設定
    D3D12_HEAP_PROPERTIES texHeapProp{};
    texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
    texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

    // リソース設定
    D3D12_RESOURCE_DESC textureResourceDesc{};
    textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    textureResourceDesc.Format = metadata.format;
    textureResourceDesc.Width = metadata.width;
    textureResourceDesc.Height = static_cast<UINT>(metadata.height);
    textureResourceDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
    textureResourceDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
    textureResourceDesc.SampleDesc.Count = 1;
#pragma endregion

    // InitDirectXのインスタンス取得
    InitDirectX* iDX = InitDirectX::GetInstance();

#pragma region テクスチャバッファ
    // テクスチャバッファの生成
    r = iDX->GetDevice()->CreateCommittedResource(
        &texHeapProp,
        D3D12_HEAP_FLAG_NONE,
        &textureResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&tmp.info_.buff_)); // MAP_VALUEのbuff_へ書き込み
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

#pragma region srvCpuHandleの保存とバッファへのデータ転送
    // デスクリプタのサイズを取得する
    uint32_t incrementSize = iDX->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    // MAP_VALUEのsrvCPUHandle_へ書き込み
    tmp.info_.srvCpuHandle_ = srvHeap_.Get()->GetCPUDescriptorHandleForHeapStart(); // Descのヒープ領域のスタート位置を取得
    tmp.info_.srvCpuHandle_.ptr += (size_t)incrementSize * (size_t)handle;

    // 全ミップマップについて
    for (size_t i = 0; i < metadata.mipLevels; i++) {
        // ミップマップレベルを指定してイメージを取得
        const DirectX::Image* img = scratchImg.GetImage(i, 0, 0);

        // テクスチャバッファにデータ転送
        r = tmp.info_.buff_-> // MAP_VALUEのbuff_
            WriteToSubresource(
                static_cast<UINT>(i),
                nullptr,		// 全領域へコピー
                img->pixels,	// 元データアドレス
                static_cast<UINT>(img->rowPitch),	// 1ラインサイズ
                static_cast<UINT>(img->slicePitch)	// 全サイズ
            );
#ifdef _DEBUG
        assert(SUCCEEDED(r));
#endif // _DEBUG
    }
#pragma endregion

#pragma region SRVの設定と生成
    // シェーダリソースビュー設定
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = textureResourceDesc.Format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

    // ハンドルのさす位置にシェーダーリソースビューの作成
    iDX->GetDevice()->CreateShaderResourceView(tmp.info_.buff_.Get(), &srvDesc, tmp.info_.srvCpuHandle_);
#pragma endregion

#pragma region srvGpuHandleの保存とmapに紐づけて保存
    // MAP_VALUEのsrvGPUHandle_へ書き込み
    tmp.info_.srvGpuHandle_ = srvHeap_.Get()->GetGPUDescriptorHandleForHeapStart(); // Descのヒープ領域のスタート位置を取得
    // ハンドルを進める
    tmp.info_.srvGpuHandle_.ptr += (size_t)incrementSize * (size_t)handle;

    // mapへの挿入
    textures_.insert({ tmp.name_, tmp.info_ });
#pragma endregion
}

void Texture::Load(const fsPath& pathAndFileName, const std::string& id)
{
    // 既に読み込んだテクスチャとの重複確認。
    if (textures_.count(pathAndFileName)) {
        // 重複があった場合は読み込みはしない。
        OutputDebugString(L"WARNING: An image with the same name was loaded.");
        return;
    }
    else {
        // 重複がない場合読み込み
        Load(pathAndFileName);
    }

    // 既に設定したidとの重複確認
    if (mapKeys_.count(id)) {
        try {
            // 重複がある場合例外スロー
            throw std::logic_error("ERROR: The same \"registerId\"(TEX_KEY_ID) is already in use.");
        }
        catch (const std::logic_error&) {
            // 強制終了。
            std::exit(1);
        }
    }
    // MAP_KEYとの紐付け
    mapKeys_.insert({ id, pathAndFileName });
}

const bool Texture::ExistTexture(const std::string& path)
{
    return textures_.count(path);
}

void Texture::SetId(const fsPath& pathAndFileName, const std::string& id)
{
    // 既に読み込んでいるテクスチャの中に一致するものがあるか確認
    if (textures_.count(pathAndFileName)) {
        // ある場合、IDの重複を確認
        if (mapKeys_.count(id)) {
            // IDもある場合
            try {
                // 例外スロー
                throw std::logic_error("ERROR: The same \"registerId\"(TEX_KEY_ID) is already in use.");
            }
            catch (const std::logic_error&) {
                // 強制終了。
                std::exit(1);
            }
        }
        else {
            // IDを割り当てるテクスチャが存在している + IDが被っていない場合
            mapKeys_.insert({ id, pathAndFileName });
        }
    }
    else {
        try {
            // ない場合、例外を投げる
            throw std::logic_error("Specified TEXTURE_KEY does not exist.");
        }
        catch (const std::logic_error&) {
            // 強制終了。
            std::exit(1);
        }
    }
}

const Texture::TEXTURE_KEY* Texture::GetTextureKey(const std::string& id)
{
    return &mapKeys_.at(id);
}

void Texture::Eject(const fsPath& pathAndFileName)
{
    //textures_.erase(pathAndFileName);
}

void Texture::DeleteId(const std::string& id)
{
    // idが存在するか
    if (!mapKeys_.count(id)) {
        try {
            // 存在しない場合例外スロー
            throw std::logic_error("ERROR: Specified \"id\"(TEX_KEY_ID) does not exist.");
        }
        catch (const std::logic_error&) {
            // 強制終了。
            std::exit(1);
        }
    }
    else {
        // 存在する場合削除
        mapKeys_.erase(id);
    }
}

void Texture::GenerateMissingTexture(void)
{
#pragma region エラー画像の生成と登録
#pragma region missingTexture生成
    uint32_t handle{ indexNextDescHeap_ };

    Texture tmp{}; // てんぽらりん

    tmp.SetMapKey("ERROR_IMAGE");
    indexNextDescHeap_++;

    // 一辺のピクセル数
    constexpr size_t imageLength{ 256 };
    // 配列の要素数
    constexpr size_t imageDataCount{ imageLength * imageLength };

    // イメージデータ配列
    DirectX::XMFLOAT4* imageData{ new DirectX::XMFLOAT4[imageDataCount] };

    // 生成
    for (size_t i = 0; i < imageDataCount; i++) {
        if (i < 32513) {
            if (i % 256 < 128) {
                imageData[i] = { 0.0f, 0.0f, 0.0f, 1.0f };
            }
            else if (i % 256 >= 128) {
                imageData[i] = { 1.0f, 0.0f, 1.0f, 1.0f };
            }
        }
        else {
            if (i % 256 < 128) {
                imageData[i] = { 1.0f, 0.0f, 1.0f, 1.0f };
            }
            else if (i % 256 >= 128) {
                imageData[i] = { 0.0f, 0.0f, 0.0f, 1.0f };
            }
        }
    }

#pragma endregion
#pragma region ヒープ設定とデスクリプタ設定
    // ヒープ設定
    D3D12_HEAP_PROPERTIES texHeapProp{};
    texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
    texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

    // リソース設定
    D3D12_RESOURCE_DESC textureResourceDesc{};
    textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    textureResourceDesc.Width = imageLength;
    textureResourceDesc.Height = imageLength;
    textureResourceDesc.DepthOrArraySize = 1;
    textureResourceDesc.MipLevels = 1;
    textureResourceDesc.SampleDesc.Count = 1;
#pragma endregion

    // InitDirectXのインスタンス取得
    InitDirectX* iDX = InitDirectX::GetInstance();

#pragma region テクスチャバッファ
    // テクスチャバッファの生成
    HRESULT r = iDX->GetDevice()->CreateCommittedResource(
        &texHeapProp,
        D3D12_HEAP_FLAG_NONE,
        &textureResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&tmp.info_.buff_)); // MAP_VALUEのbuff_へ書き込み
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

#pragma region srvCpuHandleの保存とバッファへのデータ転送
    // デスクリプタのサイズを取得する
    uint32_t incrementSize = iDX->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    // MAP_VALUEのsrvCPUHandle_へ書き込み
    tmp.info_.srvCpuHandle_ = srvHeap_.Get()->GetCPUDescriptorHandleForHeapStart(); // Descのヒープ領域のスタート位置を取得
    tmp.info_.srvCpuHandle_.ptr += (size_t)incrementSize * (size_t)handle;

    // テクスチャバッファにデータ転送
    r = tmp.info_.buff_-> // MAP_VALUEのbuff_
        WriteToSubresource(
            0,
            nullptr,		// 全領域へコピー
            imageData,	// 元データアドレス
            sizeof(DirectX::XMFLOAT4) * imageLength,	// 1ラインサイズ
            sizeof(DirectX::XMFLOAT4) * imageDataCount// 全サイズ
        );

    //イメージデータ解放
    delete[] imageData;

#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

#pragma region SRVの設定と生成
    // シェーダリソースビュー設定
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = textureResourceDesc.Format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

    // ハンドルのさす位置にシェーダーリソースビューの作成
    iDX->GetDevice()->CreateShaderResourceView(tmp.info_.buff_.Get(), &srvDesc, tmp.info_.srvCpuHandle_);
#pragma endregion

#pragma region srvGpuHandleの保存とmapに紐づけて保存
    // MAP_VALUEのsrvGPUHandle_へ書き込み
    tmp.info_.srvGpuHandle_ = srvHeap_.Get()->GetGPUDescriptorHandleForHeapStart(); // Descのヒープ領域のスタート位置を取得
    // ハンドルを進める
    tmp.info_.srvGpuHandle_.ptr += (size_t)incrementSize * (size_t)handle;

    // mapへの挿入
    textures_.insert({ tmp.name_, tmp.info_ });
#pragma endregion
#pragma endregion
}

const Texture::TEXTURE_VALUE* Texture::GetTextureInfo(const fsPath& pathAndFileName)
{
    return &textures_.at(pathAndFileName);
}

const Texture::TEXTURE_VALUE* Texture::GetTextureInfoById(const std::string& id)
{
    return &textures_.at(*GetTextureKey(id));
}

void Texture::SetMapKey(const fsPath& keyName)
{
    name_ = keyName;
}