#pragma once
class SceneAB
{
public: //Å@ä÷êî
    virtual void Initialize(void) = 0;
    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;
};

class SceneManager
{
public: // ê√ìIä÷êî
    template<class Scene> static void Transition(void);
};