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

    for (int i = 0; i < 7; i++)
    {

        selectRope[i] = std::make_unique<Sprite>("Resources/Image/selectRope.png", CMode::PATH);

    }

    numSprite[0] = std::make_unique<Sprite>("Resources/Image/num11.png", CMode::PATH);
    numSprite[1] = std::make_unique<Sprite>("Resources/Image/num1.png", CMode::PATH);
    numSprite[2] = std::make_unique<Sprite>("Resources/Image/num2.png", CMode::PATH);
    numSprite[3] = std::make_unique<Sprite>("Resources/Image/num3.png", CMode::PATH);
    numSprite[4] = std::make_unique<Sprite>("Resources/Image/num4.png", CMode::PATH);
    numSprite[5] = std::make_unique<Sprite>("Resources/Image/num5.png", CMode::PATH);
    numSprite[6] = std::make_unique<Sprite>("Resources/Image/num6.png", CMode::PATH);
    numSprite[7] = std::make_unique<Sprite>("Resources/Image/num7.png", CMode::PATH);
    numSprite[8] = std::make_unique<Sprite>("Resources/Image/num8.png", CMode::PATH);
    numSprite[9] = std::make_unique<Sprite>("Resources/Image/num9.png", CMode::PATH);
    numSprite[10] = std::make_unique<Sprite>("Resources/Image/num10.png", CMode::PATH);


    selectBack = std::make_unique<Sprite>("Resources/Image/selectBack.png", CMode::PATH);

    for (int i = 0; i < 6; i++)
    {
        elecSprite[i] = std::make_unique<Sprite>("Resources/Image/elecTex.png", CMode::PATH);
    }

    for (int i = 0; i < 8; i++)
    {
        selecttilePos[i].x = (-128 + (-512 + (512 * i))) + (640 - 512 * (float)selectStageIdx_);
        selecttile[i]->SetPosition(selecttilePos[i]);

    }

  
    for (int i = 0; i < 7; i++)
    {

        selectRopePos[i].x = (-128 + (-256 + (512 * i))) + (640 - 512 * (float)selectStageIdx_);
        selectRope[i]->SetPosition(selectRopePos[i]);

    }
    

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
      /*  else if (selectStageIdx_ == 0) {
            BlockManager::ClearAll();
            std::unique_ptr<BaseScene> nextScene{ sceneManager_->CreateScene("TUTORIAL") };
            sceneManager_->RequestChangeScene(nextScene);
        }*/
        else if (selectStageIdx_ >= 0) {
            oldStageIdx_ = selectStageIdx_;
            BlockManager::ClearAll();
            std::unique_ptr<BaseScene> nextScene{ sceneManager_->CreateScene("GAMEPLAY") };
            sceneManager_->RequestChangeScene(nextScene);
        }
    }


    for (int i = 0; i < 8; i++)
    {
        //selecttilePos[i].x = (-128 + (-512 + (512 * i))) + (640 - 512 * (float)selectStageIdx_);
        if (selecttilePos[i].x < (-128 + (-512 + (512 * i))) + (640 - 512 * (float)selectStageIdx_))
        {
            selecttilePos[i].x+=16;
        }
        else if(selecttilePos[i].x > (-128 + (-512 + (512 * i))) + (640 - 512 * (float)selectStageIdx_))
        {
            selecttilePos[i].x-=16;
        }

    }

    //ŒÅ’è’l‚¾‚ª

    for (int i = 0; i < 7; i++)
    {

        if (selectRopePos[i].x < (-128 + (-256 + (512 * i))) + (640 - 512 * (float)selectStageIdx_))
        {
            selectRopePos[i].x += 16;
        }
        else if (selectRopePos[i].x > (-128 + (-256 + (512 * i))) + (640 - 512 * (float)selectStageIdx_))
        {
            selectRopePos[i].x -= 16;
        }

    }


    for (int i = 0; i < 8; i++)
    {

        selecttile[i]->SetPosition(selecttilePos[i]);
        numSprite[i]->SetPosition(selecttilePos[i]);

    }

    for (int i = 0; i < 7; i++)
    {

        selectRope[i]->SetPosition(selectRopePos[i]);

    }

    elecSprite[1]->SetRotation(3.141592f / 2);
    elecSprite[3]->SetRotation(3.141592f / 2);

    for (int i = 0; i < 8; i++)
    {
        if (selecttilePos[i].x <= 640 + 128 and selecttilePos[i].x >= 640 - 128)
        {
            elecPos[0] = { selecttilePos[i].x,selecttilePos[i].y - 32 };
            elecPos[1] = { selecttilePos[i].x + 288,selecttilePos[i].y };
            elecPos[2] = { selecttilePos[i].x,selecttilePos[i].y + 256 };
            elecPos[3] = { selecttilePos[i].x,selecttilePos[i].y };
            elecTileDraw = true;
            break;
        }
        else
        {
            elecTileDraw = false;
        }
    }

    for (int i = 0; i < 7; i++)
    {
        if (selectRopePos[i].x + 128 <= 640 + 160 and selecttilePos[i].x + 128 >= 640 - 256)
        {
            elecPos[4] = { selectRopePos[i].x,selectRopePos[i].y - 16 };
            elecPos[5] = { selectRopePos[i].x,selectRopePos[i].y + 64 };
            elecRopeDraw = true;
            break;
        }
        else
        {
            elecRopeDraw = false;
        }
    }

    for (int i = 0; i < 6; i++)
    {

        if (i == 1 or i == 3)
        {
            elecPos[i].x += sinf(elecMoveTime);
        }
        else
        {
            elecPos[i].y += sinf(elecMoveTime);
        }

    }

    for (int i = 0; i < 6; i++)
    {
        elecSprite[i]->SetPosition(elecPos[i]);
    }

    for (int i = 0; i < 8; i++)
    {

        selecttile[i]->Update();
        numSprite[i]->Update();
        

    }

    for (int i = 0; i < 7; i++)
    {

        selectRope[i]->Update();

    }

    for (int i = 0; i < 6; i++)
    {
        elecSprite[i]->Update();
    }


    selectBack->Update();

    elecMoveTime++;

    DrawImGui();
}

void StageSelectScene::Draw3d(void)
{
}

void StageSelectScene::Draw2d(void)
{

    selectBack->Draw();

    for (int i = 0; i < 8; i++)
    {

        selecttile[i]->Draw();
        numSprite[i]->Draw();

    }

    for (int i = 0; i < 7; i++)
    {

        selectRope[i]->Draw();

    }

    for (int i = 0; i < 4; i++)
    {
        if (elecTileDraw)
        {
            elecSprite[i]->Draw();
        }
    }

    for (int i = 4; i < 6; i++)
    {
        if (elecRopeDraw)
        {
            elecSprite[i]->Draw();
        }
    }


}

void StageSelectScene::Finalize(void)
{
}

void StageSelectScene::DrawImGui(void)
{
#ifdef _DEBUG
    ImGui::Begin("StageSelectScene");
    ImGui::Text("stageIdx : %d",selectStageIdx_);
    ImGui::End();
#endif // _DEBUG
}
