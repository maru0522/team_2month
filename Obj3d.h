#pragma once
#include "Model.h"
#include "WorldCoordinate.h"
#include "Camera.h"

class Obj3d
{
private: // ’è‹`
    using fsPath = std::experimental::filesystem::path;

public: // Ã“IŠÖ”
    static void PreDraw(void);

public: // ŠÖ”
    Obj3d(const fsPath& pathAndObjName);
    void Update(void);
    void Draw(void);


public: // •Ï”
    Camera* pCamera_{ nullptr };

    WorldCoordinate worldCoordinate_{ pCamera_ };

private: // •Ï”
    Model model_{};

    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandleCopy_;
};