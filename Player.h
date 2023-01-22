#pragma once
#include "Obj3d.h"
#include <memory>
#include "Camera.h"
#include <imgui.h>
#include "Sprite.h"

class Player
{
private: // 定義
    enum class State
    {
        DEFAULT,
        ROPE,
        TARZAN,
    };

public: // 定数
    static constexpr float speed_{ 0.4f };
    static constexpr float ropeSpeed_{ 0.2f };
    static constexpr float jumpPower_{ 1.2f }; // 2.4f -> 1.2f
    static constexpr float jumpAttenuation_{ 0.125f };
    static constexpr float gravity_{ 0.35f };
    static constexpr DirectX::XMFLOAT3 radius_{ 1.0f,1.0f,1.0f };

    // 関数
    Player(Camera* pCamera);

    void Update(void);
    void Draw3d(void);
    void Draw2d(void);
    void DrawImgui(const DirectX::XMFLOAT3& vel);

    const std::unique_ptr<Obj3d>& GetObject3d(void) const { return object_; }

    void SetPos(const DirectX::XMFLOAT3& pos) { object_->worldCoordinate_.position_ = pos; }
    void SetPosX(const float posX) { object_->worldCoordinate_.position_.x = posX; }
    void SetPosY(const float posY) { object_->worldCoordinate_.position_.y = posY; }
    void SetPosZ(const float posZ) { object_->worldCoordinate_.position_.z = posZ; }

    void SetJumpFlag(bool isJump) { isJump_ = isJump; }
    void SetFloatFlag(bool isFloat) { isFloat_ = isFloat; }
private:
    void Move(void);
    void Jump(void);
    void Controll(DirectX::XMFLOAT3& vel);
    void ControllState(void);
    void Collision(DirectX::XMFLOAT3& vel);

    void ControllKeyTimer(void);

    void SetState(State state) { state_ = state; }

private: // 変数
    std::unique_ptr<Obj3d> object_{};

    // じゃんぷ
    float jumpValue_{ 0.0f };

    bool isJump_{ false };
    bool isFloat_{ false };

    // 状態
    State state_{ State::DEFAULT };
    bool isUnderHook_{ false }; // UI表示のためのもの
    std::unique_ptr<Util::Timer> ropeKeyTimer_{ std::make_unique<Util::Timer>() };

    std::unique_ptr<Sprite> ropeUseKey_sprite_{ std::make_unique<Sprite>("Resources/Image/KEY_ENTER.png",CMode::PATH) };
    std::unique_ptr<Sprite> ropeUseKeyPress_sprite_{ std::make_unique<Sprite>("Resources/Image/KEY_ENTER_PRESS.png",CMode::PATH) };
};