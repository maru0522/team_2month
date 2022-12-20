#pragma once
class BaseScene
{
public:
    virtual ~BaseScene(void) = default;

    virtual void Initialize(void) = 0;
    virtual void Update(void) = 0;
    virtual void Draw3d(void) = 0;
    virtual void Draw2d(void) = 0;
    virtual void Finalize(void) = 0;
};

