#pragma once
#include <string>
#include <wrl.h>
#include <d3d12.h>
#include <array>
#include <map>
#include <stdexcept>
#if _MSC_VER > 1922 && !defined(_SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING)
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#endif
#include <experimental/filesystem>

class Texture
{
public: // 定義
    // mapの鍵と値
    using TEXTURE_KEY = std::experimental::filesystem::path;
    struct TEXTURE_VALUE
    {
        Microsoft::WRL::ComPtr<ID3D12Resource> buff_{ nullptr };
        D3D12_CPU_DESCRIPTOR_HANDLE srvCpuHandle_{};
        D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle_{};
    };
    // mapの鍵のID
    using TEX_KEY_ID = std::string;

private: // 定義
    using fsPath = std::experimental::filesystem::path;

public: // 静的関数

    // srvの初期化
    static void Initialize(void);

    // 読込 
    static void Load(const fsPath& pathAndFileName);
    static void Load(const fsPath& pathAndFileName, const std::string& id);

    static const bool ExistTexture(const std::string& path);

    // 読込済のテクスチャの情報だけ返す
    static const TEXTURE_VALUE* GetTextureInfo(const fsPath& pathAndFileName);
    static const TEXTURE_VALUE* GetTextureInfoById(const std::string& id); // 関数名変えたくなかったけど回避案思い浮かばんかった。

    // テクスチャのパスに名称を設定して呼び出しを簡略化。※あだ名付けてやるくらいの感覚でいい
    static void SetId(const fsPath& pathAndFileName, const std::string& id);

    // 設定したIDからMAP_KEYを取得する。publicにしてはいるが基本関数内で使う
    static const TEXTURE_KEY* GetTextureKey(const std::string& id);

    // private変数: srvHeap を取得
    static ID3D12DescriptorHeap* GetSRVHeap(void) { return srvHeap_.Get(); }

    // 解放
    static void Eject(const fsPath& pathAndFileName);

    // ID削除
    static void DeleteId(const std::string& id);

private:
    static void GenerateMissingTexture(void);

private: // 静的変数
    // エイリアステンプレート
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;

    static constexpr size_t maxSRVDesc_{ 256 }; // テクスチャの最大読込数

    static std::map<TEXTURE_KEY, TEXTURE_VALUE> textures_; // SRVDescとMAP_KEYを入れていく
    static std::map<TEX_KEY_ID, TEXTURE_KEY> mapKeys_; // MAP_KEYとIDを紐づけて入れていく
    static uint32_t indexNextDescHeap_; // 次に情報を入れるDescHeapの番号を指し示す

private: // 変数
    // 名前
    TEXTURE_KEY name_{};
    TEXTURE_VALUE info_{};

    //=============== TEXTURE_VALUE ==============//
    // ComPtr<ID3D12Resource> buffer_;            //
    // D3D12_CPU_DESCRIPTOR_HANDLE srvCPUHandle_; //
    // D3D12_GPU_DESCRIPTOR_HANDLE srvGPUHandle_; //
    //============================================//

private: // 関数
    void SetMapKey(const fsPath& keyName);
};