#pragma once
#include "IBlock.h"
#include "Camera.h"

class Stage
{
private:
    using fsPath = std::experimental::filesystem::path;

public:
    static void LoadCsv(Camera* pCamera, const fsPath& path);
};

