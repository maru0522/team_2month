#include "ImGuiController.h"
#include <imgui_impl_win32.h>
#include "Window.h"
#include <imgui_impl_dx12.h>
#include "InitDirectX.h"
#include <imgui.h>


ImGuiController::ImGuiController(void)
{
    Initialize();
}

ImGuiController::~ImGuiController(void)
{

}

void ImGuiController::Update(void)
{
    Begin();
    //ImGui::Text("Hello, world %d", 123);
    //char buf[20]{};
    //ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
    //float f{};
    //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

    //static bool isSpriteGuiWnd{ true };
    //ImGui::Begin("SpriteGui", &isSpriteGuiWnd, ImGuiWindowFlags_None);
    //if (ImGui::BeginMenuBar())
    //ImGui::End();

    //bool my_tool_active{true};
    //// Create a window called "My First Tool", with a menu bar.
    //ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
    //if (ImGui::BeginMenuBar())
    //{
    //    if (ImGui::BeginMenu("File"))
    //    {
    //        if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
    //        if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
    //        if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
    //        ImGui::EndMenu();
    //    }
    //    ImGui::EndMenuBar();
    //}

    //float my_color[4]{};

    //// Edit a color stored as 4 floats
    //ImGui::ColorEdit4("Color", my_color);

    //// Generate samples and plot them
    //float samples[100];
    //for (int n = 0; n < 100; n++)
    //    samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
    //ImGui::PlotLines("Samples", samples, 100);

    //// Display contents in a scrolling region
    //ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
    //ImGui::BeginChild("Scrolling");
    //for (int n = 0; n < 50; n++)
    //    ImGui::Text("%04d: Some text", n);
    //ImGui::EndChild();
    //ImGui::End();

    //ImGui::ShowDemoWindow();

    End();
}

void ImGuiController::Begin(void)
{
    ImGui_ImplWin32_NewFrame();
    ImGui_ImplDX12_NewFrame();
    ImGui::NewFrame();
}

void ImGuiController::End(void)
{
    ImGui::Render();
}

void ImGuiController::Draw(void)
{
    InitDirectX* iDX = InitDirectX::GetInstance();

    ID3D12DescriptorHeap* ppHeaps[] = { srvHeap_.Get() };
    iDX->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), iDX->GetCommandList());
}

void ImGuiController::SpriteSlider(float xy[2])
{
    static bool isSpriteGuiWnd{ true };
    ImGui::SetWindowSize({ 500,100 });
    ImGui::Begin("SpriteGui", &isSpriteGuiWnd, ImGuiWindowFlags_None);
    ImGui::SliderFloat2("float", xy, 0.0f, 1200.0f, "%.1f");
    ImGui::End();
}

void ImGuiController::Initialize(void)
{
    InitDirectX* iDX = InitDirectX::GetInstance();

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(Window::GetInstance()->GetHwnd());

    CreateSRVHeap();
    ImGui_ImplDX12_Init(
        iDX->GetDevice(),
        static_cast<int>(iDX->GetBackBufferCount()),
        DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
        srvHeap_.Get(),
        srvHeap_->GetCPUDescriptorHandleForHeapStart(),
        srvHeap_->GetGPUDescriptorHandleForHeapStart()
    );

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
}

void ImGuiController::Finalize(void)
{
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // srvHeap_.Reset();
}

void ImGuiController::CreateSRVHeap(void)
{
    D3D12_DESCRIPTOR_HEAP_DESC desc{};
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    desc.NumDescriptors = MAX_SRV_COUNT_FOR_IMGUI;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

    HRESULT r = InitDirectX::GetInstance()->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));
    assert(SUCCEEDED(r));
}
