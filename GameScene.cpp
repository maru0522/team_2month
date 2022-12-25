#include "GameScene.h"
#include "Texture.h"
#include "Model.h"
#include "Input.h"

void GameScene::Initialize(SceneManager* pSceneManager)
{
    BaseScene::Initialize(pSceneManager);

    // •Ï”‰Šú‰»
    cameraT_ = std::make_unique<Camera>();
    spriteT_ = std::make_unique<Sprite>("Resources/reimu.png", CMode::PATH);
    objT_ = std::make_unique<Obj3d>("Resources/3dModels/cube/cube.obj", cameraT_.get());
}

void GameScene::Update(void)
{
    cameraT_->Update();

    if (KEYS::IsDown(DIK_W)) {
        cameraT_->eye_.z += 5;
    }
    if (KEYS::IsDown(DIK_S)) {
        cameraT_->eye_.z -= 5;
    }
    if (KEYS::IsDown(DIK_A)) {
        cameraT_->eye_.x -= 5;
    }
    if (KEYS::IsDown(DIK_D)) {
        cameraT_->eye_.x += 5;
    }

    if (KEYS::IsDown(DIK_LEFTARROW)) {
        objT_->worldCoordinate_.position_.x -= 2;
    }
    if (KEYS::IsDown(DIK_RIGHTARROW)) {
        objT_->worldCoordinate_.position_.x += 2;
    }

    spriteT_->Update();
    objT_->Update();
}

void GameScene::Draw3d(void)
{
    objT_->Draw();
}

void GameScene::Draw2d(void)
{
    spriteT_->Draw();
}

void GameScene::Finalize(void)
{
}
