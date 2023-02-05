#include "Obj3d.h"
#include "Model.h"
#include "InitDirectX.h"

GraphicsPipeline* Obj3d::graphicsPipeline_{ GraphicsPipeline::GetGraphicsPipeLine3d() };

Obj3d::Obj3d(void)
{
    model_.SetMODEL_KEY("Resources/3dModels/cube/cube.obj");
    model_.SetMODEL_VALUE(*Model::GetMODEL_VALUE("Resources/3dModels/cube/cube.obj")); // �Ȃ񂩂��ꐦ���댯�ȓn�����ȋC������

    // �����Ŏ~�܂��Ă�Ȃ�.mtl���Ő������摜���L����Ă��Ȃ��\���B
    srvGpuHandleCopy_ = Texture::GetTextureInfo(model_.GetModelValueInfo()->material_.texKey)->srvGpuHandle_;
}

Obj3d::Obj3d(const fsPath& pathAndObjName)
{
    model_.SetMODEL_KEY(pathAndObjName);
    model_.SetMODEL_VALUE(*Model::GetMODEL_VALUE(pathAndObjName)); // �Ȃ񂩂��ꐦ���댯�ȓn�����ȋC������

    // �����Ŏ~�܂��Ă�Ȃ�.mtl���Ő������摜���L����Ă��Ȃ��\���B
    srvGpuHandleCopy_ = Texture::GetTextureInfo(model_.GetModelValueInfo()->material_.texKey)->srvGpuHandle_;
}

Obj3d::Obj3d(const fsPath& pathAndObjName, Camera* pCamera)
{
    worldCoordinate_.SetCamera(pCamera);

    model_.SetMODEL_KEY(pathAndObjName);

    model_.SetMODEL_VALUE(*Model::GetMODEL_VALUE(pathAndObjName)); // �Ȃ񂩂��ꐦ���댯�ȓn�����ȋC������

    // �����Ŏ~�܂��Ă�Ȃ�.mtl���Ő������摜���L����Ă��Ȃ��\���B
    srvGpuHandleCopy_ = Texture::GetTextureInfo(model_.GetModelValueInfo()->material_.texKey)->srvGpuHandle_;
}

void Obj3d::Update(void)
{
    model_.Update();
    worldCoordinate_.Update();
}

void Obj3d::PreDraw(void)
{
    // �C���X�^���X�擾
    InitDirectX* iDX = InitDirectX::GetInstance();

    // �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
    iDX->GetCommandList()->SetPipelineState(graphicsPipeline_->GetPipelineState());
    iDX->GetCommandList()->SetGraphicsRootSignature(graphicsPipeline_->GetRootSignature());

    // �v���~�e�B�u�`��̐ݒ�R�}���h
    iDX->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g

    ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetSRVHeap() };
    // SRV�q�[�v�̐ݒ�R�}���h
    iDX->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void Obj3d::Draw(void)
{
    // �C���X�^���X�擾
    InitDirectX* iDX = InitDirectX::GetInstance();

#pragma region ���_�o�b�t�@
    // ���_�o�b�t�@�̐ݒ�
    iDX->GetCommandList()->IASetVertexBuffers(0, 1, &model_.GetModelValueInfo()->vbView_);
#pragma endregion

#pragma region �C���f�b�N�X�o�b�t�@
    // �C���f�b�N�X�o�b�t�@�̐ݒ�
    iDX->GetCommandList()->IASetIndexBuffer(&model_.GetModelValueInfo()->ibView_);
#pragma endregion

#pragma region ConstBuffer
    // �萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
    //iDX->GetCommandList()->SetGraphicsRootConstantBufferView(0, cbMaterial_.GetBuffer()->GetGPUVirtualAddress());
    iDX->GetCommandList()->SetGraphicsRootConstantBufferView(0, worldCoordinate_.GetCBData()->GetBuffer()->GetGPUVirtualAddress());
    //iDX->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbTransform_.GetBuffer()->GetGPUVirtualAddress());
    iDX->GetCommandList()->SetGraphicsRootConstantBufferView(1, model_.GetModelValueInfo()->constBuffB1_.GetBuffer()->GetGPUVirtualAddress());
#pragma endregion

#pragma region SRV
    //�w��̃q�[�v�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
    iDX->GetCommandList()->SetGraphicsRootDescriptorTable(2, srvGpuHandleCopy_);
#pragma endregion

    // �`��R�}���h���X�g
    iDX->GetCommandList()->DrawIndexedInstanced((uint32_t)model_.GetModelValueInfo()->indices_.size(), 1, 0, 0, 0);
}

void Obj3d::SetModel(const fsPath& path)
{
    model_.SetMODEL_KEY(path);

    model_.SetMODEL_VALUE(*Model::GetMODEL_VALUE(path));

    srvGpuHandleCopy_ = srvGpuHandleCopy_ = Texture::GetTextureInfo(path.parent_path() / path.stem() += ".png")->srvGpuHandle_;
}

void Obj3d::SetTexture(const fsPath& pathAndFileName)
{
    srvGpuHandleCopy_ = Texture::GetTextureInfo(pathAndFileName)->srvGpuHandle_;
}

void Obj3d::SetTextureById(const std::string& id)
{
    srvGpuHandleCopy_ = Texture::GetTextureInfoById(id)->srvGpuHandle_;
}