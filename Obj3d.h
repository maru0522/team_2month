#pragma once
#include "Model.h"
#include "WorldCoordinate.h"
#include "Camera.h"
#include "GraphicsPipeline.h"

class Obj3d
{
private: // ’è‹`
    using fsPath = std::experimental::filesystem::path;

public: // Ã“IŠÖ”
    static void Initialize(void);
    static void PreDraw(void);

public: // Ã“I•Ï”
    static GraphicsPipeline* graphicsPipeline_;

public: // ŠÖ”
    Obj3d(const fsPath& pathAndObjName);
    Obj3d(const fsPath& pathAndObjName, Camera* pCamera);
    void Update(void);
    void Draw(void);


public: // •Ï”
    Camera* pCamera_{ nullptr };

    WorldCoordinate worldCoordinate_{ pCamera_ };

private: // •Ï”
    Model model_{};

    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandleCopy_;
};