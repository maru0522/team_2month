#pragma once
#include "BaseScene.h"
#include "Sprite.h"

class StageSelectScene : public BaseScene
{
private:
    // ê√ìIïœêî
    static int32_t selectStageIdx_;
    static int32_t oldStageIdx_;

public:
    // ä÷êî
    void Initialize(SceneManager* pSceneManager) override;
    void Update(void) override;
    void Draw3d(void) override;
    void Draw2d(void) override;
    void Finalize(void) override;

    void DrawImGui(void);

    inline static void SetStageIdx(int32_t idx) { selectStageIdx_ = idx; }
    inline static const int32_t GetStageIdx(void) { return selectStageIdx_; }

    bool preStick{ false };

private:

    DirectX::XMFLOAT2 selecttile1Pos = { 0,232 };
    DirectX::XMFLOAT2 selecttile2Pos = { 640 ,232 };
    DirectX::XMFLOAT2 selecttile3Pos = { 1024 ,232 };
    DirectX::XMFLOAT2 selectRope12Pos = { 256 ,328 };
    DirectX::XMFLOAT2 selectRope23Pos = { 768,328 };
    DirectX::XMFLOAT2 selectpointPos = { 640,360 };

    std::unique_ptr<Sprite> selecttile1 = std::make_unique<Sprite>("Resources/Image/selecttile.png", CMode::PATH);
    std::unique_ptr<Sprite> selecttile2 = std::make_unique<Sprite>("Resources/Image/selecttile.png", CMode::PATH);
    std::unique_ptr<Sprite> selecttile3 = std::make_unique<Sprite>("Resources/Image/selecttile.png", CMode::PATH);

    std::unique_ptr<Sprite> selectRope12 = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);
    std::unique_ptr<Sprite> selectRope23 = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);

    std::unique_ptr<Sprite> selectpoint = std::make_unique<Sprite>("Resources/Image/selectpoint.png", CMode::PATH);

};

