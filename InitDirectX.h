#pragma once
#include <vector>
#include <wrl.h>
#include "CtrlFPS.h"
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class InitDirectX
{
public: // �ÓI�֐�
	static InitDirectX* GetInstance(void);

public: // �֐�
	void Initialize(void);
	void PreDraw(void);
	void PostDraw(void);

	// getter
	ID3D12Device* GetDevice(void) { return device_.Get(); }
	ID3D12GraphicsCommandList* GetCommandList(void) { return commandList_.Get(); }
    size_t GetBackBufferCount() const { return backBuffers_.size(); }

private: // �ϐ�
	// �G�C���A�X�e���v���[�g
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#pragma region DX12�ϐ�
	// DebugLayer() �ŏ�����
	ComPtr<ID3D12Debug1> debugController_;

	// DXGIDevice() �ŏ�����
	ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;
	ComPtr<ID3D12Device> device_ = nullptr;

	// SuppressErrors() �ŏ�����
	ComPtr<ID3D12InfoQueue> infoQueue_;

	// Commands() �ŏ�����
	ComPtr<ID3D12CommandAllocator> cmdAllocator_ = nullptr;
	ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;

	// SwapChain() �ŏ�����
	ComPtr<IDXGISwapChain4> swapChain_ = nullptr;

	// RTVDescHeap() �ŏ�����
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc_{};
	ComPtr<ID3D12DescriptorHeap> rtvHeap_ = nullptr;

	// RTV() �ŏ�����
	std::vector<ComPtr<ID3D12Resource>> backBuffers_; // �o�b�N�o�b�t�@

	// DepthBuffer() �ŏ�����
	ComPtr<ID3D12Resource> depthBuff_ = nullptr;
	ComPtr<ID3D12DescriptorHeap> dsvHeap_ = nullptr;

	// Fence() �ŏ�����
	ComPtr<ID3D12Fence> fence_ = nullptr;
	UINT64 fenceVal_ = 0;
#pragma endregion

	// Initialize() �Ŏg�p
	CtrlFPS fpsCtrller_; // FPS�𐧌�

private: // �֐�
	void DebugLayer(void);
	void DXGIDevice(void);      // �A�_�v�^�̑I��/�f�o�C�X�̐���
	void SuppressErrors(void);  // �֐����Ŏw�肳�ꂽ�G���[�̕\����}��
	void Commands(void);        // �R�}���h�A���P�[�^/���X�g/�L���[�̐���
	void SwapChain(void);
	void RTVDescHeap(void);     // RTV�p�f�X�N���v�^�q�[�v�̐��� + �������邽�߂̐ݒ�i�f�X�N���v�^�j
	void RTV(void);             // RTV�̐��� + �������邽�߂̐ݒ�i�f�X�N���v�^�j
	void DepthBuffer(void);
	void Fence(void);

	void ClearRTV(void);        // RTV���N���A
	void ClearDepthBuff(void);  // �[�x�o�b�t�@���N���A

private: // singleton design
	InitDirectX(void) {};
	~InitDirectX(void);
	InitDirectX(const InitDirectX&) = delete;
	InitDirectX& operator=(const InitDirectX&) = delete;
};