#include "GraphicsPipeline.h"
#include <string>
#include <cassert>
#include "InitDirectX.h"
#include <d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

//�ÓI�ϐ��̎���
GraphicsPipeline GraphicsPipeline::gPipeline2d_{};
GraphicsPipeline GraphicsPipeline::gPipeline3d_{};

void GraphicsPipeline::Initialize(void)
{
    gPipeline2d_ = GraphicsPipeline(GraphicsPipeline::GPType::SPRITE);
    gPipeline3d_ = GraphicsPipeline(GraphicsPipeline::GPType::MODEL);
}

void GraphicsPipeline::Finalize(void)
{
    //gPipeline2d_.rootSignature_.Reset();
    //gPipeline2d_.pipelineState_.Reset();
    //gPipeline3d_.rootSignature_.Reset();
    //gPipeline3d_.pipelineState_.Reset();
}

GraphicsPipeline::GraphicsPipeline(GPType type)
{
    // �C���X�^���X�擾
    InitDirectX* iDX = InitDirectX::GetInstance();
    HRESULT r = S_FALSE;

#pragma region SPRITE��GraphicsPipeline
    if (type == GPType::SPRITE) {
#pragma region ���_���C�A�E�g�̐ݒ�
        // ���_���C�A�E�g
        D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
            {   // xyz���W(1�s�ŏ������ق������₷��)
                "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
            },
            { // uv���W(1�s�ŏ������ق������₷��)
                "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
            },
        };
#pragma endregion

#pragma region �V�F�[�_�[�̐ݒ�
        ComPtr<ID3DBlob> vsBlob{ nullptr }; // ���_�V�F�[�_�I�u�W�F�N�g
        ComPtr<ID3DBlob> psBlob{ nullptr }; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
        ComPtr<ID3DBlob> errorBlob{ nullptr }; // �G���[�I�u�W�F�N�g

#pragma region ���_�V�F�[�_
        // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
        r = D3DCompileFromFile(L"Resources/Shaders/SpriteVS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vsBlob, &errorBlob);

        // �G���[�Ȃ�
        if (FAILED(r))
        {
            // errorBlob����G���[���e��string�^�ɃR�s�[
            std::string error;
            error.resize(errorBlob->GetBufferSize());
            std::copy_n((char*)errorBlob->GetBufferPointer(),
                errorBlob->GetBufferSize(),
                error.begin());
            error += "\n";
            // �G���[���e���o�̓E�B���h�E�ɕ\��
            OutputDebugStringA(error.c_str());
            assert(0);
        }
#pragma endregion

#pragma region �s�N�Z���V�F�[�_
        // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
        r = D3DCompileFromFile(L"Resources/Shaders/SpritePS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &psBlob, &errorBlob);

        // �G���[�Ȃ�
        if (FAILED(r))
        {
            // errorBlob����G���[���e��string�^�ɃR�s�[
            std::string error;
            error.resize(errorBlob->GetBufferSize());
            std::copy_n((char*)errorBlob->GetBufferPointer(),
                errorBlob->GetBufferSize(),
                error.begin());
            error += "\n";
            // �G���[���e���o�̓E�B���h�E�ɕ\��
            OutputDebugStringA(error.c_str());
            assert(0);
        }
#pragma endregion
#pragma endregion

#pragma region �p�C�v���C���̐ݒ�i���L�j
        // �V�F�[�_�[�̐ݒ�
        pipelineDesc_.VS.pShaderBytecode = vsBlob->GetBufferPointer();
        pipelineDesc_.VS.BytecodeLength = vsBlob->GetBufferSize();
        pipelineDesc_.PS.pShaderBytecode = psBlob->GetBufferPointer();
        pipelineDesc_.PS.BytecodeLength = psBlob->GetBufferSize();

        // �T���v���}�X�N�̐ݒ�
        pipelineDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

        // ���X�^���C�U�̐ݒ�
        pipelineDesc_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // �J�����O���Ȃ�
        pipelineDesc_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
        pipelineDesc_.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

#pragma region �u�����h�ݒ�
    // �����_�[�^�[�Q�b�g�̃u�����h�ݒ�i�u�����h�X�e�[�g�j�����ꃁ���o�ɂ������������񂶂�ˁH�H
        D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc_.BlendState.RenderTarget[0];
        blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��

        // ���ʐݒ�i�A���t�@�l�j
        blenddesc.BlendEnable = true;                           // �u�����h��L���ɂ���
        blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;            // ���Z
        blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;              // �\�[�X�̒l��100%�g��
        blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;            // �f�X�g�̒l��0%�g��

        // ���Z����
        blenddesc.BlendOp = D3D12_BLEND_OP_ADD;                 // ���Z
        blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;             // �\�[�X�̒l��100%�g��
        blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;        // �f�X�g�̒l��100%�g��

        // ���Z����(RGB�l)
        blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;        // �f�X�g����\�[�X�����Z
        blenddesc.SrcBlend = D3D12_BLEND_ONE;                   // �\�[�X�̒l��100%�g��
        blenddesc.DestBlend = D3D12_BLEND_ONE;                  // �f�X�g�̒l��100%�g��

        // �F���](RGB�l)
        blenddesc.BlendOp = D3D12_BLEND_OP_ADD;                 // ���Z
        blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;        // 1.0f - �f�X�g�J���[�̒l
        blenddesc.DestBlend = D3D12_BLEND_ZERO;                 // �g��Ȃ�

        // ����������
        blenddesc.BlendOp = D3D12_BLEND_OP_ADD;                 // ���Z
        blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;             // �\�[�X�̃A���t�@�l
        blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;        // 1.0f - �\�[�X�̃A���t�@�l
#pragma endregion

        // ���_���C�A�E�g�̐ݒ�
        pipelineDesc_.InputLayout.pInputElementDescs = inputLayout;
        pipelineDesc_.InputLayout.NumElements = _countof(inputLayout);

        // �}�`�̌`��ݒ�
        pipelineDesc_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

        // ���̑��̐ݒ�
        pipelineDesc_.NumRenderTargets = 1; // �`��Ώۂ�1��
        pipelineDesc_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
        pipelineDesc_.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

#pragma region �f�X�N���v�^�����W
    // �f�X�N���v�^�����W�̕ϐ��錾
        D3D12_DESCRIPTOR_RANGE descriptorRange{};
        // �f�X�N���v�^�����W�̐ݒ�
        descriptorRange.NumDescriptors = 1;
        descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        descriptorRange.BaseShaderRegister = 0; // �e�N�X�`�����W�X�^0��
        descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
#pragma endregion

#pragma region ���[�g�p�����[�^
        // ���[�g�p�����[�^�[�̐ݒ�
        D3D12_ROOT_PARAMETER rootParams[3] = {};
        rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;    // �萔�o�b�t�@�r���[
        rootParams[0].Descriptor.ShaderRegister = 0;                    // �萔�o�b�t�@�ԍ�
        rootParams[0].Descriptor.RegisterSpace = 0;                     // �f�t�H���g�l
        rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;   // �S�ẴV�F�[�_���猩����
        // �e�N�X�`�����W�X�^0��
        rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	// ���
        rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
        rootParams[1].DescriptorTable.NumDescriptorRanges = 1;
        rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        // �萔�o�b�t�@1��
        rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;    // ���
        rootParams[2].Descriptor.ShaderRegister = 1;                    // �萔�o�b�t�@�ԍ�
        rootParams[2].Descriptor.RegisterSpace = 0;                     // �f�t�H���g�l
        rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;   // �S�ẴV�F�[�_���猩����
#pragma endregion

#pragma region �T���v���[�̐ݒ�
    //�e�N�X�`���T���v���[�̕ϐ��錾
        D3D12_STATIC_SAMPLER_DESC samplerDesc{};
        //�e�N�X�`���T���v���[�̐ݒ�
        samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���J��Ԃ�(�^�C�����O)
        samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//�c�J��Ԃ��i�^�C�����O�j
        samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���s�J��Ԃ��i�^�C�����O�j
        samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//�{�[�_�[�̎��͍�
        samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//�S�ăV�j�A���
        samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;//�~�b�v�}�b�v�ő�l
        samplerDesc.MinLOD = 0.0f;//�~�b�v�}�b�v�ŏ��l
        samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
        samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//�s�N�Z���V�F�[�_����̂ݎg�p�\
#pragma endregion

#pragma region ���[�g�V�O�l�`��
    // ���[�g�V�O�l�`���̕ϐ��錾
        D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
        // ���[�g�V�O�l�`���̐ݒ�
        rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        rootSignatureDesc.pParameters = rootParams; // ���[�g�p�����[�^�̐擪�A�h���X
        rootSignatureDesc.NumParameters = static_cast<uint32_t>(std::size(rootParams));
        rootSignatureDesc.pStaticSamplers = &samplerDesc;
        rootSignatureDesc.NumStaticSamplers = 1;

        ComPtr<ID3DBlob> rootSigBlob{ nullptr }; // ���[�g�V�O�l�`���̃V���A���C�Y

        r = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
            &rootSigBlob, &errorBlob);
#ifdef _DEBUG
        assert(SUCCEEDED(r));
#endif // _DEBUG

        r = iDX->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
            IID_PPV_ARGS(&rootSignature_));

        rootSignature_->SetName(L"rootSignature2d");

#ifdef _DEBUG
        assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

        // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
        pipelineDesc_.pRootSignature = rootSignature_.Get();
#pragma endregion �p�C�v���C���̐ݒ�i��L�j

        // �p�C�v���C���ݒ��ۑ�
        r = iDX->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc_, IID_PPV_ARGS(&pipelineState_));
#ifdef _DEBUG
        assert(SUCCEEDED(r));
#endif // _DEBUG
    }
#pragma endregion
#pragma region MODEL��GraphicsPipeline
    else if (type == GPType::MODEL) { // MODEL�^�C�v
#pragma region ���_���C�A�E�g
    // ���_���C�A�E�g
    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        { // xyz���W(1�s�ŏ������ق������₷��)
            "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
        },
        { // �@���x�N�g���i1�s�������ق����킩��₷��)
            "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        { // uv���W(1�s�ŏ������ق������₷��)
            "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
        },
    };
#pragma endregion

#pragma region �V�F�[�_�[�̐ݒ�
#pragma region ���_�V�F�[�_�[
    ComPtr<ID3DBlob> vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
    ComPtr<ID3DBlob> psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
    ComPtr<ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g
    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    r = D3DCompileFromFile(L"Resources/Shaders/ModelVS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vsBlob, &errorBlob);

    // �G���[�Ȃ�
    if (FAILED(r)) {
        // errorBlob����G���[���e��string�^�ɃR�s�[
        std::string error;
        error.resize(errorBlob->GetBufferSize());
        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            error.begin());
        error += "\n";
        // �G���[���e���o�̓E�B���h�E�ɕ\��
        OutputDebugStringA(error.c_str());
        assert(0);
    }
#pragma endregion

#pragma region �s�N�Z���V�F�[�_
    // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    r = D3DCompileFromFile(L"Resources/Shaders/ModelPS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &psBlob, &errorBlob);

    // �G���[�Ȃ�
    if (FAILED(r)) {
        // errorBlob����G���[���e��string�^�ɃR�s�[
        std::string error;
        error.resize(errorBlob->GetBufferSize());
        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            error.begin());
        error += "\n";
        // �G���[���e���o�̓E�B���h�E�ɕ\��
        OutputDebugStringA(error.c_str());
        assert(0);
    }
#pragma endregion
#pragma endregion

#pragma region �p�C�v���C���̐ݒ�i���L�j
    // �V�F�[�_�[�̐ݒ�
    pipelineDesc_.VS.pShaderBytecode = vsBlob->GetBufferPointer();
    pipelineDesc_.VS.BytecodeLength = vsBlob->GetBufferSize();
    pipelineDesc_.PS.pShaderBytecode = psBlob->GetBufferPointer();
    pipelineDesc_.PS.BytecodeLength = psBlob->GetBufferSize();

    // �T���v���}�X�N�̐ݒ�
    pipelineDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

    // ���X�^���C�U�̐ݒ�
    pipelineDesc_.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;   // �w�ʂ��J�����O
    pipelineDesc_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;  // �|���S�����h��Ԃ�
    pipelineDesc_.RasterizerState.DepthClipEnable = true;            // �[�x�N���b�s���O��L����

#pragma region �u�����h�ݒ�
    // �����_�[�^�[�Q�b�g�̃u�����h�ݒ�i�u�����h�X�e�[�g�j�����ꃁ���o�ɂ������������񂶂�ˁH�H
    D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = pipelineDesc_.BlendState.RenderTarget[0];
    blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��

    // ���ʐݒ�(�A���t�@�l)
    blendDesc.BlendEnable = true;					// �u�����h��L���ɂ���
    blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	// ���Z
    blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;		// �\�[�X�̒l��100%�g��
    blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;	// �f�X�g�̒l��0%�g��

    // ���Z����(RGB�l)
    blendDesc.BlendOp = D3D12_BLEND_OP_ADD;		// ���Z
    blendDesc.SrcBlend = D3D12_BLEND_ONE;		// �\�[�X�̒l��100%�g��
    blendDesc.DestBlend = D3D12_BLEND_ONE;		// �f�X�g�̒l��100%�g��

    // ���Z����(RGB�l)
    blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;		// �f�X�g�J���\�[�X�������Z
    blendDesc.SrcBlend = D3D12_BLEND_ONE;					// �\�[�X�̒l��100%�g��
    blendDesc.DestBlend = D3D12_BLEND_ONE;					// �f�X�g�̒l��100%�g��

    // �F���](RGB�l)
    blendDesc.BlendOp = D3D12_BLEND_OP_ADD;					// ���Z
    blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;		// 1.0f - �f�X�g�J���[�̒l
    blendDesc.DestBlend = D3D12_BLEND_ZERO;					// �g��Ȃ�

    // ����������
    blendDesc.BlendOp = D3D12_BLEND_OP_ADD;					// ���Z
    blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;				// �\�[�X�̃A���t�@�l
    blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		// 1.0f - �\�[�X�̃A���t�@�l
#pragma endregion

    // ���_���C�A�E�g�̐ݒ�
    pipelineDesc_.InputLayout.pInputElementDescs = inputLayout;
    pipelineDesc_.InputLayout.NumElements = _countof(inputLayout);

    // �}�`�̌`��ݒ�
    pipelineDesc_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    // ���̑��̐ݒ�
    pipelineDesc_.NumRenderTargets = 1; // �`��Ώۂ�1��
    pipelineDesc_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
    pipelineDesc_.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

#pragma region �f�X�N���v�^�����W
    // �f�X�N���v�^�����W�̐ݒ�
    D3D12_DESCRIPTOR_RANGE descriptorRange{};
    descriptorRange.NumDescriptors = 1;
    descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    descriptorRange.BaseShaderRegister = 0;	// �e�N�X�`�����W�X�^0��
    descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
#pragma endregion

#pragma region ���[�g�p�����[�^
    // ���[�g�p�����[�^�[�̐ݒ�
    D3D12_ROOT_PARAMETER rootParams[3] = {};
    // �萔�o�b�t�@0��
    rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// �萔�o�b�t�@�r���[
    rootParams[0].Descriptor.ShaderRegister = 0;					// �萔�o�b�t�@�ԍ�
    rootParams[0].Descriptor.RegisterSpace = 0;						// �f�t�H���g�l
    rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// �S�ẴV�F�[�_���猩����
    // �萔�o�b�t�@1��
    rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// ���
    rootParams[1].Descriptor.ShaderRegister = 1;					// �萔�o�b�t�@�ԍ�
    rootParams[1].Descriptor.RegisterSpace = 0;						// �f�t�H���g�l
    rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// �S�ẴV�F�[�_���猩����
    // �e�N�X�`�����W�X�^0��
    rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	// ���
    rootParams[2].DescriptorTable.pDescriptorRanges = &descriptorRange;
    rootParams[2].DescriptorTable.NumDescriptorRanges = 1;
    rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
#pragma endregion

#pragma region �T���v���[�̐ݒ�
    //�e�N�X�`���T���v���[�̐ݒ�
    D3D12_STATIC_SAMPLER_DESC samplerDesc{};
    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���J��Ԃ�(�^�C�����O)
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//�c�J��Ԃ��i�^�C�����O�j
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���s�J��Ԃ��i�^�C�����O�j
    samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//�{�[�_�[�̎��͍�
    samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//�S�ăV�j�A���
    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;//�~�b�v�}�b�v�ő�l
    samplerDesc.MinLOD = 0.0f;//�~�b�v�}�b�v�ŏ��l
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//�s�N�Z���V�F�[�_����̂ݎg�p�\
#pragma endregion

#pragma region ���[�g�V�O�l�`��
    //���[�g�V�O�l�`���̐ݒ�
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSignatureDesc.pParameters = rootParams;//���[�g�p�����[�^�̐擪�A�h���X
    rootSignatureDesc.NumParameters = _countof(rootParams);//���[�g�p�����[�^��
    rootSignatureDesc.pStaticSamplers = &samplerDesc;
    rootSignatureDesc.NumStaticSamplers = 1;

    ComPtr<ID3DBlob> rootSigBlob{ nullptr }; // ���[�g�V�O�l�`���̃V���A���C�Y
    r = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
        &rootSigBlob, &errorBlob);
    assert(SUCCEEDED(r));
    r = iDX->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
        IID_PPV_ARGS(&rootSignature_));

    rootSignature_->SetName(L"rootSignature3d");

    assert(SUCCEEDED(r));
#pragma endregion

    // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
    pipelineDesc_.pRootSignature = rootSignature_.Get();

#pragma region �[�x�ݒ�
    // �f�v�X�X�e���V���X�e�[�g�̐ݒ�
    pipelineDesc_.DepthStencilState.DepthEnable = true;  // �[�x�e�X�g���s��
    pipelineDesc_.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;     // �������݋���
    pipelineDesc_.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;          // ��������΍��i
    pipelineDesc_.DSVFormat = DXGI_FORMAT_D32_FLOAT;                                 // �[�x�l�t�H�[�}�b�g
#pragma endregion
#pragma endregion

#pragma region �p�C�v���C���X�e�[�g�̐���
    // �p�C�v���C���ݒ�̕ۑ�
    r = iDX->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc_, IID_PPV_ARGS(&pipelineState_));
    assert(SUCCEEDED(r));
#pragma endregion
    }
#pragma endregion
}
