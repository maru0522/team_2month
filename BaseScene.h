#pragma once
#include <memory>

class SceneManager;

class BaseScene
{
public:
    virtual ~BaseScene(void) = default;

    virtual void Initialize(SceneManager* pSceneManager);
    virtual void Update(void) = 0;
    virtual void Draw3d(void) = 0;
    virtual void Draw2d(void) = 0;
    virtual void Finalize(void) = 0;

protected:
    SceneManager* sceneManager_{ nullptr };
};

