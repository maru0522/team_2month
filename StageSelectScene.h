#pragma once
#include "BaseScene.h"
#include "Sprite.h"

class StageSelectScene : public BaseScene
{
private:
    // �ÓI�ϐ�
    static int32_t selectStageIdx_;
    static int32_t oldStageIdx_;

public:
    // �֐�
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

    DirectX::XMFLOAT2 selecttilePos[8] = { {0,232},{ 512,232 },{ 1024 ,232 },{ 1536 ,232 },{ 2048 ,232 },{ 2560 ,232 },{ 3072 ,232 },{ 3584 ,232 } };

    DirectX::XMFLOAT2 selectRope01Pos = { 0 ,328 };
    DirectX::XMFLOAT2 selectRope12Pos = { 512 ,328 };
    DirectX::XMFLOAT2 selectRope23Pos = { 1024,328 };
    DirectX::XMFLOAT2 selectRope34Pos = { 1536,328 };
    DirectX::XMFLOAT2 selectRope45Pos = { 2048,328 };
    DirectX::XMFLOAT2 selectRope56Pos = { 2560,328 };
    DirectX::XMFLOAT2 selectRope67Pos = { 3072,328 };

    DirectX::XMFLOAT2 selectpointPos = { 640,360 };

    std::unique_ptr<Sprite> selecttile[8];

    std::unique_ptr<Sprite> numSprite[11];

    std::unique_ptr<Sprite> selectBack;


    std::unique_ptr<Sprite> selectRope01 = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);
    std::unique_ptr<Sprite> selectRope12 = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);
    std::unique_ptr<Sprite> selectRope23 = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);
    std::unique_ptr<Sprite> selectRope34 = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);
    std::unique_ptr<Sprite> selectRope45 = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);
    std::unique_ptr<Sprite> selectRope56 = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);
    std::unique_ptr<Sprite> selectRope67 = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);

    std::unique_ptr<Sprite> selectpoint = std::make_unique<Sprite>("Resources/Image/selectpoint.png", CMode::PATH);

};

