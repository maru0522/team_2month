#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <wrl.h>

class mInitDirectX
{
private: // 変数
	// エイリアステンプレート
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Device> device_ = nullptr;
	ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;
	ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	ComPtr<ID3D12CommandAllocator> cmdAllocator_ = nullptr;
	ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	ComPtr<ID3D12DescriptorHeap> rtvHeap_ = nullptr;

private:
	void Device(void);
	void Command(void);
	void SwapChain(void);
};