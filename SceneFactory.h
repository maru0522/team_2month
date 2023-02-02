#pragma once
#include "AbstractSceneFactory.h"
#include <memory>

class SceneFactory :
    public AbstractSceneFactory
{
public:
    std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName) override;
};