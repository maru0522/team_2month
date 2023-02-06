#pragma once
#include <memory>
#include "Camera.h"
#include "Sprite.h"
#include "Obj3d.h"
#include "BaseScene.h"
#include "Util.h"

using std::unique_ptr;

class TitleScene : public BaseScene
{
public: // ä÷êî
    void Initialize(SceneManager* pSceneManager) override;
    void Update(void) override;
    void Draw3d(void) override;
    void Draw2d(void) override;
    void Finalize(void) override;

private:
    unique_ptr<Sprite> titleName_sprite_{ std::make_unique<Sprite>("title", CMode::ID) };
    unique_ptr<Sprite> anybutton_sprite_{ std::make_unique<Sprite>("anybutton", CMode::ID) };

    unique_ptr<Util::Timer> anybuttonTimer_{ std::make_unique<Util::Timer>() };
};

