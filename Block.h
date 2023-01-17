#pragma once
#include "Obj3d.h"
#include "Camera.h"
#include <memory>

class Block
{
public:
    static constexpr float radius_{ 1.0f };

    enum class Type
    {
        START,
        PATH,
        SWITCH,
    };

public: // ä÷êî
    Block(Type type, Camera* pCamera);

    void Update(void);
    void Draw(void);

    void SetVisible(bool isVisible) { isVisible_ = isVisible; }

    std::unique_ptr<Obj3d> object_;

private:
    bool isVisible_{ true };
};

