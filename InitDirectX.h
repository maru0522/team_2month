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
public: // 静的関数
	static InitDirectX* GetInstance(void);

public: // 関数
	void Initialize(void);
	void PreDraw(void);
	void PostDraw(void);

	// getter
	ID3D12Device* GetDevice(void) { return device_.Get(); }
	ID3D12GraphicsCommandList* GetCommandList(void) { return commandList_.Get(); }
    size_t GetBackBufferCount() const { return backBuffers_.size(); }

private: // 変数
	// エイリアステンプレート
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#pragma region DX12変数
	// DebugLayer() で初期化
	ComPtr<ID3D12Debug1> debugController_;

	// DXGIDevice() で初期化
	ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;
	ComPtr<ID3D12Device> device_ = nullptr;

	// SuppressErrors() で初期化
	ComPtr<ID3D12InfoQueue> infoQueue_;

	// Commands() で初期化
	ComPtr<ID3D12CommandAllocator> cmdAllocator_ = nullptr;
	ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;

	// SwapChain() で初期化
	ComPtr<IDXGISwapChain4> swapChain_ = nullptr;

	// RTVDescHeap() で初期化
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc_{};
	ComPtr<ID3D12DescriptorHeap> rtvHeap_ = nullptr;

	// RTV() で初期化
	std::vector<ComPtr<ID3D12Resource>> backBuffers_; // バックバッファ

	// DepthBuffer() で初期化
	ComPtr<ID3D12Resource> depthBuff_ = nullptr;
	ComPtr<ID3D12DescriptorHeap> dsvHeap_ = nullptr;

	// Fence() で初期化
	ComPtr<ID3D12Fence> fence_ = nullptr;
	UINT64 fenceVal_ = 0;
#pragma endregion

	// Initialize() で使用
	CtrlFPS fpsCtrller_; // FPSを制御

private: // 関数
	void DebugLayer(void);
	void DXGIDevice(void);      // アダプタの選別/デバイスの生成
	void SuppressErrors(void);  // 関数内で指定されたエラーの表示を抑制
	void Commands(void);        // コマンドアロケータ/リスト/キューの生成
	void SwapChain(void);
	void RTVDescHeap(void);     // RTV用デスクリプタヒープの生成 + 生成するための設定（デスクリプタ）
	void RTV(void);             // RTVの生成 + 生成するための設定（デスクリプタ）
	void DepthBuffer(void);
	void Fence(void);

	void ClearRTV(void);        // RTVをクリア
	void ClearDepthBuff(void);  // 深度バッファをクリア

private: // singleton design
	InitDirectX(void) {};
	~InitDirectX(void) {};
	InitDirectX(const InitDirectX&) = delete;
	InitDirectX& operator=(const InitDirectX&) = delete;
};