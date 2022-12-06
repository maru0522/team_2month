#pragma once
#include <wrl.h>
#include <stdint.h>
#include <d3d12.h>

#pragma comment(lib,"d3d12.lib")

class ImGuiController
{
public: //Å@íËêî
    static constexpr uint32_t MAX_SRV_COUNT_FOR_IMGUI{ 1 };

public: // ä÷êî
    ImGuiController(void);
    ~ImGuiController(void);

    void Update(void);
    void Begin(void);
    void End(void);
    void Draw(void);

    void SpriteSlider(float xy[2]);

private: // ä÷êî
    void Initialize(void);
    void Finalize(void);

    void CreateSRVHeap(void);

private: // ïœêî
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;
};