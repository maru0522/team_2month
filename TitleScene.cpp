#include "TitleScene.h"
#include "Texture.h"
#include "Model.h"
#include "Input.h"
#include "SceneManager.h"
#include "GameScene.h"

void TitleScene::Initialize(SceneManager* pSceneManager)
{
    BaseScene::Initialize(pSceneManager);

    // •Ï”‰Šú‰»
    anybutton_sprite_->SetPosition({ 500,500 });
    anybutton_sprite_->SetColor({ 1.f,1.f,1.f,1.f });
    anybuttonTimer_->Start(2.5f);

    titleName_sprite_->SetPosition({400,250});
   /* titleName_sprite_->SetSize({512.f,128.f});*/ //Œ³‚Ì‰æ‘œ‚Ì2”{
    //c‚Ì‚â‚Â‚ÉŠY“–‚·‚é‚à‚Ì‚ð‚‚É‚·‚é
    elecSprite[1]->SetRotation(3.141592f / 2);
    elecSprite[3]->SetRotation(3.141592f / 2);

    for (int i = 0; i < 4; i++)
    {
        elecSprite[i]->SetSize({ 64.0f,32.0f });
    }

    titleChengSprite->SetSize({ 1280.0f,720.0f });
    titleRopeSprite->SetPosition({ -1248.0f, 720.0f - 80.0f });

}

void TitleScene::Update(void)
{
    titleName_sprite_->Update();
    anybutton_sprite_->Update();
    BackSprite->Update();
    titleEffectSprite->Update();
    titleChengSprite->Update();
    titleRopeSprite->Update();

    if (anybuttonTimer_->GetIsEnd()) anybuttonTimer_->Start(2.5f);
    float time{ anybuttonTimer_->GetElapsedTime() / anybuttonTimer_->GetEndTime() };
    float alpha{};
    if (anybuttonTimer_->GetElapsedTime() <= anybuttonTimer_->GetEndTime() / 2.f) alpha = Util::EaseInOutSine(0.f,2.f,time);
    else alpha = 2.f - Util::EaseInOutSine(0.f,2.f,time);

    anybutton_sprite_->SetColor({ 1.f,1.f,1.f,alpha });


    if (KEYS::IsTrigger(DIK_RETURN) and isEffectEnd) {
        SceneManager::isInsertOk_ = true;
        std::unique_ptr<BaseScene> nextScene{ sceneManager_->CreateScene("STAGESELECT") };
        sceneManager_->RequestChangeScene(nextScene);
    }

    if (XPAD::IsTriggerSome() and isEffectEnd) {
        SceneManager::isInsertOk_ = true;
        std::unique_ptr<BaseScene> nextScene{ sceneManager_->CreateScene("STAGESELECT") };
        sceneManager_->RequestChangeScene(nextScene);
    }

    if (XPAD::IsTriggerSome() and isEffectEnd==false) {

        isEffectEnd = true;
    }

    if (isEffectEnd)
    {
        ropeMoveTime = maxRopeMoveTime;
        ChengWaitTime1 = 0;
        ChengWaitTime2 = 0;
        ChengWaitTime3 = 0;
        ChengWaitTime4 = 0;
        isCheng = true;
    }
    
    if (ropeMoveTime < maxRopeMoveTime and isEffectEnd == false)
    {
        titleRopeSprite->SetPosition({ easeInQuint(-1248.0f,256.0f,ropeMoveTime / maxRopeMoveTime),720.0f - 80.0f });
        ropeMoveTime++;
    }

    if (ropeMoveTime >= maxRopeMoveTime and isEffectEnd == false)
    {
        if (ChengWaitTime1 > 0)
        {
            isCheng = true;
            ChengWaitTime1--;
        }
        else if (ChengWaitTime2 > 0)
        {
            isCheng = false;
            ChengWaitTime2--;
        }
        else if (ChengWaitTime3 > 0)
        {
            isCheng = true;
            ChengWaitTime3--;
        }
        else if (ChengWaitTime4 > 0)
        {
            isCheng = false;
            ChengWaitTime4--;
        }
        else
        {
            isCheng = true;
            isEffectEnd = true;
        }


    }


    elecSprite[0]->SetPosition({ lerp(titleName_sprite_->GetPosition().x + 512,titleName_sprite_->GetPosition().x - 64,elecMoveWTime / maxElecMoveWTime),(titleName_sprite_->GetPosition().y + 64) + sinf(elecMoveWTime) * 2 });
    elecSprite[1]->SetPosition({ titleName_sprite_->GetPosition().x + sinf(elecMoveHTime) * 2 ,lerp(titleName_sprite_->GetPosition().y + 64,titleName_sprite_->GetPosition().y - 64,elecMoveHTime / maxElecMoveHTime) });
    elecSprite[2]->SetPosition({ lerp(titleName_sprite_->GetPosition().x - 64 ,titleName_sprite_->GetPosition().x + 512,elecMoveWTime / maxElecMoveWTime),(titleName_sprite_->GetPosition().y - 32) + sinf(elecMoveWTime) * 2 });
    elecSprite[3]->SetPosition({ (titleName_sprite_->GetPosition().x + 544) + sinf(elecMoveHTime) * 2 ,lerp(titleName_sprite_->GetPosition().y - 64,titleName_sprite_->GetPosition().y + 64,elecMoveHTime / maxElecMoveHTime) });

    if (elecMoveWTime < maxElecMoveWTime)
    {
        elecMoveWTime++;
    }
    else
    {
        elecMoveWTime = 0;
    }

    if (elecMoveHTime < maxElecMoveHTime)
    {
        elecMoveHTime++;
    }
    else
    {
        elecMoveHTime = 0;
    }

    for (int i = 0; i < 4; i++)
    {
        elecSprite[i]->Update();
    }


#ifdef _DEBUG
    ImGui::Begin("title screen");
    ImGui::Text("alpha : %f", alpha);
    ImGui::End();
#endif // _DEBUG
}

void TitleScene::Draw3d(void)
{
}

void TitleScene::Draw2d(void)
{
    BackSprite->Draw();
   
    titleName_sprite_->Draw();
    anybutton_sprite_->Draw();
    for (int i = 0; i < 4; i++)
    {
        elecSprite[i]->Draw();
    }
    titleEffectSprite->Draw();
    if (!isCheng)
    {
        titleChengSprite->Draw();
    }
    titleRopeSprite->Draw();
}

void TitleScene::Finalize(void)
{
}

float TitleScene::lerp(const float& start, const float& end, const float t)
{
    return start * (1.0f - t) + end * t;
}

float TitleScene::easeInQuint(float start, float end, float time)
{
    return start + (time * time * time * time * time) * (end - start);
}
