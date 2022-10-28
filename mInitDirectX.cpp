#include "mInitDirectX.h"
#include <cassert>
#include <vector>
#include "mWindow.h"

void mInitDirectX::Device(void)
{
#pragma region アダプタの列挙
	// DXGIファクトリーの生成
	HRESULT r = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));

#ifdef _DEBUG
	assert(SUCCEEDED(r));
#endif // _DEBUG

	// アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter4>> adapters;

	// ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter4> tmpAdapter = nullptr;

	// パフォーマンスが高いものから順に、全てのアダプターを列挙する
	for (UINT i = 0;
		dxgiFactory_->EnumAdapterByGpuPreference( // ここの引数がUINTのため i の型がsize_tでない
			i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++) {

		// 動的配列に追加する
		adapters.push_back(tmpAdapter);
	}
#pragma endregion

#pragma region アダプタの選別
	// 妥当なアダプタを選別する
	for (size_t i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;

		// アダプターの情報を取得する
		adapters[i]->GetDesc3(&adapterDesc);

		// ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// デバイスを採用してループを抜ける
			tmpAdapter = adapters.at(i);

			break;
		}
	}
#pragma endregion

#pragma region デバイスの生成
	// 対応レベルの配列
	std::vector<D3D_FEATURE_LEVEL> levels{
	 D3D_FEATURE_LEVEL_12_1,
	 D3D_FEATURE_LEVEL_12_0,
	 D3D_FEATURE_LEVEL_11_1,
	 D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;
	for (size_t i = 0; i < levels.size(); i++) {
		// 採用したアダプターでデバイスを生成
		r = D3D12CreateDevice(tmpAdapter.Get(), levels.at(i),
			IID_PPV_ARGS(&device_));

		if (r == S_OK) {
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels.at(i);
			break;
		}
	}
#pragma endregion
}

void mInitDirectX::Command(void)
{
#pragma region コマンドアロケータ
	// コマンドアロケータを生成
 	HRESULT r = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator_));

#ifdef _DEBUG
	assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

#pragma region コマンドリストの生成
	// コマンドリストを生成
	r = device_->CreateCommandList(0,D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));

#ifdef _DEBUG
	assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

#pragma region コマンドキューの生成
	//コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

	//コマンドキューを生成
	r = device_->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue_));

#ifdef _DEBUG
	assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion
}

void mInitDirectX::SwapChain(void)
{
	mWindow* wnd = mWindow::GetInstance();

#pragma region スワップチェーンの生成
	// スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = 1280;
	swapChainDesc.Height = 720;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 色情報の書式
	swapChainDesc.SampleDesc.Count = 1; // マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // バックバッファ用
	swapChainDesc.BufferCount = 2; // バッファ数を２つに設定
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // フリップ後は破棄
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// IDXGISwapChain1のtmp
	ComPtr<IDXGISwapChain1> swapChain1Tmp;

	// スワップチェーンの生成
	HRESULT r = dxgiFactory_->CreateSwapChainForHwnd(commandQueue_.Get(), wnd->GetHwnd(), &swapChainDesc, nullptr, nullptr, &swapChain1Tmp);

#ifdef _DEBUG
	assert(SUCCEEDED(r));
#endif // _DEBUG

	// 生成したIDXGISwapChain1のオブジェクトをIDXGISwapChain4に変換する
	swapChain1Tmp.As(&swapChain_);
#pragma endregion
}