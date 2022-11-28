#pragma once
class SceneAB
{
public: //　関数
    virtual void Initialize(void) = 0;
    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;
};

class SceneManager
{
public: // 静的関数
    template<class Scene> static void Transition(void);
};