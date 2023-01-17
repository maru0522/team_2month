#pragma once
#include <memory>
#include "Camera.h"
#include "Sprite.h"
#include "Obj3d.h"
#include "BaseScene.h"
#include "Player.h"
#include "ExtendBlock.h"

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
    unique_ptr<Camera> cameraT_{ std::make_unique<Camera>() };
    unique_ptr<Player> player_{ std::make_unique<Player>(cameraT_.get()) };

    ExtendBlock* block;
};

