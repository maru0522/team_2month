#pragma once
#include <memory>
#include "Camera.h"
#include "Sprite.h"
#include "Obj3d.h"
#include "BaseScene.h"
<<<<<<< HEAD
#include "Player.h"
#include "NormalBlock.h"
=======
>>>>>>> parent of 5499b30 (playerできた)

using std::unique_ptr;

class GameScene : public BaseScene
{
private:
    static constexpr float rangeIntoPlayer_{ 50.0f };

public: // �֐�
    void Initialize(SceneManager* sceneManager) override;
    void Update(void) override;
    void Draw3d(void) override;
    void Draw2d(void) override;
    void Finalize(void) override;

private:
<<<<<<< HEAD


private:
    unique_ptr<Camera> cameraT_{ std::make_unique<Camera>() };
    unique_ptr<Player> player_{ std::make_unique<Player>(cameraT_.get()) };

    unique_ptr<Sprite> reset_{ std::make_unique<Sprite>("Resources/Image/Reset.png",CMode::PATH) };
=======
    unique_ptr<Camera> cameraT_{};
    unique_ptr<Sprite> spriteT_{};
    unique_ptr<Obj3d> objT_{};
>>>>>>> parent of 5499b30 (playerできた)
};

