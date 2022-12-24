#include "SceneManager.h"

SceneManager::~SceneManager(void)
{
    Finalize();
}

std::unique_ptr<BaseScene> SceneManager::CreateScene(const std::string& sceneName)
{
    return std::move(sceneFactory_->CreateScene(sceneName));
}

void SceneManager::Update(void)
{
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

void SceneManager::Draw3d(void)
{
    currentScene_->Draw3d();
}

void SceneManager::Draw2d(void)
{
    currentScene_->Draw2d();
}

void SceneManager::Finalize(void)
{
    if (currentScene_) {
        currentScene_->Finalize();
        currentScene_.reset();
    }
}
