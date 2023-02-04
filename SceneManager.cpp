#include "SceneManager.h"
#include "Texture.h"

bool SceneManager::isInsertOk_{ false };

SceneManager::SceneManager(void)
{
    

    insertAnimation_->SetSprite();
}

SceneManager::~SceneManager(void)
{
    Finalize();
}

std::unique_ptr<BaseScene> SceneManager::CreateScene(const std::string& sceneName)
{
    return std::move(sceneFactory_->CreateScene(sceneName));
}

void SceneManager::RequestChangeScene(unique_ptr<BaseScene>& nextScene)
{
    nextScene_ = std::move(nextScene);
}

void SceneManager::Update(void)
{
    if (isInsertOk_) {
        insertAnimation_->Update();
        if (nextScene_) {
            if (insertAnimation_->GetIsResetOk()) insertAnimation_->Reset();

            if (insertAnimation_->GetIsStart() == false) {
                insertAnimation_->SetIsStart(true);
            }

            if (insertAnimation_->GetIsEnd() == true) {
                if (currentScene_) {
                    currentScene_->Finalize();
                    currentScene_.reset();
                }

                currentScene_ = std::move(nextScene_);
                nextScene_.reset();

                currentScene_->Initialize(this);
            }
        }

        if (insertAnimation_->GetIsEnd() == true) {
            currentScene_->Update();
        }
    }
    else {
        if (nextScene_) {
            if (currentScene_) {
                currentScene_->Finalize();
                currentScene_.reset();
            }

            currentScene_ = std::move(nextScene_);
            nextScene_.reset();

            currentScene_->Initialize(this);
        }
        currentScene_->Update();
    }
}

void SceneManager::Draw3d(void)
{
    currentScene_->Draw3d();
}

void SceneManager::Draw2d(void)
{
     currentScene_->Draw2d();
    if (insertAnimation_->GetIsStart()) insertAnimation_->Draw();
}

void SceneManager::Finalize(void)
{
    if (currentScene_) {
        currentScene_->Finalize();
        currentScene_.reset();
    }
}
