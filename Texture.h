#pragma once
#include <string>
#include <wrl.h>
#include <d3d12.h>
#include <array>
#include <map>
#include <tuple>

class Texture
{
public: // 静的関数
    // mapの鍵と値
    using MAP_KEY = std::string;
    using MAP_VALUE = std::tuple < Microsoft::WRL::ComPtr<ID3D12Resource>, D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE >;

    // srvの初期化
    static void Initialize(void);

    // 読込
    static void Load(const std::string& relativePath, const std::string& fileName);
    static void Load(const std::string& pathAndFileName);

    // 読込済のテクスチャを検索し返す
    static const Texture& GetTexture(const std::string& relativePath, const std::string& fileName);
    static const Texture& GetTexture(const std::string& pathAndFileName);

    // 読込済のテクスチャの情報だけ返す
    static const MAP_VALUE& GetTextureInfo(const std::string& relativePath, const std::string& fileName);
    static const MAP_VALUE& GetTextureInfo(const std::string& pathAndFileName);

    static const D3D12_GPU_DESCRIPTOR_HANDLE& GetTexSRVGpuH(const std::string& pathAndFileName);

    // private変数: srvHeap を取得
    static ID3D12DescriptorHeap* GetSRVHeap(void) { return srvHeap_.Get(); }

private: // 静的変数
    // エイリアステンプレート
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;

    static constexpr size_t maxSRVDesc_{ 256 }; // テクスチャの最大読込数

    static std::map<MAP_KEY, MAP_VALUE> textures_; // SRVを入れていく
    static uint32_t indexNextDescHeap_; // 次に情報を入れるDescHeapの番号を指し示す

private: // 変数
    // 名前
    MAP_KEY name_{};
    MAP_VALUE info_{};

    //================= MAP_VALUE =================//
    // ComPtr<ID3D12Resource> buffer_;             //
    // D3D12_CPU_DESCRIPTOR_HANDLE srvCPUHandle_;  //
    // D3D12_GPU_DESCRIPTOR_HANDLE srvGPUHandle_;  //
    //=============================================//

private: // 関数
    void SetMapKey(std::string keyName);
};