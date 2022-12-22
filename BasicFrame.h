#pragma once
#include "Window.h"
#include "InitDirectX.h"
#include "ImGuiController.h"
#include <memory>
#include "SceneManager.h"

class BasicFrame
{
public: // ä÷êî
    virtual ~BasicFrame(void) = default;

    void Run(void);

    virtual void Initialize(void);
    virtual void Update(void);
    virtual void Draw(void) = 0;
    virtual void Finalize(void);

    virtual bool EndRequest(void);

protected: // ïœêî
    InitDirectX* iDX_{ nullptr };
    std::unique_ptr<SceneManager> sceneManager_{ nullptr };


private: // ïœêî
    Window* wnd_{ nullptr };
    std::unique_ptr<ImGuiController> imGuiController_{ nullptr };
};

