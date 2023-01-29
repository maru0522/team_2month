#include "InitDirectX.h"
#include <cassert>
#include <vector>
#include "Window.h"

InitDirectX* InitDirectX::GetInstance(void)
{
    static InitDirectX* mIDX = new InitDirectX{};
	return mIDX;
}

void InitDirectX::DeleteInstance(void)
{
    delete GetInstance();
}

void InitDirectX::Initialize(void)
{
	// FPS�Œ艻����������
	fpsCtrller_.Initialize();

	DebugLayer();       // �f�o�b�O���C���[���I��
	DXGIDevice();
#ifdef _DEBUG
	SuppressErrors();   // �ꕔ�̃G���[��}��
#endif
	Commands();
	SwapChain();
	RTVDescHeap();
	RTV();
	DepthBuffer();
	Fence();
}

void InitDirectX::PreDraw(void)
{
#pragma region ���\�[�X�o���A�̕ύX
	// �o�b�N�o�b�t�@�̔ԍ����擾�i2�Ȃ̂�0�Ԃ�1�ԁj
	UINT bbIndex = swapChain_->GetCurrentBackBufferIndex();

	// �P�D���\�[�X�o���A�ŏ������݉\�ɕύX
	D3D12_RESOURCE_BARRIER barrierDesc{};
	barrierDesc.Transition.pResource = backBuffers_.at(bbIndex).Get(); // �o�b�N�o�b�t�@���w��
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // �\����Ԃ���
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`���Ԃ�
	commandList_->ResourceBarrier(1, &barrierDesc);
#pragma endregion

#pragma region �`���̕ύX
	// �Q�D�`���̕ύX
	// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * device_->GetDescriptorHandleIncrementSize(rtvDescHeapDesc_.Type);

	// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap_->GetCPUDescriptorHandleForHeapStart();

	// �����_�[�^�[�Q�b�g���Z�b�g
	commandList_->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
#pragma endregion

#pragma region ��ʃN���A
	// �R�D��ʃN���A 
	ClearRTV();

	// �[�x�o�b�t�@�N���A
	ClearDepthBuff();
#pragma endregion

#pragma region �O���t�B�b�N�X�R�}���h���͑O�ݒ�
	// �S�D�`��R�}���h��������
	// �r���[�|�[�g�ݒ�R�}���h
	D3D12_VIEWPORT viewport{};
	viewport.Width = Window::width_;
	viewport.Height = Window::height_;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	// �r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	commandList_->RSSetViewports(1, &viewport);

	// �V�U�[��`
	D3D12_RECT scissorRect{};
	scissorRect.left = 0; // �؂蔲�����W��
	scissorRect.right = scissorRect.left + Window::width_; // �؂蔲�����W�E
	scissorRect.top = 0; // �؂蔲�����W��
	scissorRect.bottom = scissorRect.top + Window::height_; // �؂蔲�����W��
	// �V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	commandList_->RSSetScissorRects(1, &scissorRect);
#pragma endregion
	// ----- ������PreDraw()��PostDraw()�̊Ԃɕ`��R�}���h���������ށ����� ----- //
}

void InitDirectX::PostDraw(void)
{
	// ----- ������PreDraw()��PostDraw()�̊Ԃɕ`��R�}���h���������ށ����� ----- //
	// �S�D�`��R�}���h�����܂�

	// �o�b�N�o�b�t�@�̔ԍ����擾�i2�Ȃ̂�0�Ԃ�1�ԁj
	UINT bbIndex = swapChain_->GetCurrentBackBufferIndex();

#pragma region ���\�[�X�o���A�����ɖ߂�
	// �T�D���\�[�X�o���A��߂�
	D3D12_RESOURCE_BARRIER barrierDesc{};
	barrierDesc.Transition.pResource = backBuffers_.at(bbIndex).Get(); // �o�b�N�o�b�t�@���w��
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`���Ԃ���
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // �\����Ԃ�
	commandList_->ResourceBarrier(1, &barrierDesc);
#pragma endregion

#pragma region �R�}���h���X�g���s
	// ���߂̃N���[�Y
	HRESULT r = commandList_->Close();
#ifdef _DEBUG
	assert(SUCCEEDED(r));
#endif // _DEBUG


	// �R�}���h���X�g�̎��s
	ID3D12CommandList* commandLists[] = { commandList_.Get() };
	commandQueue_->ExecuteCommandLists(1, commandLists);
	// ��ʂɕ\������o�b�t�@���t���b�v�i���\�̓��ւ��j
	r = swapChain_->Present(1, 0);
#ifdef _DEBUG
	assert(SUCCEEDED(r));
#endif // _DEBUG


	// �R�}���h�̎��s������҂�
	commandQueue_->Signal(fence_.Get(), ++fenceVal_);
	if (fence_->GetCompletedValue() != fenceVal_) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		if (event != 0) {
			fence_->SetEventOnCompletion(fenceVal_, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
	}

	// FPS�Œ�
	fpsCtrller_.Update();

	// �L���[���N���A
	r = cmdAllocator_->Reset();
#ifdef _DEBUG
	assert(SUCCEEDED(r));
#endif // _DEBUG


	// �ĂуR�}���h���X�g�𒙂߂鏀��
	r = commandList_->Reset(cmdAllocator_.Get(), nullptr);
#ifdef _DEBUG
	assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion
}

void InitDirectX::DebugLayer(void)
{
#ifdef _DEBUG
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController_)))) {
		debugController_->EnableDebugLayer();
		debugController_->SetEnableGPUBasedValidation(true);
	}
#endif // _DEBUG
}

void InitDirectX::DXGIDevice(void)
{
#pragma region �A�_�v�^�̗�
	// DXGI�t�@�N�g���[�̐���
	HRESULT r = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));

#ifdef _DEBUG
	assert(SUCCEEDED(r));
#endif // _DEBUG

	// �A�_�v�^�[�̗񋓗p
	std::vector<ComPtr<IDXGIAdapter4>> adapters;

	// �����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	ComPtr<IDXGIAdapter4> tmpAdapter = nullptr;

	// �p�t�H�[�}���X���������̂��珇�ɁA�S�ẴA�_�v�^�[��񋓂���
	for (UINT i = 0;
		dxgiFactory_->EnumAdapterByGpuPreference( // �����̈�����UINT�̂��� i �̌^��size_t�łȂ�
			i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++) {

		// ���I�z��ɒǉ�����
		adapters.push_back(tmpAdapter);
	}
#pragma endregion

#pragma region �A�_�v�^�̑I��
	// �Ó��ȃA�_�v�^��I�ʂ���
	for (size_t i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;

		// �A�_�v�^�[�̏����擾����
		adapters.at(i)->GetDesc3(&adapterDesc);

		// �\�t�g�E�F�A�f�o�C�X�����
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// �f�o�C�X���̗p���ă��[�v�𔲂���
			tmpAdapter = adapters.at(i);

			break;
		}
	}
#pragma endregion

#pragma region �f�o�C�X�̐���
	// �Ή����x���̔z��
	std::vector<D3D_FEATURE_LEVEL> levels{
	 D3D_FEATURE_LEVEL_12_1,
	 D3D_FEATURE_LEVEL_12_0,
	 D3D_FEATURE_LEVEL_11_1,
	 D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;
	for (size_t i = 0; i < levels.size(); i++) {
		// �̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		r = D3D12CreateDevice(tmpAdapter.Get(), levels.at(i),
			IID_PPV_ARGS(&device_));

		if (r == S_OK) {
			// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels.at(i);
			break;
		}
	}
#pragma endregion
}

void InitDirectX::SuppressErrors(void)
{
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue_)))) {
        infoQueue_->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);    // ��΂��G���[�ꎞ�Ɏ~�܂�
        infoQueue_->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);        // �G���[���Ɏ~�܂�
        //infoQueue_->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);    // ���[�j���O���Ɏ~�܂�
	}

	//�}������G���[
	D3D12_MESSAGE_ID denyIds[] = {
		D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
	};

	//�}�������\�����x��
	D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
	D3D12_INFO_QUEUE_FILTER filter{};
	filter.DenyList.NumIDs = _countof(denyIds);
	filter.DenyList.pIDList = denyIds;
	filter.DenyList.NumSeverities = _countof(severities);
	filter.DenyList.pSeverityList = severities;
	//�w�肵���G���[�̕\����}������
	infoQueue_->PushStorageFilter(&filter);
}

void InitDirectX::Commands(void)
{
#pragma region �R�}���h�A���P�[�^
	// �R�}���h�A���P�[�^�𐶐�
	HRESULT r = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator_));

#ifdef _DEBUG
	assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

#pragma region �R�}���h���X�g�̐���
	// �R�}���h���X�g�𐶐�
	r = device_->CreateCommandList(0,D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));

#ifdef _DEBUG
	assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

#pragma region �R�}���h�L���[�̐���
	//�R�}���h�L���[�̐ݒ�
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

	//�R�}���h�L���[�𐶐�
	r = device_->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue_));

#ifdef _DEBUG
	assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion
}

void InitDirectX::SwapChain(void)
{
	Window* wnd = Window::GetInstance();

#pragma region �X���b�v�`�F�[���̐���
	// �X���b�v�`�F�[���̐ݒ�
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = 1280;
	swapChainDesc.Height = 720;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �F���̏���
	swapChainDesc.SampleDesc.Count = 1; // �}���`�T���v�����Ȃ�
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // �o�b�N�o�b�t�@�p
	swapChainDesc.BufferCount = 2; // �o�b�t�@�����Q�ɐݒ�
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // �t���b�v��͔j��
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// IDXGISwapChain1��tmp
	ComPtr<IDXGISwapChain1> swapChain1Tmp;

	// �X���b�v�`�F�[���̐���
	HRESULT r = dxgiFactory_->CreateSwapChainForHwnd(commandQueue_.Get(), wnd->GetHwnd(), &swapChainDesc, nullptr, nullptr, &swapChain1Tmp);

#ifdef _DEBUG
	assert(SUCCEEDED(r));
#endif // _DEBUG

	// ��������IDXGISwapChain1�̃I�u�W�F�N�g��IDXGISwapChain4�ɕϊ�����
	swapChain1Tmp.As(&swapChain_);
#pragma endregion
}

void InitDirectX::RTVDescHeap(void)
{
	// swapChainDesc1 �� ��ϐ���錾
	DXGI_SWAP_CHAIN_DESC swcd{};

	// �R�s�[�Ŏ擾
	HRESULT r = swapChain_->GetDesc(&swcd);

#ifdef _DEBUG
	assert(SUCCEEDED(r));
#endif // _DEBUG

#pragma region RTV�̃f�X�N���v�^�q�[�v
	// �f�X�N���v�^�q�[�v�̐ݒ�
	rtvDescHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // �����_�[�^�[�Q�b�g�r���[
	rtvDescHeapDesc_.NumDescriptors = swcd.BufferCount; // ���\�̂Q��

	// �ݒ������RTV�p�f�X�N���v�^�q�[�v�̐���
	device_->CreateDescriptorHeap(&rtvDescHeapDesc_, IID_PPV_ARGS(&rtvHeap_));
#pragma endregion
}

void InitDirectX::RTV(void)
{
	// swapChainDesc1 �� ��ϐ���錾
	DXGI_SWAP_CHAIN_DESC swcd{};

	// �R�s�[�Ŏ擾
	HRESULT r = swapChain_->GetDesc(&swcd);

#ifdef _DEBUG
	assert(SUCCEEDED(r));
#endif // _DEBUG

	backBuffers_.resize(swcd.BufferCount);

	// �X���b�v�`�F�[���̑S�Ẵo�b�t�@�ɂ��ď�������
	for (size_t i = 0; i < backBuffers_.size(); i++) {
		// �X���b�v�`�F�[������o�b�t�@���擾
		swapChain_->GetBuffer(static_cast<UINT>(i), IID_PPV_ARGS(&backBuffers_.at(i)));
		// �f�X�N���v�^�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
		// �����\���ŃA�h���X�������
		rtvHandle.ptr += i * device_->GetDescriptorHandleIncrementSize(rtvDescHeapDesc_.Type);
		// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		// �V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// �����_�[�^�[�Q�b�g�r���[�̐���
		device_->CreateRenderTargetView(backBuffers_.at(i).Get(), &rtvDesc, rtvHandle);
	}
#pragma endregion
}

void InitDirectX::DepthBuffer(void)
{
#pragma region �[�x�e�X�g�[�x�o�b�t�@
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = Window::width_;     // �����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.Height = Window::height_;   // �����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;   // �[�x�l�t�H�[�}�b�g
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;  // �f�v�X�X�e���V��

	// �[�x�l�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	// �[�x�l�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;      // �[�x�l1.0f�i�ő�l�j�ŃN���A
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT; // �[�x�l�t�H�[�}�b�g

	// �[�x�o�b�t�@�̐���
	// ���\�[�X����
	HRESULT r = device_->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,       // �[�x�l�������݂Ɏg�p
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff_)
	);

    depthBuff_->SetName(L"InitDirectXDepthBuffer");

#ifdef _DEBUG
	assert(SUCCEEDED(r));
#endif // _DEBUG

	// �[�x�r���[�p�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;     // �[�x�r���[��1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;      // �f�v�X�X�e���V���r���[
	// �ݒ�����ɐ[�x�r���[�p�f�X�N���v�^�q�[�v�̐���
	r = device_->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap_));

#ifdef _DEBUG
	assert(SUCCEEDED(r));
#endif // _DEBUG

	// �[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;     // �[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	// �[�x�r���[�̐���
	device_->CreateDepthStencilView(
		depthBuff_.Get(),
		&dsvDesc,
		dsvHeap_->GetCPUDescriptorHandleForHeapStart());
#pragma endregion
}

void InitDirectX::Fence(void)
{
#pragma region �t�F���X
	// �t�F���X�̐���
	HRESULT r = device_->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));

#ifdef _DEBUG
	assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion
}

void InitDirectX::ClearRTV(void)
{
	// �o�b�N�o�b�t�@�̔ԍ����擾�i2�Ȃ̂�0�Ԃ�1�ԁj
	UINT bbIndex = swapChain_->GetCurrentBackBufferIndex();

	// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * device_->GetDescriptorHandleIncrementSize(rtvDescHeapDesc_.Type);

	// ���F�ŃN���A����̂��ݒ�
	FLOAT clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // ���ۂ��F
	// RTV���N���A����֐����R�}���h���X�g�ɐς�
	commandList_->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

void InitDirectX::ClearDepthBuff(void)
{
	// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap_->GetCPUDescriptorHandleForHeapStart();

	// �[�x�o�b�t�@���N���A����֐����R�}���h���X�g�ɐς�
	commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

InitDirectX::~InitDirectX(void)
{
    // device�ȊO�����Z�b�g
    if (debugController_) debugController_.Reset();
    if (dxgiFactory_) dxgiFactory_.Reset();
    if (infoQueue_) infoQueue_.Reset();
    if (cmdAllocator_)cmdAllocator_.Reset();
    if (commandList_) commandList_.Reset();
    if (commandQueue_)commandQueue_.Reset();
    if (swapChain_)swapChain_.Reset();
    if (rtvHeap_) rtvHeap_.Reset();
    for (ComPtr<ID3D12Resource>& backBuffer : backBuffers_) {
        if (backBuffer) {
            backBuffer.Reset();
        }
    }
    //if (depthBuff_)depthBuff_.Reset();
    if (dsvHeap_)dsvHeap_.Reset();
    if (fence_)fence_.Reset();

    //ComPtr<ID3D12DebugDevice> debugInterface;

    //if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&debugInterface))))
    //{
    //    debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
    //}
}
