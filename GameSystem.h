#pragma once
#include <memory>
#include "BasicFrame.h"
#include "GameScene.h"

class GameSystem : public BasicFrame
{
public: // ä÷êî
    GameSystem(void);

    void Initialize(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Finalize(void) override;

private: 
    std::unique_ptr<GameScene> scene_{ nullptr };
};