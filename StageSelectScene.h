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

    DirectX::XMFLOAT2 selecttilePos[8] = { {0,232},{ 512,232 },{ 1024 ,232 },{ 1536 ,232 },{ 2048 ,232 },{ 2560 ,232 },{ 3072 ,232 },{ 3584 ,232 } };

    DirectX::XMFLOAT2 selectRopePos[7] = { { 0 ,328 } ,{ 512 ,328 } ,{ 1024,328 },{ 1536,328 },{ 2048,328 },{ 2560,328 },{ 3072,328 } };

    DirectX::XMFLOAT2 elecPos[6] = { {512,200},{800,232},{512,488},{512,232} };

    bool elecTileDraw = false;
    bool elecRopeDraw = false;

    float elecMoveTime = 0;

    std::unique_ptr<Sprite> selecttile[8];

    std::unique_ptr<Sprite> numSprite[11];

    std::unique_ptr<Sprite> elecSprite[6];

    std::unique_ptr<Sprite> selectBack;

    std::unique_ptr<Sprite> selectRope[7];


};

