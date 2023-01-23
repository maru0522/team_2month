#include "Model.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>
#include <DirectXMath.h>
#include "InitDirectX.h"
#include "Texture.h"
#include "GraphicsPipeline.h"
#include "Util.h"
#include<d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

// �ÓI�ϐ��̎���
std::map<Model::MODEL_KEY, Model::MODEL_VALUE> Model::models_{};

void Model::Load(const fsPath& pathAndObjName)
{
    const std::string path{ pathAndObjName.parent_path().string() + "/" };
    const std::string objName{ pathAndObjName.filename().string() };

    // �Ă�ۂ���
    Model tmp{};

#ifdef _DEBUG
    // ���ɓǂݍ��񂾃e�N�X�`���Ƃ̏d���m�F�B
    if (models_.count(pathAndObjName)) {
        OutputDebugString(L"A .obj file with the same name was loaded. The model may have been overwritten.");
    }
#endif // _DEBUG

    tmp.name_ = pathAndObjName;

#pragma region ���_�ƃC���f�b�N�X���W
    // �t�@�C���X�g���[��
    std::ifstream file;
    // .obj���J��
    file.open(pathAndObjName);
    // �t�@�C���I�[�v�����s���`�F�b�N
    assert(!file.fail());

    std::vector<DirectX::XMFLOAT3> positions{};   // ���_���W
    std::vector<DirectX::XMFLOAT3> normals{};     // �@���x�N�g��
    std::vector<DirectX::XMFLOAT2> texcoords{};   // �e�N�X�`��UV
    // 1�s���ǂݍ���
    std::string line;
    while (std::getline(file, line)) {
        // 1�s���̕�������X�g���[���ɕϊ�
        std::istringstream line_stream(line);

        // ���p�X�y�[�X��؂�ōs�̐擪��������擾
        std::string key;
        std::getline(line_stream, key, ' ');

        if (key == "mtllib") {
            // �}�e���A���̃t�@�C�����ǂݍ���
            std::string mtlFileName;
            line_stream >> mtlFileName;
            // �}�e���A���ǂݍ���
            LoadMaterial(tmp, pathAndObjName);
        }

        // �擪������v�Ȃ璸�_���W
        if (key == "v") {
            // X, Y, Z���W�ǂݍ���
            DirectX::XMFLOAT3 position{};
            line_stream >> position.x;
            line_stream >> position.y;
            line_stream >> position.z;
            // ���W�f�[�^�ɒǉ�
            positions.emplace_back(position);
        }

        // �擪������vt�Ȃ�e�N�X�`��
        if (key == "vt") {
            // UV������ǂݍ���
            DirectX::XMFLOAT2 texcoord{};
            line_stream >> texcoord.x;
            line_stream >> texcoord.y;
            // V�������]
            texcoord.y = 1.0f - texcoord.y;
            // �e�N�X�`�����W�f�[�^�ɒǉ�
            texcoords.emplace_back(texcoord);
        }

        // �擪������vn�Ȃ�@���x�N�g��
        if (key == "vn") {
            // XYZ������ǂݍ���
            DirectX::XMFLOAT3 normal{};
            line_stream >> normal.x;
            line_stream >> normal.y;
            line_stream >> normal.z;
            // �@���x�N�g���f�[�^�ɒǉ�
            normals.emplace_back(normal);
        }

        if (key == "f") {
            // ���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
            std::string index_string;
            while (std::getline(line_stream, index_string, ' ')) {
                //���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ�
                std::istringstream index_stream(index_string);
                uint16_t indexPosition{};
                uint16_t indexNormal{};
                uint16_t indexTexcoord{};
                index_stream >> indexPosition;
                index_stream.seekg(1, std::ios_base::cur); // �X���b�V�����΂�
                index_stream >> indexTexcoord;
                index_stream.seekg(1, std::ios_base::cur); // �X���b�V�����΂�
                index_stream >> indexNormal;
                // ���_�f�[�^�̒ǉ�
                VertexModel3d_st vertex{};
                vertex.pos_ = positions[indexPosition - 1];
                vertex.normal_ = normals[indexNormal - 1];
                vertex.uv_ = texcoords[indexTexcoord - 1];
                tmp.info_.vertices_.emplace_back(vertex);
                // ���_�C���f�b�N�X�f�[�^�̒ǉ�
                tmp.info_.indices_.emplace_back((uint16_t)tmp.info_.indices_.size());
            }
        }
    }
#pragma endregion

#pragma region ���_�o�b�t�@�̐���
    // ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
    uint32_t sizeVB = static_cast<uint32_t>(sizeof(VertexModel3d_st) * tmp.info_.vertices_.size());

    // ���_�o�b�t�@�̐ݒ�
    D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

    // ���\�[�X�ݒ�
    D3D12_RESOURCE_DESC resDesc{};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    InitDirectX* iDX = InitDirectX::GetInstance();

    // ���_�o�b�t�@�̐���
    HRESULT r = iDX->GetDevice()->CreateCommittedResource(
        &heapProp, // �q�[�v�ݒ�
        D3D12_HEAP_FLAG_NONE,
        &resDesc, // ���\�[�X�ݒ�
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&tmp.info_.vertBuff_));
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

#pragma region ���_�o�b�t�@�̃}�b�s���O
    // GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
    VertexModel3d_st* vertMap{ nullptr };
    r = tmp.info_.vertBuff_->Map(0, nullptr, (void**)&vertMap);
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
    // �S���_�ɑ΂���
    std::copy(tmp.info_.vertices_.begin(), tmp.info_.vertices_.end(), vertMap);

    // �q���������
    tmp.info_.vertBuff_->Unmap(0, nullptr);
#pragma endregion

#pragma region ���_�o�b�t�@�̃r���[�ݒ�
    // vbview
    // GPU���z�A�h���X
    tmp.info_.vbView_.BufferLocation = tmp.info_.vertBuff_->GetGPUVirtualAddress();
    // ���_�o�b�t�@�̃T�C�Y
    tmp.info_.vbView_.SizeInBytes = sizeVB;
    // ���_1���̃f�[�^�T�C�Y
    tmp.info_.vbView_.StrideInBytes = sizeof(VertexModel3d_st);
#pragma endregion

    //SetColor();

    //SetCBTransform();

#pragma region �C���f�b�N�X�o�b�t�@�̐���
    // �C���f�b�N�X�f�[�^�S�̂̃T�C�Y
    uint32_t sizeIB = static_cast<uint32_t>(sizeof(uint16_t) * tmp.info_.indices_.size());

    // ���\�[�X�ݒ�
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeIB;	// �C���f�b�N�X��񂪓��镪�̃T�C�Y
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // �C���f�b�N�X�o�b�t�@�̐���
    r = iDX->GetDevice()->CreateCommittedResource(
        &heapProp,	// �q�[�v�ݒ�
        D3D12_HEAP_FLAG_NONE,
        &resDesc,	// ���\�[�X�ݒ�
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&tmp.info_.indexBuff_));
#pragma endregion

#pragma region �C���f�b�N�X�o�b�t�@�̃}�b�s���O
    // �C���f�b�N�X�o�b�t�@���}�b�s���O
    uint16_t* indexMap = nullptr;
    r = tmp.info_.indexBuff_->Map(0, nullptr, (void**)&indexMap);
    // �S�C���f�b�N�X�ɑ΂���
    std::copy(tmp.info_.indices_.begin(), tmp.info_.indices_.end(), indexMap);

    // �}�b�s���O����
    tmp.info_.indexBuff_->Unmap(0, nullptr);
#pragma endregion

#pragma region �C���f�b�N�X�o�b�t�@�̃r���[�ݒ�
    // �C���f�b�N�X�o�b�t�@�r���[�̍쐬
    tmp.info_.ibView_.BufferLocation = tmp.info_.indexBuff_->GetGPUVirtualAddress();
    tmp.info_.ibView_.Format = DXGI_FORMAT_R16_UINT;
    tmp.info_.ibView_.SizeInBytes = sizeIB;
#pragma endregion

    // map�ւ̑}���ior ����j������̏ꍇ�A����KEY�ɑ΂���VALUE���㏑������邽�ߒ���
    models_.insert_or_assign(tmp.name_, tmp.info_); // ������ł����Ă��S������VALUE������Ƃ͎v����B
}

void Model::Finalize(void)
{
    for (std::pair<const MODEL_KEY, MODEL_VALUE>& mpElem : models_) {
        if(mpElem.second.vertBuff_)mpElem.second.vertBuff_.Reset();
        if(mpElem.second.indexBuff_)mpElem.second.indexBuff_.Reset();
    }
}

const Model::MODEL_VALUE* Model::GetMODEL_VALUE(const fsPath& pathAndObjName)
{
    return &models_.at(pathAndObjName);
}

void Model::LoadMaterial(Model& model, const fsPath& pathAndObjName)
{
    const std::string path{ pathAndObjName.parent_path().string() + "/" };
    const std::string mtlname{ pathAndObjName.stem().string() + ".mtl" };

    //const std::string a{ "Resources/3dModels/cube/untitled.mtl" };

    // �t�@�C���X�g���[��
    std::ifstream file;
    // �}�e���A���t�@�C�����J��
    file.open(path + mtlname);
    // �t�@�C���I�[�v�����s���`�F�b�N
    if (file.fail()) assert(0);

    // 1�s���ǂݍ���
    std::string line;
    while (std::getline(file, line)) {
        // 1�s���̕�������X�g���[���ɕϊ�
        std::istringstream line_stream(line);

        // ���p�X�y�[�X��؂�ōs�̐擪��������擾
        std::string key;
        std::getline(line_stream, key, ' ');

        // �擪�̃^�u�����͖�������
        if (key[0] == '\t') {
            key.erase(key.begin()); // �擪�̕������폜
        }

        // �擪������newmtl�Ȃ�}�e���A����
        if (key == "newmtl") {
            // �}�e���A�����ǂݍ���
            line_stream >> model.info_.material_.name;
        }

        // �擪������Ka�Ȃ�A���r�G���g�F
        if (key == "Ka") {
            line_stream >> model.info_.material_.ambient.x;
            line_stream >> model.info_.material_.ambient.y;
            line_stream >> model.info_.material_.ambient.z;
        }

        // �擪������Ȃ�Kd�Ȃ�f�B�t���[�Y�F
        if (key == "kd") {
            line_stream >> model.info_.material_.diffuse.x;
            line_stream >> model.info_.material_.diffuse.y;
            line_stream >> model.info_.material_.diffuse.z;
        }

        // �擪������Ks�Ȃ�X�y�L�����[�F
        if (key == "Ks") {
            line_stream >> model.info_.material_.specular.x;
            line_stream >> model.info_.material_.specular.y;
            line_stream >> model.info_.material_.specular.z;
        }

        // �擪������map_Kd�Ȃ�e�N�X�`���t�@�C����
        if (key == "map_Kd") {
            Texture::TEXTURE_KEY tmpName;

            // �e�N�X�`���̃t�@�C�����ǂݍ���
            line_stream >> tmpName;
            model.info_.material_.texKey = pathAndObjName.parent_path() / tmpName;

            // �e�N�X�`���ǂݍ���
            Texture::Load(model.info_.material_.texKey);
        }
    }
}

void Model::Update(void)
{
    UpdateMaterial();
}

void Model::UpdateMaterial(void)
{
    info_.constBuffB1_.GetConstBuffMap()->ambient = info_.material_.ambient;
    info_.constBuffB1_.GetConstBuffMap()->diffuse = info_.material_.diffuse;
    info_.constBuffB1_.GetConstBuffMap()->specular = info_.material_.specular;
    info_.constBuffB1_.GetConstBuffMap()->alpha = info_.material_.alpha;
}

//void Model::PreDraw(void)
//{
//    // �C���X�^���X�擾
//    InitDirectX* iDX = InitDirectX::GetInstance();
//
//    // �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
//    iDX->GetCommandList()->SetPipelineState(graphicsPipeline_.GetPipelineState());
//    iDX->GetCommandList()->SetGraphicsRootSignature(graphicsPipeline_.GetRootSignature());
//
//    // �v���~�e�B�u�`��̐ݒ�R�}���h
//    iDX->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g
//
//    ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetSRVHeap() };
//    // SRV�q�[�v�̐ݒ�R�}���h
//    iDX->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
//}

//void Model::Draw(void)
//{
//    // �C���X�^���X�擾
//    InitDirectX* iDX = InitDirectX::GetInstance();
//
//    SetCommandsBeforeDraw();
//
//#pragma region ���_�o�b�t�@
//    // ���_�o�b�t�@�̐ݒ�
//    iDX->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
//#pragma endregion
//
//#pragma region �C���f�b�N�X�o�b�t�@
//    // �C���f�b�N�X�o�b�t�@�̐ݒ�
//    iDX->GetCommandList()->IASetIndexBuffer(&ibView_);
//#pragma endregion
//
//#pragma region ConstBuffer
//    // �萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
//    //iDX->GetCommandList()->SetGraphicsRootConstantBufferView(0, cbMaterial_.GetBuffer()->GetGPUVirtualAddress());
//    iDX->GetCommandList()->SetGraphicsRootConstantBufferView(0, worldCoordinate_.GetCBData()->GetBuffer()->GetGPUVirtualAddress());
//    //iDX->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbTransform_.GetBuffer()->GetGPUVirtualAddress());
//    iDX->GetCommandList()->SetGraphicsRootConstantBufferView(1, constBuffB1_.GetBuffer()->GetGPUVirtualAddress());
//#pragma endregion
//
//#pragma region SRV
//    //�w��̃q�[�v�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
//    iDX->GetCommandList()->SetGraphicsRootDescriptorTable(2, srvGpuHandleCopy_);
//#pragma endregion
//
//    // �`��R�}���h���X�g
//    iDX->GetCommandList()->DrawIndexedInstanced(static_cast<uint32_t>(indices_.size()), 1, 0, 0, 0);
//}
//
//
//void Model::SetCommandsBeforeDraw(void)
//{
//    // �C���X�^���X�擾
//    InitDirectX* iDX = InitDirectX::GetInstance();
//
//    // �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
//    iDX->GetCommandList()->SetPipelineState(graphicsPipeline_.GetPipelineState());
//    iDX->GetCommandList()->SetGraphicsRootSignature(graphicsPipeline_.GetRootSignature());
//
//    // �v���~�e�B�u�`��̐ݒ�R�}���h
//    iDX->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g
//
//    ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetSRVHeap() };
//    // SRV�q�[�v�̐ݒ�R�}���h
//    iDX->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
//}