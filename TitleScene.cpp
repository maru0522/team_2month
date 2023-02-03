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

}

void TitleScene::Update(void)
{
    titleName_sprite_->Update();
    anybutton_sprite_->Update();

    if (anybuttonTimer_->GetIsEnd()) anybuttonTimer_->Start(2.5f);
    float time{ anybuttonTimer_->GetElapsedTime() / anybuttonTimer_->GetEndTime() };
    float alpha{};
    if (anybuttonTimer_->GetElapsedTime() <= anybuttonTimer_->GetEndTime() / 2.f) alpha = Util::EaseInOutSine(0.f,2.f,time);
    else alpha = 2.f - Util::EaseInOutSine(0.f,2.f,time);

    anybutton_sprite_->SetColor({ 1.f,1.f,1.f,alpha });


    //if (KEYS::IsTrigger(DIK_RETURN)) {
    //    std::unique_ptr<BaseScene> nextScene{ sceneManager_->CreateScene("GAMEPLAY") };
    //    sceneManager_->RequestChangeScene(nextScene);
    //}

    if (XPAD::IsTriggerSome()) {
        SceneManager::isInsertOk_ = true;
        std::unique_ptr<BaseScene> nextScene{ sceneManager_->CreateScene("STAGESELECT") };
        sceneManager_->RequestChangeScene(nextScene);
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
    titleName_sprite_->Draw();
    anybutton_sprite_->Draw();
}

void TitleScene::Finalize(void)
{
}
