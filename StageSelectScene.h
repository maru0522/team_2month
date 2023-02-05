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

    DirectX::XMFLOAT2 selecttilePos[8] = { {-512,232},{ 0,232 },{ 512 ,232 },{ 1024 ,232 },{ 1536 ,232 },{ 2048 ,232 },{ 2560 ,232 },{ 3072 ,232 } };

    DirectX::XMFLOAT2 selectRope01Pos = { -256 ,328 };
    DirectX::XMFLOAT2 selectRope12Pos = { 256 ,328 };
    DirectX::XMFLOAT2 selectRope23Pos = { 768,328 };
    DirectX::XMFLOAT2 selectRope34Pos = { 1280,328 };
    DirectX::XMFLOAT2 selectRope45Pos = { 1792,328 };
    DirectX::XMFLOAT2 selectRope56Pos = { 2304,328 };
    DirectX::XMFLOAT2 selectRope67Pos = { 2816,328 };

    DirectX::XMFLOAT2 selectpointPos = { 640,360 };

    std::unique_ptr<Sprite> selecttile[8];


    std::unique_ptr<Sprite> selectRope01 = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);
    std::unique_ptr<Sprite> selectRope12 = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);
    std::unique_ptr<Sprite> selectRope23 = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);
    std::unique_ptr<Sprite> selectRope34 = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);
    std::unique_ptr<Sprite> selectRope45 = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);
    std::unique_ptr<Sprite> selectRope56 = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);
    std::unique_ptr<Sprite> selectRope67 = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);

    std::unique_ptr<Sprite> selectpoint = std::make_unique<Sprite>("Resources/Image/selectpoint.png", CMode::PATH);

};

