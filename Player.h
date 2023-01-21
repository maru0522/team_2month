#pragma once
#include "Obj3d.h"
#include <memory>
#include "Camera.h"

class Player
{
private: // ��`
    enum class State
    {
        DEFAULT,
        ROPE,
        TARZAN,
    };

public: // �萔
    static constexpr float speed_{ 0.4f };
    static constexpr float ropeSpeed_{ 0.2f };
    static constexpr float jumpPower_{ 1.2f }; // 2.4f -> 1.2f
    static constexpr float jumpAttenuation_{ 0.125f };
    static constexpr float gravity_{ 0.35f };
    static constexpr DirectX::XMFLOAT3 radius_{ 1.0f,1.0f,1.0f };

    // �֐�
    Player(Camera* pCamera);

    void Update(void);
    void Draw(void);

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

    void SetState(State state) { state_ = state; }

private: // �ϐ�
    std::unique_ptr<Obj3d> object_{};

    // ������
    float jumpValue_{ 0.0f };

    bool isJump_{ false };
    bool isFloat_{ false };

    // ���
    State state_{ State::DEFAULT };
    bool isUnderHook_{ false }; // UI�\���̂��߂̂���
};