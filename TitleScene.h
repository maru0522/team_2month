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

    float lerp(const float& start, const float& end, const float t);
    float easeInQuint(float start, float end, float time);

private:
    unique_ptr<Sprite> titleName_sprite_{ std::make_unique<Sprite>("title", CMode::ID) };
    unique_ptr<Sprite> anybutton_sprite_{ std::make_unique<Sprite>("anybutton", CMode::ID) };

    unique_ptr<Util::Timer> anybuttonTimer_{ std::make_unique<Util::Timer>() };
    
    std::unique_ptr<Sprite> BackSprite{ std::make_unique<Sprite>("Resources/Image/selectBack.png", CMode::PATH) };
    std::unique_ptr<Sprite> titleEffectSprite{ std::make_unique<Sprite>("Resources/Image/titleeffect.png", CMode::PATH) };
    std::unique_ptr<Sprite> titleChengSprite{ std::make_unique<Sprite>("Resources/Image/cheng.png", CMode::PATH) };
    std::unique_ptr<Sprite> titleRopeSprite{ std::make_unique<Sprite>("Resources/Image/titleRope.png", CMode::PATH) };

    std::unique_ptr<Sprite> elecSprite[4] = { { std::make_unique<Sprite>("Resources/Image/elecTex.png", CMode::PATH) } ,{ std::make_unique<Sprite>("Resources/Image/elecTex.png", CMode::PATH) } ,{ std::make_unique<Sprite>("Resources/Image/elecTex.png", CMode::PATH) } ,{ std::make_unique<Sprite>("Resources/Image/elecTex.png", CMode::PATH) } };

    float maxElecMoveWTime = 50;
    float maxElecMoveHTime = 50;

    float elecMoveWTime = 0;
    float elecMoveHTime = 0;

    float maxRopeMoveTime = 30;
    float ropeMoveTime = 0;

    //ì_ñ≈Ç∑ÇÈÇ∆Ç´ÇÃêÿÇËë÷ÇÌÇÈÇΩÇﬂÇÃéûä‘
    float ChengWaitTime1 = 10;
    float ChengWaitTime2 = 20;
    float ChengWaitTime3 = 5;
    float ChengWaitTime4 = 5;

    //ïœÇÌÇ¡ÇƒÇ¢ÇÈÇ©
    bool isCheng = false;

    bool isEffectEnd = false;

};

