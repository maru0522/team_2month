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

    selecttile1 = std::make_unique<Sprite>("Resources/Image/selecttile.png", CMode::PATH);
    selecttile2 = std::make_unique<Sprite>("Resources/Image/selecttile.png", CMode::PATH);
    selecttile3 = std::make_unique<Sprite>("Resources/Image/selecttile.png", CMode::PATH);

    selectRope12 = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);
    selectRope23 = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);

    selectpoint = std::make_unique<Sprite>("Resources/Image/selectpoint.png", CMode::PATH);

    selecttile1->SetPosition(selecttile1Pos);
    selecttile2->SetPosition(selecttile2Pos);
    selecttile3->SetPosition(selecttile3Pos);


    selectRope12->SetPosition(selectRope12Pos);
    selectRope23->SetPosition(selectRope23Pos);

    selectpoint->SetPosition(selectpointPos);
    

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

    selecttile1Pos.x = -128+(640 - 512 * selectStageIdx_);
    selecttile2Pos.x = (-256 + 640) + (640 - 512 * selectStageIdx_);
    selecttile3Pos.x = (-128 + 1024) + (640 - 512 * selectStageIdx_);

    selectRope12Pos.x = (-128 + 256) + (640 - 512 * selectStageIdx_);
    selectRope23Pos.x = (-128 + 768) + (640 - 512 * selectStageIdx_);

    selecttile1->SetPosition(selecttile1Pos);
    selecttile2->SetPosition(selecttile2Pos);
    selecttile3->SetPosition(selecttile3Pos);


    selectRope12->SetPosition(selectRope12Pos);
    selectRope23->SetPosition(selectRope23Pos);

    selectpoint->SetPosition(selectpointPos);

    selecttile1->Update();
    selecttile2->Update();
    selecttile3->Update();


    selectRope12->Update();
    selectRope23->Update();

    selectpoint->Update();

    DrawImGui();
}

void StageSelectScene::Draw3d(void)
{
}

void StageSelectScene::Draw2d(void)
{

    selecttile1->Draw();
    selecttile2->Draw();
    selecttile3->Draw();


    selectRope12->Draw();
    selectRope23->Draw();

    selectpoint->Draw();

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
