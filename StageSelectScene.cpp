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

    for (int i = 0; i < 8; i++)
    {

        selecttile[i] = std::make_unique<Sprite>("Resources/Image/selecttile.png", CMode::PATH);

    }

    selectRope01 = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);
    selectRope12 = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);
    selectRope23 = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);
    selectRope34 = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);
    selectRope45 = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);
    selectRope56 = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);
    selectRope67 = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);

    selectpoint = std::make_unique<Sprite>("Resources/Image/selectpoint.png", CMode::PATH);

    for (int i = 0; i < 8; i++)
    {

        selecttile[i]->SetPosition(selecttilePos[i]);

    }


    selectRope01->SetPosition(selectRope12Pos);
    selectRope12->SetPosition(selectRope12Pos);
    selectRope23->SetPosition(selectRope23Pos);
    selectRope34->SetPosition(selectRope34Pos);
    selectRope45->SetPosition(selectRope45Pos);
    selectRope56->SetPosition(selectRope56Pos);
    selectRope67->SetPosition(selectRope67Pos);

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

    for (int i = 0; i < 8; i++)
    {
        selecttilePos[i].x = (-128 + (-512 + (512 * i))) + (640 - 512 * (float)selectStageIdx_);
    }

    //ŒÅ’è’l‚¾‚ª

    selectRope01Pos.x = (-128 - 256) + (640 - 512 * (float)selectStageIdx_);
    selectRope12Pos.x = (-128 + 256) + (640 - 512 * (float)selectStageIdx_);
    selectRope23Pos.x = (-128 + 768) + (640 - 512 * (float)selectStageIdx_);
    selectRope34Pos.x = (-128 + 1280) + (640 - 512 * (float)selectStageIdx_);
    selectRope45Pos.x = (-128 + 1792) + (640 - 512 * (float)selectStageIdx_);
    selectRope56Pos.x = (-128 + 2304) + (640 - 512 * (float)selectStageIdx_);
    selectRope67Pos.x = (-128 + 2816) + (640 - 512 * (float)selectStageIdx_);


    for (int i = 0; i < 8; i++)
    {

        selecttile[i]->SetPosition(selecttilePos[i]);

    }


    selectRope01->SetPosition(selectRope01Pos);
    selectRope12->SetPosition(selectRope12Pos);
    selectRope23->SetPosition(selectRope23Pos);
    selectRope34->SetPosition(selectRope34Pos);
    selectRope45->SetPosition(selectRope45Pos);
    selectRope56->SetPosition(selectRope56Pos);
    selectRope67->SetPosition(selectRope67Pos);

    selectpoint->SetPosition(selectpointPos);


    for (int i = 0; i < 8; i++)
    {

        selecttile[i]->Update();

    }


    selectRope01->Update();
    selectRope12->Update();
    selectRope23->Update();
    selectRope34->Update();
    selectRope45->Update();
    selectRope56->Update();
    selectRope67->Update();

    selectpoint->Update();

    DrawImGui();
}

void StageSelectScene::Draw3d(void)
{
}

void StageSelectScene::Draw2d(void)
{

    for (int i = 0; i < 8; i++)
    {

        selecttile[i]->Draw();

    }


    selectRope01->Draw();
    selectRope12->Draw();
    selectRope23->Draw();
    selectRope34->Draw();
    selectRope45->Draw();
    selectRope56->Draw();
    selectRope67->Draw();

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
