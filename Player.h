#pragma once
#include "Obj3d.h"
#include <memory>
#include "Camera.h"
#include <imgui.h>
#include "Sprite.h"
#include "IBlock.h"

class Player
{
private: // 定義
    enum class MoveState
    {
        DEFAULT,
        ROPE,
        TARZAN,

#ifdef _DEBUG
        S_DEBUG,
#endif // _DEBUG
    };

public: // 定数
    static constexpr float speed_{ 0.3f };
    static constexpr float ropeSpeed_{ 0.2f };
    static constexpr float jumpPower_{ 2.7f }; // 2.4f -> 1.2f
    static constexpr float jumpAttenuation_{ 0.23f };
    static constexpr float gravity_{ 0.75f };
    static constexpr DirectX::XMFLOAT3 radius_{ 1.0f,1.0f,1.0f };

    static constexpr float epsilon_{ 0.1f };

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

private:
    void Move(void);
    void Jump(void);
    void Controll(DirectX::XMFLOAT3& vel);
    void ControllState(void);
    void Collision(DirectX::XMFLOAT3& vel);

    void ControllKeyTimer(void);

    void SetState(MoveState state) { state_ = state; }

private: // 変数
    std::unique_ptr<Obj3d> object_{};

    Camera* cameraPtr_{ nullptr };

    // じゃんぷ
    float jumpValue_{ 0.0f };

    bool isJump_{ false };

    // 状態
    MoveState state_{ MoveState::DEFAULT };
    bool isUnderHook_{ false }; // UI表示のためのもの
    std::unique_ptr<Util::Timer> ropeKeyTimer_{ std::make_unique<Util::Timer>() };

    bool isNearSupply_{ false }; // 電源ブロックの近くにいるか
    bool isNearReceive_{ false }; // 受源ブロックの近くにいるか

    bool oldConnecting_{ false }; // 苦肉
    bool isConnecting_{ false }; // ワイヤーをつなげている最中かどうか


    std::unique_ptr<Sprite> ropeUsePad_sprite_{ std::make_unique<Sprite>("XPAD_RB",CMode::ID) };
    std::unique_ptr<Sprite> ropeUsePadPress_sprite_{ std::make_unique<Sprite>("XPAD_RB_PRESS",CMode::ID) };
    std::unique_ptr<Sprite> connectUsePad_sprite_{ std::make_unique<Sprite>("XPAD_X",CMode::ID) };
    std::unique_ptr<Sprite> connectUsePadPress_sprite_{ std::make_unique<Sprite>("XPAD_X_PRESS",CMode::ID) };

    std::unique_ptr<Sprite> default_sprite_{ std::make_unique<Sprite>("p_default",CMode::ID) };
    std::unique_ptr<Sprite> rope_sprite_{ std::make_unique<Sprite>("p_rope",CMode::ID) };
    std::unique_ptr<Sprite> connect_sprite_{ std::make_unique<Sprite>("p_connect",CMode::ID) };
};