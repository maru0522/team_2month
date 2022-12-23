#pragma once
#include <memory>
#include "Camera.h"
#include "Sprite.h"
#include "Obj3d.h"
#include "BaseScene.h"

using std::unique_ptr;

class GameScene : public BaseScene
{
public: // ä÷êî
    void Initialize(SceneManager* sceneManager) override;
    void Update(void) override;
    void Draw3d(void) override;
    void Draw2d(void) override;
    void Finalize(void) override;

private:
    unique_ptr<Camera> cameraT_{};
    unique_ptr<Sprite> spriteT_{};
    unique_ptr<Obj3d> objT_{};
};

