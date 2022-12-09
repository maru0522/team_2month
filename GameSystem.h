#pragma once
#include "BasicFrame.h"

class GameSystem : public BasicFrame
{
public: // ä÷êî
    GameSystem(void);

    void Initialize(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Finalize(void) override;
};