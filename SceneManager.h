#pragma once
#include <memory>
#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include "SceneFactory.h"
#include "Util.h"
#include "sceenChenge.h"

using std::unique_ptr;

class SceneManager
{
public:
    SceneManager(void);
    ~SceneManager(void);

    std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName);
    void RequestChangeScene(unique_ptr<BaseScene>& nextScene);

    void Update(void);
    void Draw3d(void);
    void Draw2d(void);
    void Finalize(void);

    static bool isInsertOk_;
private:
    unique_ptr<BaseScene> currentScene_{ nullptr };
    unique_ptr<BaseScene> nextScene_{ nullptr };
    unique_ptr<AbstractSceneFactory> sceneFactory_{ std::make_unique<SceneFactory>() };

    unique_ptr<SceenChenge> insertAnimation_{ std::make_unique<SceenChenge>()};

};