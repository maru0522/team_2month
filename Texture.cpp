#include "Texture.h"
#include <cassert>
#include "Util.h"
#include <DirectXTex.h>
#include "InitDirectX.h"

// 静的変数の実体生成
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> Texture::srvHeap_ = nullptr;
std::map<Texture::MAP_KEY, Texture::MAP_VALUE> Texture::textures_{};
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
}

void Texture::Load(const std::string& relativePath, const std::string& fileName)
{
    // srvDescの空き確認
    assert(indexNextDescHeap_ < maxSRVDesc_);
    uint32_t handle{ indexNextDescHeap_ };

    Texture tmp{}; // 一時obj

    // relativePathの末尾に"/"があるか確認
    if (!std::string{ relativePath.back() }.compare("/")) { // 一致している場合0を返すので!で非0としている。
        // ある場合、そのまま名前を設定
        tmp.SetMapKey(relativePath + fileName);
    }
    else {
        // がない場合、"/"を加えて名前を設定
        tmp.SetMapKey(relativePath + "/" + fileName);
    }

    // 既に読み込んだテクスチャとの重複確認。
    if (textures_.count(tmp.name_)) {
        // 重複があった場合イテレータを返す。
        decltype(textures_)::iterator it{ textures_.find(tmp.name_) };

        // イテレータからハンドルを取得する
        handle = static_cast<uint32_t>(std::distance(textures_.begin(), it));
    }
    else {
        // 重複がなかった場合は次のテクスチャのためにハンドルの指標を1進める。
        indexNextDescHeap_++;
    }

    //画像イメージデータ配列
    DirectX::TexMetadata metadata{};
    DirectX::ScratchImage scratchImg{};

    // WICテクスチャのロードに使う pathを文字列変換
    std::wstring wPath{ tmp.name_.begin(),tmp.name_.end() };
    const wchar_t* szFile{ wPath.c_str() };

    // WICテクスチャのロード
    HRESULT r = LoadFromWICFile(
        szFile,   //「Resources」フォルダの「texture.png」
        DirectX::WIC_FLAGS_NONE,
        &metadata, scratchImg);
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG

    DirectX::ScratchImage mipChain{};

    // ミップマップ生成
    r = GenerateMipMaps(scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(), DirectX::TEX_FILTER_DEFAULT, 0, mipChain);
    if (SUCCEEDED(r)) {
        scratchImg = std::move(mipChain);
        metadata = scratchImg.GetMetadata();
    }

    // 読み込んだディフューズテクスチャをSRGBとして扱う
    metadata.format = DirectX::MakeSRGB(metadata.format);

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

    // InitDirectXのインスタンス取得
    InitDirectX* iDX = InitDirectX::GetInstance();

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

    // 要確認: static_castを外すと警告↓
    // デスクリプタのサイズを取得する
    uint32_t incrementSize = iDX->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    // MAP_VALUEのsrvCPUHandle_へ書き込み
    tmp.info_.srvCpuHandle_ = srvHeap_.Get()->GetCPUDescriptorHandleForHeapStart(); // Descのヒープ領域のスタート位置を取得
    tmp.info_.srvCpuHandle_.ptr += static_cast<size_t>(incrementSize) * static_cast<size_t>(handle);

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

    // シェーダリソースビュー設定
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = textureResourceDesc.Format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

    // ハンドルのさす位置にシェーダーリソースビューの作成
    iDX->GetDevice()->CreateShaderResourceView(tmp.info_.buff_.Get(), &srvDesc, tmp.info_.srvCpuHandle_);
    // MAP_VALUEのsrvGPUHandle_へ書き込み
    tmp.info_.srvGpuHandle_ = srvHeap_.Get()->GetGPUDescriptorHandleForHeapStart(); // Descのヒープ領域のスタート位置を取得
    // ハンドルを進める
    tmp.info_.srvGpuHandle_.ptr += static_cast<size_t>(incrementSize) * static_cast<size_t>(handle);

    // mapへの挿入（or 代入）※代入の場合、同一KEYに対してVALUEが上書きされるため注意
    textures_.insert_or_assign(tmp.name_, tmp.info_); // 代入時であっても全く同じVALUEが入るとは思われる。
}

void Texture::Load(const std::string& pathAndFileName)
{
    // srvDescの空き確認
    assert(indexNextDescHeap_ < maxSRVDesc_);
    uint32_t handle{ indexNextDescHeap_ };

    Texture tmp{}; // 一時obj

    // 名前が1つなので判定はせずそのまま設定
    tmp.SetMapKey(pathAndFileName);

    // 既に読み込んだテクスチャとの重複確認。
    if (textures_.count(tmp.name_)) {
        // 重複があった場合イテレータを返す。
        decltype(textures_)::iterator it{ textures_.find(tmp.name_) };

        // イテレータからハンドルを取得する
        handle = static_cast<uint32_t>(std::distance(textures_.begin(), it));
    }
    else {
        // 重複がなかった場合は次のテクスチャのためにハンドルの指標を1進める。
        indexNextDescHeap_++;
    }

    //画像イメージデータ配列
    DirectX::TexMetadata metadata{};
    DirectX::ScratchImage scratchImg{};

    // WICテクスチャのロードに使う pathを文字列変換
    std::wstring wPath{ tmp.name_.begin(),tmp.name_.end() };
    const wchar_t* szFile{ wPath.c_str() };

    // WICテクスチャのロード
    HRESULT r = LoadFromWICFile(
        szFile,   //「Resources」フォルダの「texture.png」
        DirectX::WIC_FLAGS_NONE,
        &metadata, scratchImg);
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG

    DirectX::ScratchImage mipChain{};

    // ミップマップ生成
    r = GenerateMipMaps(scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(), DirectX::TEX_FILTER_DEFAULT, 0, mipChain);
    if (SUCCEEDED(r)) {
        scratchImg = std::move(mipChain);
        metadata = scratchImg.GetMetadata();
    }

    // 読み込んだディフューズテクスチャをSRGBとして扱う
    metadata.format = DirectX::MakeSRGB(metadata.format);

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

    // InitDirectXのインスタンス取得
    InitDirectX* iDX = InitDirectX::GetInstance();

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

    // 要確認: static_castを外すと警告↓
    // デスクリプタのサイズを取得する
    uint32_t incrementSize = iDX->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    // MAP_VALUEのsrvCPUHandle_へ書き込み
    tmp.info_.srvCpuHandle_ = srvHeap_.Get()->GetCPUDescriptorHandleForHeapStart(); // Descのヒープ領域のスタート位置を取得
    tmp.info_.srvCpuHandle_.ptr += static_cast<size_t>(incrementSize) * static_cast<size_t>(handle);

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

    // シェーダリソースビュー設定
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = textureResourceDesc.Format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

    // ハンドルのさす位置にシェーダーリソースビューの作成
    iDX->GetDevice()->CreateShaderResourceView(tmp.info_.buff_.Get(), &srvDesc, tmp.info_.srvCpuHandle_);
    // MAP_VALUEのsrvGPUHandle_へ書き込み
    tmp.info_.srvGpuHandle_ = srvHeap_.Get()->GetGPUDescriptorHandleForHeapStart(); // Descのヒープ領域のスタート位置を取得
    // ハンドルを進める
    tmp.info_.srvGpuHandle_.ptr += static_cast<size_t>(incrementSize) * static_cast<size_t>(handle);

    // mapへの挿入（or 代入）※代入の場合、同一KEYに対してVALUEが上書きされるため注意
    textures_.insert_or_assign(tmp.name_, tmp.info_); // 代入時であっても全く同じVALUEが入るとは思われる。
}

const Texture Texture::GetTexture(const std::string& relativePath, const std::string& fileName)
{
    Texture tmp{}; // 一時obj

    // relativePathの末尾に"/"があるか確認
    if (!std::string{ relativePath.back() }.compare("/")) { // 一致している場合0を返すので!で非0としている。
        // ある場合、そのまま名前を設定
        tmp.SetMapKey(relativePath + fileName);
    }
    else {
        // がない場合、"/"を加えて名前を設定
        tmp.SetMapKey(relativePath + "/" + fileName);
    }
    tmp.info_ = textures_.at(tmp.name_);

    return tmp;
}

const Texture Texture::GetTexture(const std::string& pathAndFileName)
{
    Texture tmp{}; // 一時obj

    tmp.SetMapKey(pathAndFileName);
    tmp.info_ = textures_.at(tmp.name_);

    return tmp;
}

const Texture::MAP_VALUE Texture::GetTextureInfo(const std::string& relativePath, const std::string& fileName)
{
    std::string name{};

    // relativePathの末尾に"/"があるか確認
    if (!std::string{ relativePath.back() }.compare("/")) { // 一致している場合0を返すので!で非0としている。
        // ある場合、そのまま名前を設定
        name = relativePath + fileName;
    }
    else {
        // がない場合、"/"を加えて名前を設定
        name = relativePath + "/" + fileName;
    }

    return textures_.at(name);
}

const Texture::MAP_VALUE Texture::GetTextureInfo(const std::string& pathAndFileName)
{
    return textures_.at(pathAndFileName);
}

void Texture::SetMapKey(const std::string keyName)
{
    name_ = keyName;
}
