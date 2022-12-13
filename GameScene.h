#pragma once
#include <memory>
#include "Camera.h"
#include "Sprite.h"
#include "Obj3d.h"

using std::unique_ptr;

class GameScene
{
public: // ä÷êî
    void Initialize(void);
    void Update(void);
    void Draw3d(void);
    void Draw2d(void);
    void Finalize(void);

private:
    unique_ptr<Camera> cameraT_{};
    unique_ptr<Sprite> spriteT_{};
    unique_ptr<Obj3d> objT_{};
};

