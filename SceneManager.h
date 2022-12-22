#pragma once
#include <memory>
#include "BaseScene.h"

using std::unique_ptr;

class SceneManager
{
public:
    ~SceneManager(void);

    void SetNextScene(unique_ptr<BaseScene>& nextScene) { nextScene_ = std::move(nextScene); }

    void Update(void);
    void Draw3d(void);
    void Draw2d(void);
    void Finalize(void);

private:
    unique_ptr<BaseScene> currentScene_{ nullptr };
    unique_ptr<BaseScene> nextScene_{ nullptr };
};