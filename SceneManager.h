#pragma once
#include <memory>
#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include "SceneFactory.h"

using std::unique_ptr;

class SceneManager
{
public:
    ~SceneManager(void);

    std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName);
    void RequestChangeScene(unique_ptr<BaseScene>& nextScene) { nextScene_ = std::move(nextScene); }

    void Update(void);
    void Draw3d(void);
    void Draw2d(void);
    void Finalize(void);

private:
    unique_ptr<BaseScene> currentScene_{ nullptr };
    unique_ptr<BaseScene> nextScene_{ nullptr };
    unique_ptr<AbstractSceneFactory> sceneFactory_{ std::make_unique<SceneFactory>() };
};