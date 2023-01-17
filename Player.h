#pragma once
#include"Sprite.h"
#include "Obj3d.h"
#include <memory>
#include "Camera.h"

class Player
{
public: // 定数
    static constexpr float speed_{ 1.0f };
    static constexpr float jumpPower_{ 2.0f };
    static constexpr float jumpAttenuation_{ 0.2f };

    // 関数
    Player(Camera* pCamera);

    void Update(void);
    void Draw(void);

    void Draw2D(void);

private:
    void Move(void);
    void Jump(void);
    void Throw(void);//投げる関数

private: // 変数
    std::unique_ptr<Obj3d> object_{};

    float jumpValue_{ 0.0f };

    bool isJump_{ false };
    bool isFloat_{ false };

    std::unique_ptr<Obj3d>  ropeObj_{};
    DirectX::XMFLOAT3 ropePos_{ 0.0f,0.0f,0.0f };
    bool isThrow_{ false };//投げた時のフラグ

    bool LimitDecrease_{ false };

    float ropeUpLimit_ = 5.0f;//ロープの上限

    bool isConduction_{ false };

};