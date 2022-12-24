#pragma once
#include "BaseScene.h"
#include <string>
#include <memory>

class AbstractSceneFactory
{
public:
    virtual ~AbstractSceneFactory(void) = default;

    virtual std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName) = 0;
};

