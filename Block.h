#pragma once
#include "Obj3d.h"
#include "Camera.h"
#include <memory>

class Block
{
public:
    static constexpr float radius_{ 1.0f };

public: // ä÷êî
    Block(Camera* pCamera);

    void Update(void);
    void Draw(void);

    std::unique_ptr<Obj3d> object_;
};

