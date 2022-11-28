#pragma once
#include "Model.h"
#include "WorldCoordinate.h"
#include "Camera.h"
#include "GraphicsPipeline.h"

class Obj3d
{
private: // 定義
    using fsPath = std::experimental::filesystem::path;

public: // 静的関数
    static void PreDraw(void);

public: // 静的変数
    static GraphicsPipeline* graphicsPipeline_;

public: // 関数
    Obj3d(void); // 配列用
    Obj3d(const fsPath& pathAndObjName);
    Obj3d(const fsPath& pathAndObjName, Camera* pCamera);
    void Update(void);
    void Draw(void);

    void SetCamera(Camera* pCamera) { worldCoordinate_.SetCamera(pCamera); }

    // Texture::Load("パス")で読み込んだ"パス"を書き込む
    void SetTexture(const fsPath& pathAndFileName);
    void SetTextureById(const std::string& id);

public: // 変数
    WorldCoordinate worldCoordinate_{};

private: // 変数
    Model model_{};

    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandleCopy_;
};