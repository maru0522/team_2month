#include "Texture.h"
#include <cassert>
#include "Util.h"
#include <DirectXTex.h>
#include "InitDirectX.h"

// �ÓI�ϐ��̎��̐���
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> Texture::srvHeap_ = nullptr;
std::map<Texture::TEXTURE_KEY, Texture::TEXTURE_VALUE> Texture::textures_{};
std::map<Texture::TEX_KEY_ID, Texture::TEXTURE_KEY> Texture::mapKeys_{};
uint32_t Texture::indexNextDescHeap_{ 0 };

void Texture::Initialize(void)
{
    InitDirectX* iDX_ = InitDirectX::GetInstance();

    // �f�X�N���v�^�q�[�v�̐ݒ�
    D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
    srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// �V�F�[�_���猩����悤��
    srvDescHeapDesc.NumDescriptors = maxSRVDesc_;

    // �ݒ������SRV�p�f�X�N���v�^�q�[�v���쐬
    HRESULT r = iDX_->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&srvHeap_));

    srvHeap_->SetName(L"textureDescriptorHeap");

#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG

    GenerateMissingTexture();
}

void Texture::Finalize(void)
{
    for (std::pair<const TEXTURE_KEY, TEXTURE_VALUE>& mpElem : textures_) {
        //if (mpElem.second.buff_) mpElem.second.buff_.Reset();
    }
    textures_.clear();
    //if (srvHeap_) srvHeap_.Reset();
}

void Texture::Load(const fsPath& pathAndFileName)
{
#pragma region ���낢��m�F
    // srvDesc�̋󂫊m�F
    assert(indexNextDescHeap_ < maxSRVDesc_);
    uint32_t handle{ indexNextDescHeap_ };

    Texture tmp{}; // �ꎞobj

    tmp.SetMapKey(pathAndFileName);

    // ���ɓǂݍ��񂾃e�N�X�`���Ƃ̏d���m�F�B
    if (textures_.count(tmp.name_)) {
        // �d�����������ꍇ�͓ǂݍ��݂͂��Ȃ��B
        OutputDebugString(L"WARNING: An image with the same name was loaded.");
        return;
    }
    else {
        // �d�����Ȃ������ꍇ�͎��̃e�N�X�`���̂��߂Ƀn���h���̎w�W��1�i�߂�B
        indexNextDescHeap_++;
    }
#pragma endregion

#pragma region �e�N�X�`���̃��[�h
    //�摜�C���[�W�f�[�^�z��
    DirectX::TexMetadata metadata{};
    DirectX::ScratchImage scratchImg{};

    // WIC�e�N�X�`���̃��[�h�Ɏg�� path�𕶎���ϊ�
    std::string path{ tmp.name_.string() };
    std::wstring wPath{ path.begin(),path.end() };
    const wchar_t* szFile{ wPath.c_str() };

    // WIC�e�N�X�`���̃��[�h
    HRESULT r = LoadFromWICFile(
        szFile,   //�uResources�v�t�H���_�́utexture.png�v
        DirectX::WIC_FLAGS_NONE,
        &metadata, scratchImg);
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

#pragma region �~�b�v�}�b�v
    DirectX::ScratchImage mipChain{};

    // �~�b�v�}�b�v����
    r = GenerateMipMaps(scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(), DirectX::TEX_FILTER_DEFAULT, 0, mipChain);
    if (SUCCEEDED(r)) {
        scratchImg = std::move(mipChain);
        metadata = scratchImg.GetMetadata();
    }
#pragma endregion

    // �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
    metadata.format = DirectX::MakeSRGB(metadata.format);

#pragma region �q�[�v�ݒ�ƃf�X�N���v�^�ݒ�
    // �q�[�v�ݒ�
    D3D12_HEAP_PROPERTIES texHeapProp{};
    texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
    texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

    // ���\�[�X�ݒ�
    D3D12_RESOURCE_DESC textureResourceDesc{};
    textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    textureResourceDesc.Format = metadata.format;
    textureResourceDesc.Width = metadata.width;
    textureResourceDesc.Height = static_cast<UINT>(metadata.height);
    textureResourceDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
    textureResourceDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
    textureResourceDesc.SampleDesc.Count = 1;
#pragma endregion

    // InitDirectX�̃C���X�^���X�擾
    InitDirectX* iDX = InitDirectX::GetInstance();

#pragma region �e�N�X�`���o�b�t�@
    // �e�N�X�`���o�b�t�@�̐���
    r = iDX->GetDevice()->CreateCommittedResource(
        &texHeapProp,
        D3D12_HEAP_FLAG_NONE,
        &textureResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&tmp.info_.buff_)); // MAP_VALUE��buff_�֏�������

    tmp.info_.buff_->SetName(L"textureclassBuff");

#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

#pragma region srvCpuHandle�̕ۑ��ƃo�b�t�@�ւ̃f�[�^�]��
    // �f�X�N���v�^�̃T�C�Y���擾����
    uint32_t incrementSize = iDX->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    // MAP_VALUE��srvCPUHandle_�֏�������
    tmp.info_.srvCpuHandle_ = srvHeap_.Get()->GetCPUDescriptorHandleForHeapStart(); // Desc�̃q�[�v�̈�̃X�^�[�g�ʒu���擾
    tmp.info_.srvCpuHandle_.ptr += (size_t)incrementSize * (size_t)handle;

    // �S�~�b�v�}�b�v�ɂ���
    for (size_t i = 0; i < metadata.mipLevels; i++) {
        // �~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
        const DirectX::Image* img = scratchImg.GetImage(i, 0, 0);

        // �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
        r = tmp.info_.buff_-> // MAP_VALUE��buff_
            WriteToSubresource(
                static_cast<UINT>(i),
                nullptr,		// �S�̈�փR�s�[
                img->pixels,	// ���f�[�^�A�h���X
                static_cast<UINT>(img->rowPitch),	// 1���C���T�C�Y
                static_cast<UINT>(img->slicePitch)	// �S�T�C�Y
            );
#ifdef _DEBUG
        assert(SUCCEEDED(r));
#endif // _DEBUG
    }
#pragma endregion

#pragma region SRV�̐ݒ�Ɛ���
    // �V�F�[�_���\�[�X�r���[�ݒ�
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = textureResourceDesc.Format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

    // �n���h���̂����ʒu�ɃV�F�[�_�[���\�[�X�r���[�̍쐬
    iDX->GetDevice()->CreateShaderResourceView(tmp.info_.buff_.Get(), &srvDesc, tmp.info_.srvCpuHandle_);
#pragma endregion

#pragma region srvGpuHandle�̕ۑ���map�ɕR�Â��ĕۑ�
    // MAP_VALUE��srvGPUHandle_�֏�������
    tmp.info_.srvGpuHandle_ = srvHeap_.Get()->GetGPUDescriptorHandleForHeapStart(); // Desc�̃q�[�v�̈�̃X�^�[�g�ʒu���擾
    // �n���h����i�߂�
    tmp.info_.srvGpuHandle_.ptr += (size_t)incrementSize * (size_t)handle;

    // map�ւ̑}��
    textures_.insert({ tmp.name_, tmp.info_ });
#pragma endregion
}

void Texture::Load(const fsPath& pathAndFileName, const std::string& id)
{
    // ���ɓǂݍ��񂾃e�N�X�`���Ƃ̏d���m�F�B
    if (textures_.count(pathAndFileName)) {
        // �d�����������ꍇ�͓ǂݍ��݂͂��Ȃ��B
        OutputDebugString(L"WARNING: An image with the same name was loaded.");
        return;
    }
    else {
        // �d�����Ȃ��ꍇ�ǂݍ���
        Load(pathAndFileName);
    }

    // ���ɐݒ肵��id�Ƃ̏d���m�F
    if (mapKeys_.count(id)) {
        try {
            // �d��������ꍇ��O�X���[
            throw std::logic_error("ERROR: The same \"registerId\"(TEX_KEY_ID) is already in use.");
        }
        catch (const std::logic_error&) {
            // �����I���B
            std::exit(1);
        }
    }
    // MAP_KEY�Ƃ̕R�t��
    mapKeys_.insert({ id, pathAndFileName });
}

const bool Texture::ExistTexture(const std::string& path)
{
    return textures_.count(path);
}

void Texture::SetId(const fsPath& pathAndFileName, const std::string& id)
{
    // ���ɓǂݍ���ł���e�N�X�`���̒��Ɉ�v������̂����邩�m�F
    if (textures_.count(pathAndFileName)) {
        // ����ꍇ�AID�̏d�����m�F
        if (mapKeys_.count(id)) {
            // ID������ꍇ
            try {
                // ��O�X���[
                throw std::logic_error("ERROR: The same \"registerId\"(TEX_KEY_ID) is already in use.");
            }
            catch (const std::logic_error&) {
                // �����I���B
                std::exit(1);
            }
        }
        else {
            // ID�����蓖�Ă�e�N�X�`�������݂��Ă��� + ID������Ă��Ȃ��ꍇ
            mapKeys_.insert({ id, pathAndFileName });
        }
    }
    else {
        try {
            // �Ȃ��ꍇ�A��O�𓊂���
            throw std::logic_error("Specified TEXTURE_KEY does not exist.");
        }
        catch (const std::logic_error&) {
            // �����I���B
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
    // id�����݂��邩
    if (!mapKeys_.count(id)) {
        try {
            // ���݂��Ȃ��ꍇ��O�X���[
            throw std::logic_error("ERROR: Specified \"id\"(TEX_KEY_ID) does not exist.");
        }
        catch (const std::logic_error&) {
            // �����I���B
            std::exit(1);
        }
    }
    else {
        // ���݂���ꍇ�폜
        mapKeys_.erase(id);
    }
}

void Texture::GenerateMissingTexture(void)
{
#pragma region �G���[�摜�̐����Ɠo�^
#pragma region missingTexture����
    uint32_t handle{ indexNextDescHeap_ };

    Texture tmp{}; // �Ă�ۂ���

    tmp.SetMapKey("ERROR_IMAGE");
    indexNextDescHeap_++;

    // ��ӂ̃s�N�Z����
    constexpr size_t imageLength{ 256 };
    // �z��̗v�f��
    constexpr size_t imageDataCount{ imageLength * imageLength };

    // �C���[�W�f�[�^�z��
    DirectX::XMFLOAT4* imageData{ new DirectX::XMFLOAT4[imageDataCount] };

    // ����
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
#pragma region �q�[�v�ݒ�ƃf�X�N���v�^�ݒ�
    // �q�[�v�ݒ�
    D3D12_HEAP_PROPERTIES texHeapProp{};
    texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
    texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

    // ���\�[�X�ݒ�
    D3D12_RESOURCE_DESC textureResourceDesc{};
    textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    textureResourceDesc.Width = imageLength;
    textureResourceDesc.Height = imageLength;
    textureResourceDesc.DepthOrArraySize = 1;
    textureResourceDesc.MipLevels = 1;
    textureResourceDesc.SampleDesc.Count = 1;
#pragma endregion

    // InitDirectX�̃C���X�^���X�擾
    InitDirectX* iDX = InitDirectX::GetInstance();

#pragma region �e�N�X�`���o�b�t�@
    // �e�N�X�`���o�b�t�@�̐���
    HRESULT r = iDX->GetDevice()->CreateCommittedResource(
        &texHeapProp,
        D3D12_HEAP_FLAG_NONE,
        &textureResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&tmp.info_.buff_)); // MAP_VALUE��buff_�֏�������
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

#pragma region srvCpuHandle�̕ۑ��ƃo�b�t�@�ւ̃f�[�^�]��
    // �f�X�N���v�^�̃T�C�Y���擾����
    uint32_t incrementSize = iDX->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    // MAP_VALUE��srvCPUHandle_�֏�������
    tmp.info_.srvCpuHandle_ = srvHeap_.Get()->GetCPUDescriptorHandleForHeapStart(); // Desc�̃q�[�v�̈�̃X�^�[�g�ʒu���擾
    tmp.info_.srvCpuHandle_.ptr += (size_t)incrementSize * (size_t)handle;

    // �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
    r = tmp.info_.buff_-> // MAP_VALUE��buff_
        WriteToSubresource(
            0,
            nullptr,		// �S�̈�փR�s�[
            imageData,	// ���f�[�^�A�h���X
            sizeof(DirectX::XMFLOAT4) * imageLength,	// 1���C���T�C�Y
            sizeof(DirectX::XMFLOAT4) * imageDataCount// �S�T�C�Y
        );

    //�C���[�W�f�[�^���
    delete[] imageData;

#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

#pragma region SRV�̐ݒ�Ɛ���
    // �V�F�[�_���\�[�X�r���[�ݒ�
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = textureResourceDesc.Format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

    // �n���h���̂����ʒu�ɃV�F�[�_�[���\�[�X�r���[�̍쐬
    iDX->GetDevice()->CreateShaderResourceView(tmp.info_.buff_.Get(), &srvDesc, tmp.info_.srvCpuHandle_);
#pragma endregion

#pragma region srvGpuHandle�̕ۑ���map�ɕR�Â��ĕۑ�
    // MAP_VALUE��srvGPUHandle_�֏�������
    tmp.info_.srvGpuHandle_ = srvHeap_.Get()->GetGPUDescriptorHandleForHeapStart(); // Desc�̃q�[�v�̈�̃X�^�[�g�ʒu���擾
    // �n���h����i�߂�
    tmp.info_.srvGpuHandle_.ptr += (size_t)incrementSize * (size_t)handle;

    // map�ւ̑}��
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