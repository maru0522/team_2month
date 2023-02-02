#pragma once
#include "BaseScene.h"

class StageSelectScene : public BaseScene
{
private:
    // 静的変数
    static int32_t selectStageIdx_;
    static int32_t oldStageIdx_;

public:
    // 関数
    void Initialize(SceneManager* pSceneManager) override;
    void Update(void) override;
    void Draw3d(void) override;
    void Draw2d(void) override;
    void Finalize(void) override;

    void DrawImGui(void);

    inline static void SetStageIdx(int32_t idx) { selectStageIdx_ = idx; }
    inline static const int32_t GetStageIdx(void) { return selectStageIdx_; }

    bool preStick{ false };
};

