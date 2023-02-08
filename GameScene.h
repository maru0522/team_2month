#pragma once
#include <memory>
#include "Camera.h"
#include "Sprite.h"
#include "Obj3d.h"
#include "BaseScene.h"
#include "Player.h"
#include "NormalBlock.h"

using std::unique_ptr;

class GameScene : public BaseScene
{
private:
    static constexpr float distancePlayerToCamera_{ 120.f };

public: // ä÷êî
    void Initialize(SceneManager* sceneManager) override;
    void Update(void) override;
    void Draw3d(void) override;
    void Draw2d(void) override;
    void Finalize(void) override;

private:
    unique_ptr<Camera> cameraT_{ std::make_unique<Camera>() };
    unique_ptr<Player> player_{ std::make_unique<Player>(cameraT_.get()) };

    unique_ptr<Sprite> reset_{ std::make_unique<Sprite>("Resources/Image/Reset.png",CMode::PATH) };
    unique_ptr<Sprite> bokashi_{ std::make_unique<Sprite>("Resources/Image/Game/bokashi.png",CMode::PATH) };
    unique_ptr<Sprite> wireString_{ std::make_unique<Sprite>("Resources/Image/Title/gameTitle.png",CMode::PATH) };
    std::array<unique_ptr<Sprite>, 5> wireArray_{};


    unique_ptr<Sprite> tutorial1_{ std::make_unique<Sprite>("Resources/Image/tutorial1.png",CMode::PATH) };
    unique_ptr<Sprite> tutorial2_{ std::make_unique<Sprite>("Resources/Image/tutorial2.png",CMode::PATH) };
    unique_ptr<Sprite> tutorial3_{ std::make_unique<Sprite>("Resources/Image/tutorial3.png",CMode::PATH) };
    // ÉJÉÅÉâmode - auto / controll
    DirectX::XMFLOAT3 oldCameraEye_{}, oldCameraTarget_{};
    bool isAutoCameraMode_{ true };

    float cameraSpeed = 1.2f;
    float cameraPosY = 0.0f;

    unique_ptr<Obj3d> skydome_{};
    unique_ptr<Obj3d> targetPoint_{};
};

