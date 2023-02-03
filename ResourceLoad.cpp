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
    Texture::Load("Resources/Image/Type_GOAL.png");
    Texture::Load("Resources/Image/Type_PATH.png");
    Texture::Load("Resources/Image/Type_HOOK.png");
    Texture::Load("Resources/Image/Type_POWERSUPPLY.png");
    Texture::Load("Resources/Image/Type_POWERRECEIVE.png");
    Texture::Load("Resources/Image/Type_SWITCH.png");

    Texture::Load("Resources/Image/wire.png");

    // タイトル
    Texture::Load("Resources/Image/Title/title.png","titleName");
    Texture::Load("Resources/Image/Title/title_anybutton.png","anybutton");
    Texture::Load("Resources/Image/Title/gameTitle.png", "title");

    //ステージセレクト
    Texture::Load("Resources/Image/num.png");
    Texture::Load("Resources/Image/selectRope.png");

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
    Model::Load("Resources/3dModels/playertest2/playertest2.obj");
    Model::Load("Resources/3dModels/ropetest/ropetest.obj");
    //プレイヤー読み込み
    Model::Load("Resources/3dModels/player/player.obj");
    //ロープ読み込み
    Model::Load("Resources/3dModels/rope25/rope25.obj");
    Model::Load("Resources/3dModels/rope50/rope50.obj");
    Model::Load("Resources/3dModels/rope75/rope75.obj");
    Model::Load("Resources/3dModels/rope100/rope100.obj");
   
    Model::Load("Resources/3dModels/powerBlock/powerBlock.obj");

    Model::Load("Resources/3dModels/ropepin/ropepin.obj");
    
    Model::Load("Resources/3dModels/pinandrope/pinandrope_end.obj");

    Model::Load("Resources/3dModels/pinandrope_end/pinandrope_end.obj");
    Model::Load("Resources/3dModels/pinandrope_rope/pinandrope_rope.obj");
    Model::Load("Resources/3dModels/pinandrope2_tex/pinandrope2_tex.obj");



    Model::Load("Resources/3dModels/buttonX/buttonX.obj");
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
