#pragma once
#include <memory>
#include "BasicFrame.h"
#include "BaseScene.h"
#include "ImGuiController.h"

class GameSystem : public BasicFrame
{
public: // �֐�
    GameSystem(void);

    void Initialize(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Finalize(void) override;

private: 
    std::unique_ptr<BaseScene> scene_{ nullptr };
};