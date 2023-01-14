#pragma once
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

private:
    void Move(void);
    void Jump(void);

private: // 変数
    std::unique_ptr<Obj3d> object_{};

    float jumpValue_{ 0.0f };

    bool isJump_{ false };
    bool isFloat_{ false };
};