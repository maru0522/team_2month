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
    title_ = std::make_unique<Sprite>("Resources/Image/title.png", CMode::PATH);
    enter_ = std::make_unique<Sprite>("Resources/Image/title2.png", CMode::PATH);
    enter_->SetPosition({ 500,500 });
}

void TitleScene::Update(void)
{
    title_->Update();
    enter_->Update();

    if (KEYS::IsTrigger(DIK_RETURN)) {
        std::unique_ptr<BaseScene> nextScene{ sceneManager_->CreateScene("GAMEPLAY") };
        sceneManager_->RequestChangeScene(nextScene);
    }
}

void TitleScene::Draw3d(void)
{
}

void TitleScene::Draw2d(void)
{
    title_->Draw();
    enter_->Draw();
}

void TitleScene::Finalize(void)
{
}
