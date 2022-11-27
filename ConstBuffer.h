#pragma once
#include <Windows.h>
#include "InitDirectX.h"
#include <cassert>
#include <algorithm>
#include <wrl.h>
#include <DirectXMath.h>
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")

template<typename CBType> class ConstBuffer
{
public: // 関数
    ConstBuffer(void) {
        InitDirectX* iDX = InitDirectX::GetInstance();

        // 定数バッファ生成用の設定
        // ヒープ設定
        D3D12_HEAP_PROPERTIES cbHeapProp{};
        cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;               // GPUへの転送用
        // リソース設定
        D3D12_RESOURCE_DESC cbResourceDesc{};
        cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        cbResourceDesc.Width = (sizeof(CBType) + 0xff) & ~0xff; //256バイトアラインメント
        cbResourceDesc.Height = 1;
        cbResourceDesc.DepthOrArraySize = 1;
        cbResourceDesc.MipLevels = 1;
        cbResourceDesc.SampleDesc.Count = 1;
        cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        // 定数バッファの生成
        HRESULT r = iDX->GetDevice()->CreateCommittedResource(
            &cbHeapProp,    // ヒープ設定
            D3D12_HEAP_FLAG_NONE,
            &cbResourceDesc,    // リソース設定
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&buffer_));
        assert(SUCCEEDED(r));

        // 定数バッファのマッピング
        CBMap();
        //SetColorは使用先に委任
    }

    ~ConstBuffer(void) {
    }

    // private変数: cBTypeMap_ を取得
    CBType* GetConstBuffMap(void) { return cbTypeMap_; } // 値書き換え用取得（マテリアルカラーのセッティングとか）

    // private変数: cBMaterial を取得
    ID3D12Resource* GetBuffer(void) { return buffer_.Get(); }

private: // 変数
    // エイリアステンプレート
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    ComPtr<ID3D12Resource> buffer_{ nullptr };

    CBType* cbTypeMap_{ nullptr };

private: // 関数
    // cBMaterialとcMapMaterialのマッピング
    void CBMap(void) {
        // 定数バッファのマッピング
        HRESULT r = buffer_->Map(0, nullptr, (void**)&cbTypeMap_);       // マッピング
        assert(SUCCEEDED(r));
    }

    // 途中で色を変化させる予定がないなら初期化後に使う
    // マッピングの解除
    void CBUnMap(void) {
        buffer_->Unmap(0, nullptr);
    }
};