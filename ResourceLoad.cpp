#include "ResourceLoad.h"
#include "Texture.h"
#include "Model.h"
#include "Input.h"
#include "SceneManager.h"
#include "TitleScene.h"

void ResourceLoad::Initialize(SceneManager* pSceneManager)
{
    BaseScene::Initialize(pSceneManager);

    // テクスチャ読み込み
    Texture::Load("Resources/mario.jpg");
    Texture::Load("Resources/thinking.png");

    // 3d.obj読み込み
    Model::Load("Resources/3dModels/cube/cube.obj");
}

void ResourceLoad::Update(void)
{
    std::unique_ptr<BaseScene> nextScene{ sceneManager_->CreateScene("TITLE") };
    sceneManager_->RequestChangeScene(nextScene);
}

void ResourceLoad::Draw3d(void)
{
}

void ResourceLoad::Draw2d(void)
{
}

void ResourceLoad::Finalize(void)
{
}
