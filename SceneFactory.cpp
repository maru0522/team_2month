#include "SceneFactory.h"

std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& sceneName)
{
    std::unique_ptr<BaseScene> newScene{ nullptr };

    if (sceneName == "TITLE") {
        newScene = std::make_unique<TitleScene>();
    }
    else if (sceneName == "GAMEPLAY") {
        newScene = std::make_unique<GameScene>();
    }

    return std::move(newScene);
}