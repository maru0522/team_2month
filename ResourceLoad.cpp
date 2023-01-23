#include "ResourceLoad.h"
#include "Texture.h"
#include "Model.h"
#include "Input.h"
#include "SceneManager.h"
#include "TitleScene.h"

void ResourceLoad::Initialize(SceneManager* pSceneManager)
{
    BaseScene::Initialize(pSceneManager);

    // �e�N�X�`���ǂݍ���
    Texture::Load("Resources/mario.jpg");
    Texture::Load("Resources/thinking.png");
    Texture::Load("Resources/Image/Type_START.png");
    Texture::Load("Resources/Image/Type_PATH.png");
    Texture::Load("Resources/Image/Type_HOOK.png");
    Texture::Load("Resources/Image/Type_POWERSUPPLY.png");
    Texture::Load("Resources/Image/Type_POWERRECEIVE.png");
    Texture::Load("Resources/Image/Type_HOOK.png");
    Texture::Load("Resources/Image/title.png");
    Texture::Load("Resources/Image/title2.png");
    Texture::Load("Resources/Image/Reset.png");
    Texture::Load("Resources/Image/KEY_ENTER.png");
    Texture::Load("Resources/Image/KEY_ENTER_PRESS.png");

    // 3d.obj�ǂݍ���
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
