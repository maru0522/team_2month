#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "StageSelectScene.h"
#include "TutorialScene.h"

std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& sceneName)
{
    std::unique_ptr<BaseScene> newScene{ nullptr };

    if (sceneName == "TITLE") {
        newScene = std::make_unique<TitleScene>();
    }
    else if (sceneName == "STAGESELECT") {
        newScene = std::make_unique<StageSelectScene>();
    }
    else if (sceneName == "TUTORIAL") {
        newScene = std::make_unique<TutorialScene>();
    }
    else if (sceneName == "GAMEPLAY") {
        newScene = std::make_unique<GameScene>();
    }

    return std::move(newScene);
}