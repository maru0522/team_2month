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
    Texture::Load("Resources/Image/Type_START.png");
    Texture::Load("Resources/Image/Type_PATH.png");
    Texture::Load("Resources/Image/Type_HOOK.png");
    Texture::Load("Resources/Image/Type_POWERSUPPLY.png");
    Texture::Load("Resources/Image/Type_POWERRECEIVE.png");
    Texture::Load("Resources/Image/Type_SWITCH.png");

    // タイトル
    Texture::Load("Resources/Image/Title/title.png","titleName");
    Texture::Load("Resources/Image/Title/title_anybutton.png","anybutton");

    // ゲーム - player
    Texture::Load("Resources/Image/Game/Player/player_default.png","p_default");
    Texture::Load("Resources/Image/Game/Player/player_rope.png","p_rope");
    Texture::Load("Resources/Image/Game/Player/player_isConnect.png","p_connect");

    // XPAD
    Texture::Load("Resources/Image/XPAD/XPAD_A.png","XPAD_A");
    Texture::Load("Resources/Image/XPAD/XPAD_A_PRESS.png","XPAD_A_PRESS");
    Texture::Load("Resources/Image/XPAD/XPAD_X.png", "XPAD_X");
    Texture::Load("Resources/Image/XPAD/XPAD_X_PRESS.png", "XPAD_X_PRESS");
    Texture::Load("Resources/Image/XPAD/XPAD_RB.png","XPAD_RB");
    Texture::Load("Resources/Image/XPAD/XPAD_RB_PRESS.png","XPAD_RB_PRESS");

    Texture::Load("Resources/Image/Reset.png");
    Texture::Load("Resources/Image/KEY_ENTER.png");
    Texture::Load("Resources/Image/KEY_ENTER_PRESS.png");

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
