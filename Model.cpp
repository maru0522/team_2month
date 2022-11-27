#include "Model.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>
#include <DirectXMath.h>
#include "InitDirectX.h"
#include "Texture.h"
#include "GraphicsPipeline.h"
#include "Util.h"
#include<d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

// 静的変数の実体
std::map<Model::MODEL_KEY, Model::MODEL_VALUE> Model::models_{};

void Model::Load(const fsPath& pathAndObjName)
{
    const std::string path{ pathAndObjName.parent_path().string() + "/" };
    const std::string objName{ pathAndObjName.filename().string() };

    // てんぽらりん
    Model tmp{};

#ifdef _DEBUG
    // 既に読み込んだテクスチャとの重複確認。
    if (models_.count(pathAndObjName)) {
        OutputDebugString(L"A .obj file with the same name was loaded. The model may have been overwritten.");
    }
#endif // _DEBUG

    tmp.name_ = pathAndObjName;

#pragma region 頂点とインデックス座標
    // ファイルストリーム
    std::ifstream file;
    // .objを開く
    file.open(pathAndObjName);
    // ファイルオープン失敗をチェック
    assert(!file.fail());

    std::vector<DirectX::XMFLOAT3> positions{};   // 頂点座標
    std::vector<DirectX::XMFLOAT3> normals{};     // 法線ベクトル
    std::vector<DirectX::XMFLOAT2> texcoords{};   // テクスチャUV
    // 1行ずつ読み込む
    std::string line;
    while (std::getline(file, line)) {
        // 1行分の文字列をストリームに変換
        std::istringstream line_stream(line);

        // 半角スペース区切りで行の先頭文字列を取得
        std::string key;
        std::getline(line_stream, key, ' ');

        if (key == "mtllib") {
            // マテリアルのファイル名読み込み
            std::string mtlFileName;
            line_stream >> mtlFileName;
            // マテリアル読み込み
            LoadMaterial(tmp, pathAndObjName);
        }

        // 先頭文字列がvなら頂点座標
        if (key == "v") {
            // X, Y, Z座標読み込み
            DirectX::XMFLOAT3 position{};
            line_stream >> position.x;
            line_stream >> position.y;
            line_stream >> position.z;
            // 座標データに追加
            positions.emplace_back(position);
        }

        // 先頭文字列がvtならテクスチャ
        if (key == "vt") {
            // UV成分を読み込み
            DirectX::XMFLOAT2 texcoord{};
            line_stream >> texcoord.x;
            line_stream >> texcoord.y;
            // V方向反転
            texcoord.y = 1.0f - texcoord.y;
            // テクスチャ座標データに追加
            texcoords.emplace_back(texcoord);
        }

        // 先頭文字列がvnなら法線ベクトル
        if (key == "vn") {
            // XYZ成分を読み込み
            DirectX::XMFLOAT3 normal{};
            line_stream >> normal.x;
            line_stream >> normal.y;
            line_stream >> normal.z;
            // 法線ベクトルデータに追加
            normals.emplace_back(normal);
        }

        if (key == "f") {
            // 半角スペース区切りで行の続きを読み込む
            std::string index_string;
            while (std::getline(line_stream, index_string, ' ')) {
                //頂点インデックス1個分の文字列をストリームに変換
                std::istringstream index_stream(index_string);
                uint16_t indexPosition{};
                uint16_t indexNormal{};
                uint16_t indexTexcoord{};
                index_stream >> indexPosition;
                index_stream.seekg(1, std::ios_base::cur); // スラッシュを飛ばす
                index_stream >> indexTexcoord;
                index_stream.seekg(1, std::ios_base::cur); // スラッシュを飛ばす
                index_stream >> indexNormal;
                // 頂点データの追加
                VertexModel3d_st vertex{};
                vertex.pos_ = positions[indexPosition - 1];
                vertex.normal_ = normals[indexNormal - 1];
                vertex.uv_ = texcoords[indexTexcoord - 1];
                tmp.info_.vertices_.emplace_back(vertex);
                // 頂点インデックスデータの追加
                tmp.info_.indices_.emplace_back((uint16_t)tmp.info_.indices_.size());
            }
        }
    }
#pragma endregion

#pragma region 頂点バッファの生成
    // 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
    uint32_t sizeVB = static_cast<uint32_t>(sizeof(VertexModel3d_st) * tmp.info_.vertices_.size());

    // 頂点バッファの設定
    D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

    // リソース設定
    D3D12_RESOURCE_DESC resDesc{};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeVB; // 頂点データ全体のサイズ
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    InitDirectX* iDX = InitDirectX::GetInstance();

    // 頂点バッファの生成
    HRESULT r = iDX->GetDevice()->CreateCommittedResource(
        &heapProp, // ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &resDesc, // リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&tmp.info_.vertBuff_));
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

#pragma region 頂点バッファのマッピング
    // GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
    VertexModel3d_st* vertMap{ nullptr };
    r = tmp.info_.vertBuff_->Map(0, nullptr, (void**)&vertMap);
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
    // 全頂点に対して
    std::copy(tmp.info_.vertices_.begin(), tmp.info_.vertices_.end(), vertMap);

    // 繋がりを解除
    tmp.info_.vertBuff_->Unmap(0, nullptr);
#pragma endregion

#pragma region 頂点バッファのビュー設定
    // vbview
    // GPU仮想アドレス
    tmp.info_.vbView_.BufferLocation = tmp.info_.vertBuff_->GetGPUVirtualAddress();
    // 頂点バッファのサイズ
    tmp.info_.vbView_.SizeInBytes = sizeVB;
    // 頂点1つ分のデータサイズ
    tmp.info_.vbView_.StrideInBytes = sizeof(VertexModel3d_st);
#pragma endregion

    //SetColor();

    //SetCBTransform();

#pragma region インデックスバッファの生成
    // インデックスデータ全体のサイズ
    uint32_t sizeIB = static_cast<uint32_t>(sizeof(uint16_t) * tmp.info_.indices_.size());

    // リソース設定
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeIB;	// インデックス情報が入る分のサイズ
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // インデックスバッファの生成
    r = iDX->GetDevice()->CreateCommittedResource(
        &heapProp,	// ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &resDesc,	// リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&tmp.info_.indexBuff_));
#pragma endregion

#pragma region インデックスバッファのマッピング
    // インデックスバッファをマッピング
    uint16_t* indexMap = nullptr;
    r = tmp.info_.indexBuff_->Map(0, nullptr, (void**)&indexMap);
    // 全インデックスに対して
    std::copy(tmp.info_.indices_.begin(), tmp.info_.indices_.end(), indexMap);

    // マッピング解除
    tmp.info_.indexBuff_->Unmap(0, nullptr);
#pragma endregion

#pragma region インデックスバッファのビュー設定
    // インデックスバッファビューの作成
    tmp.info_.ibView_.BufferLocation = tmp.info_.indexBuff_->GetGPUVirtualAddress();
    tmp.info_.ibView_.Format = DXGI_FORMAT_R16_UINT;
    tmp.info_.ibView_.SizeInBytes = sizeIB;
#pragma endregion

    // mapへの挿入（or 代入）※代入の場合、同一KEYに対してVALUEが上書きされるため注意
    models_.insert_or_assign(tmp.name_, tmp.info_); // 代入時であっても全く同じVALUEが入るとは思われる。
}

const Model::MODEL_VALUE* Model::GetMODEL_VALUE(const fsPath& pathAndObjName)
{
    return &models_.at(pathAndObjName);
}

void Model::LoadMaterial(Model& model, const fsPath& pathAndObjName)
{
    const std::string path{ pathAndObjName.parent_path().string() + "/" };
    const std::string mtlname{ pathAndObjName.stem().string() + ".mtl" };

    //const std::string a{ "Resources/3dModels/cube/untitled.mtl" };

    // ファイルストリーム
    std::ifstream file;
    // マテリアルファイルを開く
    file.open(path + mtlname);
    // ファイルオープン失敗をチェック
    if (file.fail()) assert(0);

    // 1行ずつ読み込む
    std::string line;
    while (std::getline(file, line)) {
        // 1行分の文字列をストリームに変換
        std::istringstream line_stream(line);

        // 半角スペース区切りで行の先頭文字列を取得
        std::string key;
        std::getline(line_stream, key, ' ');

        // 先頭のタブ文字は無視する
        if (key[0] == '\t') {
            key.erase(key.begin()); // 先頭の文字を削除
        }

        // 先頭文字列がnewmtlならマテリアル名
        if (key == "newmtl") {
            // マテリアル名読み込み
            line_stream >> model.info_.material_.name;
        }

        // 先頭文字列がKaならアンビエント色
        if (key == "Ka") {
            line_stream >> model.info_.material_.ambient.x;
            line_stream >> model.info_.material_.ambient.y;
            line_stream >> model.info_.material_.ambient.z;
        }

        // 先頭文字列ならKdならディフューズ色
        if (key == "kd") {
            line_stream >> model.info_.material_.diffuse.x;
            line_stream >> model.info_.material_.diffuse.y;
            line_stream >> model.info_.material_.diffuse.z;
        }

        // 先頭文字列がKsならスペキュラー色
        if (key == "Ks") {
            line_stream >> model.info_.material_.specular.x;
            line_stream >> model.info_.material_.specular.y;
            line_stream >> model.info_.material_.specular.z;
        }

        // 先頭文字列がmap_Kdならテクスチャファイル名
        if (key == "map_Kd") {
            Texture::TEXTURE_KEY tmpName;

            // テクスチャのファイル名読み込み
            line_stream >> tmpName;
            model.info_.material_.texKey = pathAndObjName.parent_path() / tmpName;

            // テクスチャ読み込み
            Texture::Load(model.info_.material_.texKey);
        }
    }
}

void Model::Update(void)
{
    UpdateMaterial();
}

void Model::UpdateMaterial(void)
{
    info_.constBuffB1_.GetConstBuffMap()->ambient = info_.material_.ambient;
    info_.constBuffB1_.GetConstBuffMap()->diffuse = info_.material_.diffuse;
    info_.constBuffB1_.GetConstBuffMap()->specular = info_.material_.specular;
    info_.constBuffB1_.GetConstBuffMap()->alpha = info_.material_.alpha;
}

//void Model::PreDraw(void)
//{
//    // インスタンス取得
//    InitDirectX* iDX = InitDirectX::GetInstance();
//
//    // パイプラインステートとルートシグネチャの設定コマンド
//    iDX->GetCommandList()->SetPipelineState(graphicsPipeline_.GetPipelineState());
//    iDX->GetCommandList()->SetGraphicsRootSignature(graphicsPipeline_.GetRootSignature());
//
//    // プリミティブ形状の設定コマンド
//    iDX->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト
//
//    ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetSRVHeap() };
//    // SRVヒープの設定コマンド
//    iDX->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
//}

//void Model::Draw(void)
//{
//    // インスタンス取得
//    InitDirectX* iDX = InitDirectX::GetInstance();
//
//    SetCommandsBeforeDraw();
//
//#pragma region 頂点バッファ
//    // 頂点バッファの設定
//    iDX->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
//#pragma endregion
//
//#pragma region インデックスバッファ
//    // インデックスバッファの設定
//    iDX->GetCommandList()->IASetIndexBuffer(&ibView_);
//#pragma endregion
//
//#pragma region ConstBuffer
//    // 定数バッファビュー（CBV）の設定コマンド
//    //iDX->GetCommandList()->SetGraphicsRootConstantBufferView(0, cbMaterial_.GetBuffer()->GetGPUVirtualAddress());
//    iDX->GetCommandList()->SetGraphicsRootConstantBufferView(0, worldCoordinate_.GetCBData()->GetBuffer()->GetGPUVirtualAddress());
//    //iDX->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbTransform_.GetBuffer()->GetGPUVirtualAddress());
//    iDX->GetCommandList()->SetGraphicsRootConstantBufferView(1, constBuffB1_.GetBuffer()->GetGPUVirtualAddress());
//#pragma endregion
//
//#pragma region SRV
//    //指定のヒープにあるSRVをルートパラメータ1番に設定
//    iDX->GetCommandList()->SetGraphicsRootDescriptorTable(2, srvGpuHandleCopy_);
//#pragma endregion
//
//    // 描画コマンドリスト
//    iDX->GetCommandList()->DrawIndexedInstanced(static_cast<uint32_t>(indices_.size()), 1, 0, 0, 0);
//}
//
//
//void Model::SetCommandsBeforeDraw(void)
//{
//    // インスタンス取得
//    InitDirectX* iDX = InitDirectX::GetInstance();
//
//    // パイプラインステートとルートシグネチャの設定コマンド
//    iDX->GetCommandList()->SetPipelineState(graphicsPipeline_.GetPipelineState());
//    iDX->GetCommandList()->SetGraphicsRootSignature(graphicsPipeline_.GetRootSignature());
//
//    // プリミティブ形状の設定コマンド
//    iDX->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト
//
//    ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetSRVHeap() };
//    // SRVヒープの設定コマンド
//    iDX->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
//}