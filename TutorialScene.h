#pragma once
#include "BaseScene.h"
class TutorialScene :
    public BaseScene
{
public: // �֐�
    void Initialize(SceneManager* pSceneManager) override;
    void Update(void) override;
    void Draw3d(void) override;
    void Draw2d(void) override;
    void Finalize(void) override;

};

