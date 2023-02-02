#include "StageSelectScene.h"
#include "BlockManager.h"
#include "SceneManager.h"
#include "Input.h"

int32_t StageSelectScene::selectStageIdx_{ 0 };
int32_t StageSelectScene::oldStageIdx_{ 0 };

void StageSelectScene::Initialize(SceneManager* pSceneManager)
{
    BaseScene::Initialize(pSceneManager);

    selectStageIdx_ = oldStageIdx_;
}

void StageSelectScene::Update(void)
{
    if (preStick == false && 0 < XPAD::GetLStick().x) {
        preStick = true;
        selectStageIdx_ < 6 /*stageNumMax*/ ? selectStageIdx_++ : selectStageIdx_ = 6;
    }
    else if (preStick == false && XPAD::GetLStick().x < 0) {
        preStick = true;
        selectStageIdx_ > -1 ? selectStageIdx_-- : selectStageIdx_ = -1;
    }
    else {
        preStick = false;
    }

    if (XPAD::IsTrigger(XPAD_RIGHT)) {
        selectStageIdx_ < 6 /*stageNumMax*/ ? selectStageIdx_++ : selectStageIdx_ = 6;
    }
    else if (XPAD::IsTrigger(XPAD_LEFT)) {
        selectStageIdx_ > -1 ? selectStageIdx_-- : selectStageIdx_ = -1;
    }

    if (XPAD::IsTrigger(XPAD_A)) {
        if (selectStageIdx_ == -1) {
            BlockManager::ClearAll();
            std::unique_ptr<BaseScene> nextScene{ sceneManager_->CreateScene("TITLE") };
            sceneManager_->RequestChangeScene(nextScene);
        }
        else if (selectStageIdx_ == 0) {
            BlockManager::ClearAll();
            std::unique_ptr<BaseScene> nextScene{ sceneManager_->CreateScene("TUTORIAL") };
            sceneManager_->RequestChangeScene(nextScene);
        }
        else if (selectStageIdx_ >= 1) {
            oldStageIdx_ = selectStageIdx_;
            BlockManager::ClearAll();
            std::unique_ptr<BaseScene> nextScene{ sceneManager_->CreateScene("GAMEPLAY") };
            sceneManager_->RequestChangeScene(nextScene);
        }
    }

    DrawImGui();
}

void StageSelectScene::Draw3d(void)
{
}

void StageSelectScene::Draw2d(void)
{
}

void StageSelectScene::Finalize(void)
{
}

void StageSelectScene::DrawImGui(void)
{
    ImGui::Begin("StageSelectScene");
    ImGui::Text("stageIdx : %d",selectStageIdx_);
    ImGui::End();
}
