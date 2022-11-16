#pragma once
#include <vector>
#include <DirectXMath.h>
#include <string>
#include "Texture.h"
#include <d3d12.h>

#pragma comment(lib,"d3d12.lib")

struct CBDataB0 {
    DirectX::XMMATRIX mat_;     // 3D変換行列
    //DirectX::XMFLOAT4 color_;   // 色（RGBA）
};

struct CBDataB1 {
    DirectX::XMFLOAT3 ambient;  // ambient
    float_t pad1;               // padding
    DirectX::XMFLOAT3 diffuse;  // diffuse
    float_t pad2;               // padding
    DirectX::XMFLOAT3 specular; // specular
    float_t alpha;              // alpha
};

struct VertexModel3d_st {
    DirectX::XMFLOAT3 pos_;     // xyz座標
    DirectX::XMFLOAT3 normal_;  // 法線ベクトル
    DirectX::XMFLOAT2 uv_;      // uv座標
};

struct MaterialModel3d_st {
    std::string name;            // マテリアルの名前
    DirectX::XMFLOAT3 ambient{};   // ambient（環境）影響度
    DirectX::XMFLOAT3 diffuse{};   // diffuse（広汎）影響度
    DirectX::XMFLOAT3 specular{};  // specular（鏡面反射）影響度
    float_t alpha{};               // アルファ
    Texture::TEXTURE_KEY texKey; // テクスチャファイル名

    MaterialModel3d_st() {
        ambient = { 0.3f,0.3f,0.3f };
        diffuse = { 0.0f,0.0f,0.0f };
        specular = { 0.0f,0.0f,0.0f };
        alpha = 1.0f;
    }
};